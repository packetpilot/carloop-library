/* Open Exhaust Valve(s) on BMW vehicles with such equipment
 *
 * Copyright 2017 Dave Castelletti
 *
 * Distributed under the MIT license. See LICENSE.txt for more details.
 */
 
#include "application.h"
#include "carloop.h"

// Don't block the main program while connecting to WiFi/cellular.
// This way the main program runs on the Carloop even outside of WiFi range.
SYSTEM_THREAD(ENABLED);

// Tell the program which revision of Carloop you are using.
Carloop<CarloopRevision2> carloop;

// Send the message every 15 seconds
void sendMessage() {
  static uint32_t lastTransmitTime = 0;
  uint32_t transmitInterval = 15000; /* ms */
  uint32_t now = millis();
  if (now - lastTransmitTime > transmitInterval) {
    CANMessage message;

    // A CAN message has an ID that identifies the content inside
    message.id = 0x6F1;

    // It can have from 0 to 8 data bytes
    message.len = 8;

    // Pass the data to be transmitted in the data array
    message.data[0] = 0x12;
    message.data[1] = 0x10;
    message.data[2] = 0x08;
    message.data[3] = 0x2F;
    message.data[4] = 0x60;
    message.data[5] = 0xFD;
    message.data[6] = 0x03;
    message.data[7] = 0x19;

    // Send the message on the bus!
    carloop.can().transmit(message);

    // Pass the data to be transmitted in the data array
    message.data[0] = 0x12;
    message.data[1] = 0x21;
    message.data[2] = 0x99;
    message.data[3] = 0x0A;
    message.data[4] = 0xFF;
    message.data[5] = 0x58;
    message.data[6] = 0x00;
    message.data[7] = 0x00;

    // Send the message on the bus!
    carloop.can().transmit(message);

    lastTransmitTime = now;
  }
}

void setup() {
  // Configure the CAN bus speed for 500 kbps, the standard speed for the OBD-II port.
  // Other common speeds are 250 kbps and 1 Mbps.
  // If you don't call setCANSpeed, 500 kbps is used.
  carloop.setCANSpeed(500000);

  // Connect to the CAN bus
  carloop.begin();
}

void loop() {
  sendMessage();
}
