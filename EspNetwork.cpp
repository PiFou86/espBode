/* Specify DEBUG output target by defining DEBUG_TO_SERIAL or DEBUG_TO_TELNET (or NONE) */
//#define DEBUG_TO_SERIAL
#define DEBUG_TO_TELNET
// define PRINT macros
#ifndef PRINT_TO_SERIAL
    #define PRINT_TO_SERIAL(TEXT)   Serial.println(TEXT);
#endif
#ifndef PRINT_TO_TELNET
    #define PRINT_TO_TELNET(TEXT)   telnet.println(TEXT);
#endif
// define DEBUG output macro
#ifndef DEBUG
  #ifdef DEBUG_TO_SERIAL
    #define DEBUG(TEXT)         Serial.println(TEXT);
  #endif
  #ifdef DEBUG_TO_TELNET
    #include "ESPTelnet.h"
    extern ESPTelnet telnet;
    #define DEBUG(TEXT)         telnet.println(TEXT);
  #endif
#endif
#ifndef DEBUG
  #define DEBUG(TEXT)
#endif


#include "EspNetwork.h"
#include <ESP8266WiFi.h>
#include "Interfaces/IScpiDevice.h"
#include "Interfaces/LxiDeviceConfig.h"


EspNetwork::EspNetwork(
    WiFiClient* client,
    LxiDeviceConfig* lxiConfig,
    IScpiDevice *scpiDevice)
{
    _client = client;
    _lxiConfig = lxiConfig;
    _scpiDevice = scpiDevice;
}

/* Packet length must be four-bytes aligned. This function generates remaining
    bytes to fill the gap:

     unpadded   | padding    | packet length
    ------------+------------+-------------------
        0       |   0        |  0
        1       |   3        |  4
        2       |   2        |  4
        3       |   1        |  4
        4       |   0        |  4
        5       |   3        |  8
        6       |   2        |  8
        7       |   1        |  8
        ...     |   ...      |  ...
        n       | 3-(n+3)mod4|  unpadded+padding
    */
uint8_t EspNetwork::getPadding(uint8_t unpadded)
{
    return 3-(unpadded+3)%4;
}

/* Procedure to swap the endianness from big-endian (network) to
    little-endian (ESP). We use fast-xor swap to omit the usage of temporary
    accumulator when swapping the variables:

    a <-> b:
    a = a xor b
    b = b xor a
    b = a xor b

    We swap every four bytes: ABCD EFGH -> DCBA HGFE
    Procedure is symmetric.
    */
void EspNetwork::swapEndianess(uint8_t *data, uint8_t len)
{
    uint8_t i;
    len += getPadding(len);

    for(i=0;i<len;i+=4)
    {
            data[i]   ^= data[i+3];
            data[i+3] ^= data[i];
            data[i]   ^= data[i+3];

            data[i+1] ^= data[i+2];
            data[i+2] ^= data[i+1];
            data[i+1] ^= data[i+2];
    }
}

/* Function fills the RPC answer header. We use single-fragment communication,
    so bit #31 in the 'frag' field is always set. Also the length of the packet
    does not include the 'frag' field, so we need to subtract four bytes. */
void EspNetwork::fillResponseHeader(uint8_t *hdr, uint32_t xid, uint32_t length)
{
    /* NOTE: for some mysterious reasons current compiler refuses to use defined
        struct pointer as a function arguments. To be fixed someday.
        Currently we just pass void* then cast it to a more useful type. */
    rpcresp_header *header = (rpcresp_header*)hdr;

    header->frag = _lxiConfig->RPC_SINGLE_FRAG | (length - 4);
    header->xid = xid;
    header->msg_type = _lxiConfig->RPC_REPLY;
    header->reply_state = 0x00;
    header->verifier_l = 0x00;
    header->verifier_h = 0x00;
    header->rpc_status = 0x00;
}

