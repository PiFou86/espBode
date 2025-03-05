#pragma once

#include <Arduino.h>

#include "AWGProxy/AWGProxyBase.h"

class AWGParser {
 public:
  AWGParser(AWGProxyBase* awgProxy) : _awgProxy(awgProxy) {}

  void handleWriteMsg(char* msg, uint8_t len);

 private:
  AWGProxyBase* _awgProxy;
  volatile char* _gReadBuffer;

  friend class AWGNetwork;
};
