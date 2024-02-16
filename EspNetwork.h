#pragma once

//class WiFiClient;
#include <ESP8266WiFi.h>
//class IScpiDevice;
#include "IScpiDevice.h"
//class LxiDeviceConfig;
#include "LxiDeviceConfig.h"
//class ITerminal;
#include "ITerminal.h"

#include <stdint.h>

typedef struct
{
    uint32_t   frag;
    uint32_t   xid;
    uint32_t   msg_type;
    uint32_t   rpc_version;
    uint32_t   program;
    uint32_t   program_version;
    uint32_t   procedure;
    uint32_t   credentials_l;
    uint32_t   credentials_h;
    uint32_t   verifier_l;
    uint32_t   verifier_h;
}rpcreq_header;

typedef struct
{
    uint32_t   frag;
    uint32_t   xid;
    uint32_t   msg_type;
    uint32_t   reply_state;
    uint32_t   verifier_l;
    uint32_t   verifier_h;
    uint32_t   rpc_status;
}rpcresp_header;

typedef struct
{
    rpcreq_header  header;

    uint32_t   getport_program;
    uint32_t   getport_version;
    uint32_t   getport_protocol;
    uint32_t   getport_port;
}rpcreq_getport;

typedef struct
{
    rpcresp_header  header;

    uint32_t   vxi_port;
}rpcresp_getport;

typedef struct
{
    rpcreq_header   header;

    uint32_t   linkId;
    uint32_t   ioTimeout;
    uint32_t   lockTimeout;
    uint32_t   flags;
    uint32_t   dataLen;
    uint8_t    data[];

}rcpreq_devReadWrite;

typedef struct
{
    rpcresp_header   header;

    uint32_t   errorCode;
    uint32_t   reason;
    uint32_t   dataLen;
    uint8_t    data[];

}rcpresp_devReadWrite;

typedef struct
{
    rpcresp_header  header;

    uint32_t   errorCode;
    uint32_t   linkId;
    uint32_t   abortPort;
    uint32_t   maxReceiveSize;
}rpcresp_createLink;



class EspNetwork
{
private:
    WiFiClient* _client;
    LxiDeviceConfig* _lxiConfig;
    IScpiDevice* _scpiDevice;
    ITerminal* _terminal;
    char* _writeBuffer;
    char* _readBuffer;

public:
    EspNetwork(WiFiClient* client, LxiDeviceConfig* lxiConfig, IScpiDevice* scpiDevice, ITerminal* terminal);

    uint8_t handlePacket();

private:
    uint8_t getPadding(uint8_t unpadded);
    void swapEndianess(uint8_t* data, uint8_t len);
    void fillResponseHeader(uint8_t* hdr, uint32_t xid, uint32_t length);
    uint32_t receiveRpcPacket(uint8_t** data);
    void sendReadResponse(uint32_t xid);
    
    uint8_t handlePortmap(uint8_t* packet);
    void parseVxiWrite(uint8_t* packet);
    uint8_t handleVxi11(uint8_t* packet);
};
