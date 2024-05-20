/*
 * SimpleSender.cpp
 *
 *  Demonstrates sending IR codes in standard format with address and command
 *  An extended example for sending can be found as SendDemo.
 *
 *  Copyright (C) 2020-2022  Armin Joachimsmeyer
 *  armin.joachimsmeyer@gmail.com
 *
 *  This file is part of Arduino-IRremote https://github.com/Arduino-IRremote/Arduino-IRremote.
 *
 *  MIT License
 */
#include <Arduino.h>

#define IR_SEND_PIN 3

#if !defined(ARDUINO_ESP32C3_DEV) // This is due to a bug in RISC-V compiler, which requires unused function sections :-(.
#define DISABLE_CODE_FOR_RECEIVER // Disables static receiver code like receive timer ISR handler and static IRReceiver and irparams data. Saves 450 bytes program memory and 269 bytes RAM if receiving functions are not required.
#endif
//#define SEND_PWM_BY_TIMER         // Disable carrier PWM generation in software and use (restricted) hardware PWM.
//#define USE_NO_SEND_PWM           // Use no carrier PWM, just simulate an active low receiver signal. Overrides SEND_PWM_BY_TIMER definition

/*
 * This include defines the actual pin number for pins like IR_RECEIVE_PIN, IR_SEND_PIN for many different boards and architectures
 */
#include <IRremote.hpp> // include the library

// Variables for sending IR data
uint32_t dataToSend = 0;
uint8_t sent = 0;
uint8_t currentRow = 0;
uint8_t currentColumn = 0;
uint8_t dataArray[4];

void setup() {
    Serial.begin(115200);
    Serial.setTimeout(1); 

    // The IR library setup. That's all!
    IrSender.begin(); // Start with IR_SEND_PIN -which is defined in PinDefinitionsAndMore.h- as send pin and enable feedback LED at default feedback LED pin
    disableLEDFeedback(); // Disable feedback LED at default feedback LED pin


}

void loop() {

      while (Serial.available() != 8);

      String hexString = Serial.readStringUntil('\n'); // Read hexadecimal string from serial
      // Convert hexadecimal string to unsigned long
      dataToSend = strtoul(hexString.c_str(), NULL, 16);
      Serial.println(dataToSend, HEX); // Print the unsigned long in hexadecimal format

      // Send the IR signal
      IrSender.sendNEC(dataToSend, 32);

      // Delay must be greater than 5 ms (RECORD_GAP_MICROS), otherwise the receiver sees it as one long signal
      delay(15);


}