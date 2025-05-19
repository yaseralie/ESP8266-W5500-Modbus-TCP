/*
  ModbusTCP for W5x00 Ethernet library
  Basic Server code example

  (c)2020 Alexander Emelianov (a.m.emelianov@gmail.com)
  https://github.com/emelianov/modbus-esp8266
  GPIO Reference: https://randomnerdtutorials.com/esp8266-pinout-reference-gpios/
  This code is licensed under the BSD New License. See LICENSE.txt for more info.
*/

#include <SPI.h>
#include <Ethernet.h>       // Ethernet library v2 is required
#include <ModbusEthernet.h>

// Enter a MAC address and IP address for your controller below.
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
IPAddress ip(192, 168, 0, 210); // The IP address will be dependent on your local network:
ModbusEthernet mb;              // Declare ModbusTCP instance

int relay1 = 4;  //D2
int relay2 = 2;  //D4

void setup() {
  Serial.begin(9600);     // Open serial communications and wait for port to open
  //Output GPIO
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);

  Ethernet.init(5);        // SS pin
  Ethernet.begin(mac, ip);  // start the Ethernet connection
  delay(1000);              // give the Ethernet shield a second to initialize
  mb.server();              // Act as Modbus TCP server
  //configure coil
  mb.addCoil(0);
  mb.addCoil(1);

  //turn off all relay
  digitalWrite(relay1, HIGH);
  digitalWrite(relay2, HIGH);
}

void loop() {
  mb.task();                // Server Modbus TCP queries
  //Attach Relay to Coil register
  int coil1 = mb.Coil(0);
  int coil2 = mb.Coil(1);

  //because I use relay with ON if ground, so I inverse the value
  digitalWrite(relay1, !coil1);
  digitalWrite(relay2, !coil2);
  delay(50);
}