/* Receives the RPC packet. User is responsible for destroying the pointer.
    Function is blocking.
    - Arguments:
        uint8_t**   -   output pointer to data
    - Returns:
        uint8_t     -   number of received bytes */
uint32_t EspNetwork::receiveRpcPacket(uint8_t **data)
{
    rpcreq_header   header;
    uint32_t        length;

    /* Wait till some data is available */
    while(!_client->available());

    /* Read the RPC header */
    _client->readBytes((uint8_t*)&header, sizeof(rpcreq_header));
    swapEndianess((uint8_t*)&header, sizeof(rpcreq_header));

    /* We only handle single-fragment VXI_11 packets */
    if((header.frag & _lxiConfig->RPC_SINGLE_FRAG) &&
       ((header.program == _lxiConfig->RPC_PROGRAM_PORTMAP) || header.program == _lxiConfig->RPC_PROGRAM_VXI11))
    {
        /* Length of the whole packet is stored in the RPC header */
        length = (header.frag & 0x7FFFFFFF) + 4;
        *data = (uint8_t*)malloc(length);

        /* Copy the header */
        memcpy((rpcreq_header*)*data, &header, sizeof(rpcreq_header));

        /* Receive remaining data (length - header) */
        while(!_client->available());
        _client->readBytes(*data + sizeof(rpcreq_header),
                          length - sizeof(rpcreq_header));
        swapEndianess((uint8_t*)(*data + sizeof(rpcreq_header)),
                      length - sizeof(rpcreq_header));

        return length;
    }
    else
    {
        *data = NULL;
        return 0;
    }
}

void EspNetwork::sendReadResponse(uint32_t xid)
{
    uint8_t length = sizeof(rcpresp_devReadWrite) + strlen(_lxiConfig->deviceID) + getPadding(strlen(_lxiConfig->deviceID));
    rcpresp_devReadWrite *response = (rcpresp_devReadWrite*)malloc(length);
    memset(response, 0, length);

    fillResponseHeader((uint8_t*)(&(response->header)), xid, length);

    response->errorCode = 0x00;
    response->reason = 0x04;
    response->dataLen = strlen((char*)_readBuffer) + getPadding(strlen((char*)_readBuffer));
    strcpy((char*)(response->data), (char*)_readBuffer);

    /* Note: we do not swap endianness of the data payload */
    swapEndianess((uint8_t*)response, sizeof(rcpresp_devReadWrite));

    while(!_client->availableForWrite());
    _client->write((uint8_t*)response, length);

    free(response);
}

/* Handles RPC portmap GETPORT request */
uint8_t EspNetwork::handlePortmap(uint8_t *packet)
{
    rpcreq_getport *getport = (rpcreq_getport*)packet;

    if(getport->header.procedure != _lxiConfig->PORTMAP_PROCEDURE_GETPORT)
    {
        DEBUG("ERROR: UNKNOWN RPC PACKET");
    }
    else
    {
        rpcresp_getport response;
        fillResponseHeader((uint8_t*)&(response.header), getport->header.xid, sizeof(rpcresp_getport));
        response.vxi_port = _lxiConfig->lxiServerPort;
        swapEndianess((uint8_t*)&response, sizeof(rpcresp_getport));
        while(!_client->availableForWrite());
        _client->write((uint8_t*)&response, sizeof(rpcresp_getport));
    }
    return 0;
}

void EspNetwork::parseVxiWrite(uint8_t *packet)
{
    rcpreq_devReadWrite *request = (rcpreq_devReadWrite*)packet;

    swapEndianess((uint8_t*)&(request->data), request->dataLen);
    //handleWriteMsg((char*)&(request->data), request->dataLen);

    // build _writeBuffer (make the request->data a 0 terminated string)
    _writeBuffer = (char*)malloc(request->dataLen + 1);
    strncpy(_writeBuffer, (char*)&(request->data), request->dataLen);
    _writeBuffer[request->dataLen] = 0;

    // execute SCPI command
    _scpiDevice->sendScpiCommand(_writeBuffer);

    if (_writeBuffer != nullptr) {
        free((void*)_writeBuffer); // Prevent memory leaks
        _writeBuffer = nullptr;
    }

    // build _readBuffer (copy the SCPI command replay)
    if (_readBuffer != nullptr) {
        free((void*)_readBuffer);  // Prevent memory leaks - also free in destructor!
        _readBuffer = nullptr;
    }
    _readBuffer = (char*)malloc(strlen(_scpiDevice->lastScpiReply()) + 1);
    strcpy((char*)_readBuffer, _scpiDevice->lastScpiReply());
}

