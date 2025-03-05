#pragma once

#include <Arduino.h>

#include "AWGProxy/AWGProxyBase.h"

class AWGProxyFY6900 : public AWGProxyBase {
 public:
  virtual bool setCh1Wave(uint8_t wave) override;
  virtual bool setCh2Wave(uint8_t wave) override;

  /* 0 - off; 1 - on */
  virtual bool setCh1Output(uint32_t output) override;
  virtual bool setCh2Output(uint32_t output) override;

  /* Set frequency in Hz */
  virtual bool setCh1Freq(uint32_t frequency) override;
  virtual bool setCh2Freq(uint32_t frequency) override;

  /* Ampl is in mV: 12.345V = 12345 */
  virtual bool setCh1Ampl(uint32_t ampl) override;
  virtual bool setCh2Ampl(uint32_t ampl) override;

  /* Phase is in 0.1deg: 12.5deg = 125 */
  virtual bool setCh1Phase(uint32_t phase) override;
  virtual bool setCh2Phase(uint32_t phase) override;

  /* Offset is in mV: 12.345V = 12345 */
  virtual bool setCh1Offset(int32_t offset) override;
  virtual bool setCh2Offset(int32_t offset) override;

  /* Can be used to set some default parameters */
  virtual bool begin(void) override;

  virtual ~AWGProxyFY6900();

 private:
  bool writeCommandToSerial(char* data, uint8_t len);
  bool sendCommand(const char* cmdPattern, uint32_t param1, uint32_t param2);
  bool sendCommand(const char* cmdPattern, uint32_t param1);
  bool sendCommand(const char* cmdPattern);

  volatile SDeviceState _gDeviceState;
};
