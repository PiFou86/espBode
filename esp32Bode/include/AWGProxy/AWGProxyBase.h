#pragma once

#include <Arduino.h>

class AWGProxyBase {
 public:
  virtual bool setCh1Wave(uint8_t wave) = 0;
  virtual bool setCh2Wave(uint8_t wave) = 0;

  /* 0 - off; 1 - on */
  virtual bool setCh1Output(uint32_t output) = 0;
  virtual bool setCh2Output(uint32_t output) = 0;

  /* Set frequency in Hz */
  virtual bool setCh1Freq(uint32_t frequency) = 0;
  virtual bool setCh2Freq(uint32_t frequency) = 0;

  /* Ampl is in mV: 12.345V = 12345 */
  virtual bool setCh1Ampl(uint32_t ampl) = 0;
  virtual bool setCh2Ampl(uint32_t ampl) = 0;

  /* Phase is in 0.1deg: 12.5deg = 125 */
  virtual bool setCh1Phase(uint32_t phase) = 0;
  virtual bool setCh2Phase(uint32_t phase) = 0;

  /* Offset is in mV: 12.345V = 12345 */
  virtual bool setCh1Offset(int32_t offset) = 0;
  virtual bool setCh2Offset(int32_t offset) = 0;

  /* Can be used to set some default parameters */
  virtual bool begin(void) = 0;

  virtual ~AWGProxyBase() {}

 protected:
  typedef struct {
    uint8_t ch1Output;
    uint8_t ch2Output;
    uint8_t ch1Wave;
    uint8_t ch2Wave;
    uint32_t ch1Freq;
    uint32_t ch2Freq;
    uint32_t ch1Ampl;
    uint32_t ch2Ampl;
    uint32_t ch1Phase;
    uint32_t ch2Phase;
    uint32_t ch1Offset;
    uint32_t ch2Offset;
  } SDeviceState;

  volatile SDeviceState _gDeviceState;
};
