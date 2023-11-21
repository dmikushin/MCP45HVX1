#include "MCP45HVX1.h"

#define CHECK_ERROR(call) \
  do { \
    auto status = call; \
    if (status != MCPSuccess) { \
      Serial.print("Error "); \
      Serial.print(status); \
      Serial.print(" at "); \
      Serial.print(__FILE__); \
      Serial.print(":"); \
      Serial.println(__LINE__); \
    } \
  } \
  while (0)

using namespace mcp;

MCP45HVX1 digiPot(0x3C);

void setup()
{  
  Serial.begin(9600);

  digiPot.begin();  

  Serial.println("....... Functionality Test Begin ..........");
  /* Wiper ........................... */
  CHECK_ERROR(digiPot.writeWiper(127));                 // Baseline Establish

  Serial.println("\n----- Wiper Register ----");
  Serial.print("readWiper: ");
  uint8_t wiperValue;
  CHECK_ERROR(digiPot.readWiper(wiperValue));
  Serial.println(wiperValue);

  Serial.print("writeWiper: ");
  CHECK_ERROR(digiPot.writeWiper(200));
  CHECK_ERROR(digiPot.readWiper(wiperValue));
  Serial.println(wiperValue);

  Serial.print("incrementWiper: ");
  CHECK_ERROR(digiPot.incrementWiper());
  CHECK_ERROR(digiPot.readWiper(wiperValue));
  Serial.println(wiperValue);

  Serial.print("incrementWiper by 2: ");
  CHECK_ERROR(digiPot.incrementWiper(2));
  CHECK_ERROR(digiPot.readWiper(wiperValue));
  Serial.println(wiperValue);

  Serial.print("decrementWiper: ");
  CHECK_ERROR(digiPot.decrementWiper());
  CHECK_ERROR(digiPot.readWiper(wiperValue));
  Serial.println(wiperValue);

  Serial.print("decrementWiper by 2: ");
  CHECK_ERROR(digiPot.decrementWiper(2));
  CHECK_ERROR(digiPot.readWiper(wiperValue));
  Serial.println(wiperValue);

  /* TCON .......................... */
  Serial.println("\n----- TCON Register ----");
  CHECK_ERROR(digiPot.disconnectTerminalA());
  Serial.print("disconnectTerminalA: ");
  TCONRegister reg;
  CHECK_ERROR(digiPot.readTCON(reg));
  Serial.print(reg.R0A); Serial.print(" ");
  Serial.print(reg.R0B); Serial.print(" ");
  Serial.print(reg.R0HW); Serial.print(" ");
  Serial.println(reg.R0W);
  CHECK_ERROR(digiPot.connectTerminalA());
  
  CHECK_ERROR(digiPot.disconnectTerminalB());
  Serial.print("disconnectTerminalB: ");
  CHECK_ERROR(digiPot.readTCON(reg));
  Serial.print(reg.R0A); Serial.print(" ");
  Serial.print(reg.R0B); Serial.print(" ");
  Serial.print(reg.R0HW); Serial.print(" ");
  Serial.println(reg.R0W);
  CHECK_ERROR(digiPot.connectTerminalB());

  CHECK_ERROR(digiPot.disconnectWiper());
  Serial.print("disconnectWiper: ");
  CHECK_ERROR(digiPot.readTCON(reg));
  Serial.print(reg.R0A); Serial.print(" ");
  Serial.print(reg.R0B); Serial.print(" ");
  Serial.print(reg.R0HW); Serial.print(" ");
  Serial.println(reg.R0W);
  CHECK_ERROR(digiPot.connectWiper());

  CHECK_ERROR(digiPot.shutdown());
  Serial.print("shutdown: ");
  CHECK_ERROR(digiPot.readTCON(reg));
  Serial.print(reg.R0A); Serial.print(" ");
  Serial.print(reg.R0B); Serial.print(" ");
  Serial.print(reg.R0HW); Serial.print(" ");
  Serial.println(reg.R0W);
  CHECK_ERROR(digiPot.startup());

  CHECK_ERROR(digiPot.startup());
  Serial.print("startup: ");
  CHECK_ERROR(digiPot.readTCON(reg));
  Serial.print(reg.R0A); Serial.print(" ");
  Serial.print(reg.R0B); Serial.print(" ");
  Serial.print(reg.R0HW); Serial.print(" ");
  Serial.println(reg.R0W);
  CHECK_ERROR(digiPot.startup());

  CHECK_ERROR(digiPot.defaultTCON());
  Serial.print("default: ");
  CHECK_ERROR(digiPot.readTCON(reg));
  Serial.print(reg.R0A); Serial.print(" ");
  Serial.print(reg.R0B); Serial.print(" ");
  Serial.print(reg.R0HW); Serial.print(" ");
  Serial.println(reg.R0W);

  Serial.println("\n........ Functionality Test End ...........");
}

void loop()
{
  while (1)
    continue;
} 

