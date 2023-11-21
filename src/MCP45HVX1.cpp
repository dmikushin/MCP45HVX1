/******************************************************
  Arduino library for MCP45HVX1 digital potentiometers

  Author: Jonathan Dempsey JDWifWaf@gmail.com

  Version: 1.0.1
  License: Apache 2.0
 *******************************************************/

#include "MCP45HVX1.h"

#define DEBUG 0

/* 7 Bit I2C Operation Components ......................................... */

/* TCON configuration.................. */
#define TCON_R0HW (0x08) // Shutdown Resistor Force
#define TCON_R0A (0x04)  // Terminal A Connection
#define TCON_R0W (0x02)  // Wiper Connection
#define TCON_R0B (0x01)  // Terminal B Connection

#define GCALL_TCON (0x60)
#define GCALL_WIPER (0x40)
#define GCALL_WIPERUP (0x42)
#define GCALL_WIPERDWN (0x44)
#define GCALL_COM_WRITE (0x02)
#define GCALL_COM_RWRITE (0x03)
#define GCALL_COM_WIPERINC (0x42)
#define GCALL_COM_WIPERDEC (0x44)

#define MEM_WIPER (0x00)
#define MEM_TCON (0x40)

#define COM_WRITE (0x00)
#define COM_READ (0x0C)
#define COM_WIPERINC (0x04)
#define COM_WIPERDEC (0x08)

using namespace mcp;

/* Setup ............................................................... */
MCP45HVX1::MCP45HVX1(TwoWire& wire, uint8_t address) : _wire(wire), _address(address) { };

MCP45HVX1::MCP45HVX1(uint8_t address) : _wire(Wire), _address(address) { };

void MCP45HVX1::begin(uint32_t clock) {
  _wire.begin();
  _wire.setClock(clock);
}

/* Wiper Register..........................................................*/
MCPError MCP45HVX1::writeWiper(uint8_t wiperValue) {
  _wire.beginTransmission(_address);
  _wire.write(MEM_WIPER | COM_WRITE);
  _wire.write(wiperValue);
  return static_cast<MCPError>(_wire.endTransmission());
}

MCPError MCP45HVX1::readWiper(uint8_t& buf) {
  _wire.beginTransmission(_address);
  _wire.write(MEM_WIPER | COM_READ);
  uint8_t status = _wire.endTransmission(false);
  if (status != 0)
    return static_cast<MCPError>(status);

  if (_wire.requestFrom(_address, (uint8_t)2) != 2)
    return MCPErrorOther;

  buf = _wire.read(); // First byte is 0x00
#if DEBUG
  Serial.print("\nRead Wiper MSB:  ");
  Serial.println(buf);
#endif
  buf = _wire.read(); // Second byte contains the wiper value
#if DEBUG
  Serial.print("Read Wiper LSB:  ");
  Serial.println(buf);
#endif

  return MCPSuccess;
}

MCPError MCP45HVX1::incrementWiper(uint8_t increments) {
  _wire.beginTransmission(_address);
  for (uint8_t x = 0; x < increments; x++) {
    _wire.write(MEM_WIPER | COM_WIPERINC);
  }
  return static_cast<MCPError>(_wire.endTransmission());
}

MCPError MCP45HVX1::decrementWiper(uint8_t decrements) {
  _wire.beginTransmission(_address);
  for (uint8_t x = 0; x < decrements; x++) {
    _wire.write(MEM_WIPER | COM_WIPERDEC);
  }
  return static_cast<MCPError>(_wire.endTransmission());
}

/* TCON Register...........................................................*/
MCPError MCP45HVX1::readTCON(TCONRegister &reg) {
  uint8_t buf = 0xF0;
  _wire.beginTransmission(_address);
  _wire.write(MEM_TCON | COM_READ);
  uint8_t status = _wire.endTransmission(false);
  if (status != 0)
    return static_cast<MCPError>(status);

  if (_wire.requestFrom(_address, (uint8_t)2) != 2)
    return MCPErrorOther;

  buf = _wire.read(); // First byte is always 0x00
#if DEBUG
  Serial.print("\nRead TCON MSB:  ");
  Serial.println(buf);
#endif
  buf = _wire.read(); // Second byte contains the wiper value (for
                                  // compatability)
#if DEBUG
  Serial.print("Read TCON LSB:  ");
  Serial.println(buf);
#endif

  memset(&this->TCON_lib_reg, 0, sizeof(TCONRegister));
  this->TCON_lib_reg.R0HW = buf & TCON_R0HW ? true : false;
  this->TCON_lib_reg.R0A = buf & TCON_R0A ? true : false;
  this->TCON_lib_reg.R0B = buf & TCON_R0B ? true : false;
  this->TCON_lib_reg.R0W = buf & TCON_R0W ? true : false;

  memcpy(&reg, &this->TCON_lib_reg, sizeof(TCONRegister));

  return MCPSuccess;
}

MCPError MCP45HVX1::defaultTCON() {
  this->TCON_lib_reg.R0HW = true;
  this->TCON_lib_reg.R0A = true;
  this->TCON_lib_reg.R0B = true;
  this->TCON_lib_reg.R0W = true;

  return static_cast<MCPError>(write_TCONRegister());
}

MCPError MCP45HVX1::writeTCON(const TCONRegister &reg) {
  memcpy(&this->TCON_lib_reg, &reg, sizeof(TCONRegister));
  return static_cast<MCPError>(write_TCONRegister());
}

MCPError MCP45HVX1::write_TCON_R0HW(bool isOn) {
  this->TCON_lib_reg.R0HW = isOn;
  return static_cast<MCPError>(write_TCONRegister());
}

MCPError MCP45HVX1::write_TCON_R0A(bool isOn) {
  this->TCON_lib_reg.R0A = isOn;
  return static_cast<MCPError>(write_TCONRegister());
}

MCPError MCP45HVX1::write_TCON_R0W(bool isOn) {
  this->TCON_lib_reg.R0W = isOn;
  return static_cast<MCPError>(write_TCONRegister());
}

MCPError MCP45HVX1::write_TCON_R0B(bool isOn) {
  this->TCON_lib_reg.R0B = isOn;
  return static_cast<MCPError>(write_TCONRegister());
}

MCPError MCP45HVX1::write_TCONRegister() {
  uint8_t buf = 0xF0;

  buf |= this->TCON_lib_reg.R0HW ? TCON_R0HW : 0;
  buf |= this->TCON_lib_reg.R0A ? TCON_R0A : 0;
  buf |= this->TCON_lib_reg.R0B ? TCON_R0B : 0;
  buf |= this->TCON_lib_reg.R0W ? TCON_R0W : 0;

#if DEBUG
  Serial.print("Writing TCON: ");
  Serial.println(buf);
#endif

  _wire.beginTransmission(_address);
  _wire.write(MEM_TCON | COM_WRITE);
  _wire.write(buf);
  return static_cast<MCPError>(_wire.endTransmission());
}