uint8_t EspNetwork::handleVxi11(uint8_t *packet)
{
    rpcreq_header *header = (rpcreq_header*)packet;
    
    if (header->procedure == _lxiConfig->VXI11_PROCEDURE_CREATE_LINK)
    {
        DEBUG("CREATE_LINK");
        /* We confirm that the device is ready */
        rpcresp_createLink create_response;
        fillResponseHeader((uint8_t*)&(create_response.header), header->xid, sizeof(rpcresp_createLink));

        create_response.errorCode = 0x00;
        create_response.linkId = 0x00;
        create_response.abortPort = 0x00;
        create_response.maxReceiveSize = 0x00800000;

        swapEndianess((uint8_t*)&create_response, sizeof(rpcresp_createLink));
        while(!_client->availableForWrite());
        _client->write((uint8_t*)&create_response, sizeof(rpcresp_createLink));
        return 0;
    }

    if (header->procedure == _lxiConfig->VXI11_PROCEDURE_DESTROY_LINK)
    {
        DEBUG("DESTROY_LINK");
        /* Received at the end of the communication. We reset and wait for PORTMAP */
        return 1;
    }

    if (header->procedure == _lxiConfig->VXI11_PROCEDURE_DEV_READ)
    {
        DEBUG("DEV_READ");
        /* Answer with rcpresp_devReadWrite packet for DEV_READ */
        sendReadResponse(header->xid);
        return 0;
    }

    if (header->procedure == _lxiConfig->VXI11_PROCEDURE_DEV_WRITE)
    {
        DEBUG("DEV_WRITE ");
        parseVxiWrite((uint8_t*)header);

        rcpresp_devReadWrite write_response;
        fillResponseHeader((uint8_t*)&(write_response), header->xid, sizeof(rcpresp_devReadWrite));

        write_response.errorCode = 0x00;
        write_response.reason = 0x04;
        write_response.dataLen = 0x00;

        swapEndianess((uint8_t*)&write_response, sizeof(rcpresp_devReadWrite));
        while(!_client->availableForWrite());
        _client->write((uint8_t*)&write_response, sizeof(rcpresp_devReadWrite));
        return 0;
    }

    // unknown/unhandeled case -> error
    DEBUG("ERROR unknown or unhandeled VXI11 PROCEDURE");
    return 1;
}

uint8_t EspNetwork::handlePacket()
{
    DEBUG("EspNetwork::handlePacket() - start");

    uint8_t retVal = 0;
    rpcreq_header *header;
    if(0 == receiveRpcPacket((uint8_t**)&header))
    {
        /* ERROR: no packed received */
        DEBUG("ERROR: receiveRpcPacket failed");
        return 1;
    }

    if (header->program == _lxiConfig->RPC_PROGRAM_PORTMAP) 
    {
        DEBUG("EspNetwork::handlePacket() - CALLING handlePortmap()");
        retVal = handlePortmap((uint8_t*)header);
    } 
    else if (header->program == _lxiConfig->RPC_PROGRAM_VXI11)
    {
        DEBUG("EspNetwork::handlePacket() - CALLING handleVxi11()");
        retVal = handleVxi11((uint8_t*)header);
    }
    else
    {
        DEBUG("ERROR: unknown or unhandeled RPC PROGRAM");
        retVal = 1;
    }

    /* We are the user responsible for destroying the incoming packet */
    free(header);

    return retVal;
}
