/******************************************************
  Arduino library for MCP45HVX1 digital potentiometers

  Author: Jonathan Dempsey JDWifWaf@gmail.com

  Version: 1.0.1
  License: Apache 2.0
 *******************************************************/

#ifndef _MCP45HVX1_H
#define _MCP45HVX1_H

#include <Arduino.h>
#include <Wire.h>

namespace mcp {

enum MCPError {
  MCPSuccess = 0,
  MCPErrorLengthTooLong = 1,
  MCPErrorAddressSentNackReceived = 2,
  MCPErrorDataSentNackReceived = 3,
  MCPErrorOther = 4, // lost bus arbitration, bus error, etc.
  MCPErrorTimeout = 5
};

/* TCON Register Resistor Configuration ............................... */
struct TCONRegister {
  bool R0HW = true;
  bool R0A = true;
  bool R0B = true;
  bool R0W = true;
};

class MCP45HVX1 {
public:
  static const uint32_t MCPClockSpeed = 100000;

  /* Setup ............................................................... */
  MCP45HVX1(TwoWire &wire = Wire, uint8_t address = 0x3C);

  MCP45HVX1(uint8_t address = 0x3C);

  void begin(uint32_t speed = MCPClockSpeed);

  /* Wiper Register ...................................................... */
  MCPError readWiper(uint8_t &wiperValue);
  MCPError writeWiper(uint8_t wiperValue);
  MCPError incrementWiper(uint8_t increments = 1);
  MCPError decrementWiper(uint8_t decrements = 1);

  /* TCON Register ....................................................... */
  MCPError readTCON(TCONRegister &reg);
  MCPError writeTCON(const TCONRegister &reg);
  MCPError defaultTCON();

  MCPError inline shutdown() { write_TCON_R0HW(false); };
  MCPError inline startup() { write_TCON_R0HW(true); };
  MCPError inline connectTerminalA() { write_TCON_R0A(true); };
  MCPError inline disconnectTerminalA() { write_TCON_R0A(false); };
  MCPError inline connectTerminalB() { write_TCON_R0B(true); };
  MCPError inline disconnectTerminalB() { write_TCON_R0B(false); };
  MCPError inline connectWiper() { write_TCON_R0W(true); };
  MCPError inline disconnectWiper() { write_TCON_R0W(false); };

private:
  uint8_t _address;

  TwoWire& _wire;
  TCONRegister TCON_lib_reg;

  MCPError write_TCONRegister();
  MCPError write_TCON_R0HW(bool state);
  MCPError write_TCON_R0A(bool state);
  MCPError write_TCON_R0W(bool state);
  MCPError write_TCON_R0B(bool state);
};

} // namespace mcp

#endif

