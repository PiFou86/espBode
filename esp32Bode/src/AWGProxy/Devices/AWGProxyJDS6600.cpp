#include "AWGProxy/Devices/AWGProxyJDS6600.h"

#include "esp_config.h"

#define EWaveType_Sine 0

bool AWGProxyJDS6600::write(char* data, uint8_t len) {
  uint32_t timeout = 0;
  Serial.write((uint8_t*)data, len);
  telnet.println("[");
  telnet.print(data);
  telnet.println("]");
  while (!Serial.available()) {
    delay(1);
    if (timeout++ > 1000) return false;
  }
  bool ok = false;
  ok = (Serial.read() == 0x0d, 0x0a);  // 0x0a == \n 0x0d == \r

  if (!ok) {
    telnet.println("Invalid response for command");
  }
  delay(10);

  return ok;
}

bool AWGProxyJDS6600::setCh1Wave(uint8_t wave) {
  char command[] = ":w21=00.\r\n";
  snprintf(command, 11, ":w21=%02u.\r\n", wave);
  _gDeviceState.ch1Wave = wave;
  write(command, 10);
  delay(10);

  return true;
}

bool AWGProxyJDS6600::setCh2Wave(uint8_t wave) {
  char command[] = ":w22=00.\r\n";
  snprintf(command, 11, ":w22=%02u.\r\n", wave);
  _gDeviceState.ch1Wave = wave;
  write(command, 10);
  delay(10);

  return true;
}

bool AWGProxyJDS6600::setCh1Output(uint32_t output) {
  _gDeviceState.ch1Output = output;
  write((char*)(output ? ":w20=1,0.\r\n" : "w20=0,0.\r\n"), 11);

  return true;
}

bool AWGProxyJDS6600::setCh2Output(uint32_t output) {
  _gDeviceState.ch2Output = output;
  write((char*)(output ? ":w20=1,1.\r\n" : ":w20=1,0.\r\n"), 11);

  return true;
}

/* Set frequency in Hz */
bool AWGProxyJDS6600::setCh1Freq(uint32_t frequency) {
  if (_gDeviceState.ch1Freq != frequency) {
    char command[] = ":w23=0000000000,0.\r\n";
    snprintf(command, 21, ":w23=%10u,0.\r\n", frequency * 100);
    _gDeviceState.ch1Freq = frequency;
    write(command, 20);
  }

  return true;
}

/* Set frequency in Hz */
bool AWGProxyJDS6600::setCh2Freq(uint32_t frequency) {
  char command[] = ":w24=0000000000,0.\r\n";
  snprintf(command, 21, ":w24=%10u,0.\r\n", frequency * 100);
  _gDeviceState.ch2Freq = frequency;
  write(command, 20);

  return true;
}

/* Ampl is in mV: 12.345V = 12345 */
bool AWGProxyJDS6600::setCh1Ampl(uint32_t ampl) {
  char command[] = ":w25=00000.\r\n";
  snprintf(command, 14, ":w25=%05u.\r\n", ampl);
  _gDeviceState.ch1Ampl = ampl;
  write(command, 13);

  return true;
}

bool AWGProxyJDS6600::setCh2Ampl(uint32_t ampl) {
  char command[] = ":w26=00000.\r\n";
  snprintf(command, 14, ":w26=%05u.\r\n", ampl);
  _gDeviceState.ch2Ampl = ampl;
  write(command, 13);

  return true;
}

/* Phase is in 0.1deg: 12.5deg = 125 */
bool AWGProxyJDS6600::setCh1Phase(uint32_t phase) {
  char command[] = "w:31=0000.\r\n";
  snprintf(command, 13, ":w31=%04u.\r\n", phase);
  _gDeviceState.ch1Phase = phase;
  write(command, 12);
  delay(10);

  return true;
}

bool AWGProxyJDS6600::setCh2Phase(uint32_t phase) {
  char command[] = ":w31=0000.\r\n";
  snprintf(command, 13, ":w31=%04u.\r\n", phase);
  _gDeviceState.ch2Phase = phase;
  write(command, 12);
  delay(10);

  return true;
}

bool AWGProxyJDS6600::setCh1Offset(int32_t offset) {
  char command[] = ":w27=0000.\r\n";
  _gDeviceState.ch1Offset = offset;
  snprintf(command, 13, ":w27=%04u.\r\n", offset / 10 + 1000);
  write(command, 12);

  return true;
}

bool AWGProxyJDS6600::setCh2Offset(int32_t offset) {
  char command[] = ":w28=0000.\r\n";
  _gDeviceState.ch2Offset = offset;
  snprintf(command, 13, ":w28=%04u.\r\n", offset / 10 + 1000);
  write(command, 12);

  return true;
}

bool AWGProxyJDS6600::begin(void) {
  Serial.write((uint8_t*)"\r\n", 3);

  setCh1Output(0);
  setCh1Wave(EWaveType_Sine);
  setCh1Freq(1000);
  setCh1Ampl(1000);
  setCh1Offset(0);

  setCh2Output(0);
  setCh2Wave(EWaveType_Sine);
  setCh2Freq(1000);
  setCh2Ampl(1000);
  setCh2Offset(0);

  return true;
}
