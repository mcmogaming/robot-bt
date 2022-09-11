/**
 * Bluetooth Classic Example
 * Scan for devices - asyncronously, print device as soon as found
 * query devices for SPP - SDP profile
 * connect to first device offering a SPP connection
 *
 * Example python server:
 * source: https://gist.github.com/ukBaz/217875c83c2535d22a16ba38fc8f2a91
 *
 * Tested with Raspberry Pi onboard Wifi/BT, USB BT 4.0 dongles, USB BT 1.1
 * dongles, 202202: does NOT work with USB BT 2.0 dongles when esp32 aduino lib
 * is compiled with SSP support! see
 * https://github.com/espressif/esp-idf/issues/8394
 *
 * use ESP_SPP_SEC_ENCRYPT|ESP_SPP_SEC_AUTHENTICATE in connect() if remote side
 * requests 'RequireAuthentication': dbus.Boolean(True), use ESP_SPP_SEC_NONE
 * or ESP_SPP_SEC_ENCRYPT|ESP_SPP_SEC_AUTHENTICATE in connect() if remote side
 * has Authentication: False
 */

#include "BluetoothSerial.h"
#include "encoder.h"
#include "motorcontroller.h"
#include <Arduino.h>

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

#if !defined(CONFIG_BT_SPP_ENABLED)
#error Serial Bluetooth not available or not enabled. It is only available for the ESP32 chip.
#endif

BluetoothSerial SerialBT;

int incomingByte;

void
setup ()
{
  Serial.begin (115200);
  SerialBT.begin ("Rover"); // Bluetooth device name
  Serial.println ("The device started, now you can pair it with bluetooth!");
  // setupMotors ();
  setupEncoders ();
}

void
loop ()
{
  // if (SerialBT.available ())
  //   {
  //     incomingByte = SerialBT.read ();

  //     if (incomingByte < 0)
  //       return;

  //     switch (incomingByte)
  //       {
  //       case 'w':
  //         forward (100);
  //         break;
  //       case 's':
  //         backward (100);
  //         break;
  //       case 'a':
  //         left (100);
  //         break;
  //       case 'd':
  //         right (100);
  //         break;
  //       default:
  //         brake ();
  //         break;
  //       }
  //   }
  // else
  //   {
  //     brake ();
  //   }
  // delay (20);

  // Serial.printf ("( %d %d ) ( %d %d ) ( %d %d ) ( %d %d )\n",
  //                digitalRead (ENCODER1_PIN_A), digitalRead (ENCODER1_PIN_B),
  //                digitalRead (ENCODER2_PIN_A), digitalRead (ENCODER2_PIN_B),
  //                digitalRead (ENCODER3_PIN_A), digitalRead (ENCODER3_PIN_B),
  //                digitalRead (ENCODER4_PIN_A), digitalRead
  //                (ENCODER4_PIN_B));
  updateEncoders ();
  delay (20);
}