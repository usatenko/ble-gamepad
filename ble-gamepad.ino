#include <bluefruit.h>

BLEDis bledis;
BLEHidGamepad blegamepad;

hid_gamepad_report_t gp;

#include <Arduino.h>
#include "Adafruit_TinyUSB.h"

// #define debug

void setup() {
  #if defined(debug)
    Serial.begin(115200);
    while (!Serial);
  #endif

    Bluefruit.begin();
    Bluefruit.setName("USO Game Controller");
    Bluefruit.setTxPower(4);    // Check bluefruit.h for supported values

    // Configure and Start Device Information Service
    bledis.setManufacturer("USO Industries");
    bledis.setModel("Model 1");

    bledis.begin();
    blegamepad.begin();
    startAdv();
}

void startAdv(void) {
  // Advertising packet
  Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);
  Bluefruit.Advertising.addTxPower();
  Bluefruit.Advertising.addAppearance(BLE_APPEARANCE_HID_GAMEPAD);

  // Include BLE HID service
  Bluefruit.Advertising.addService(blegamepad);

  // There is enough room for the dev name in the advertising packet
  Bluefruit.Advertising.addName();

  Bluefruit.Advertising.restartOnDisconnect(true);
  Bluefruit.Advertising.setInterval(32, 244);    // in unit of 0.625 ms
  Bluefruit.Advertising.setFastTimeout(30);      // number of seconds in fast mode
  Bluefruit.Advertising.start(0);                // 0 = Don't stop advertising after n seconds
}

void loop() {
  int a0 = analogRead(A0);
  int a1 = analogRead(A1);
  int a2 = analogRead(A2);
  int a3 = analogRead(A3);
  int a4 = analogRead(A4);

    // nothing to do if not connected or
  if ( !Bluefruit.connected() ) return;

  gp.x       = 0;
  gp.y       = 0;
  gp.z       = 0;
  gp.rz      = 0;
  gp.rx      = 0;
  gp.ry      = 0;
  gp.hat     = 0;
  gp.buttons = 0;

  gp.x = map(min(a2, 937), 0, 938, -127, 128);
  gp.y = map(min(a1, 937), 0, 938, -127, 128);

  gp.z = map(min(a3, 937), 0, 938, -127, 128);
  gp.rz = map(min(a4, 937), 0, 938, -127, 128);
  
  int b = a0; map(min(a0, 937), 0, 938, 0, 100);
  if (b <= 10) {
    gp.buttons = 1ul << 0;
  }
  if (b >= 45 && b <= 65) {
    gp.buttons = 1ul << 1;
  }
  if (b >= 565 && b <= 585) {
    gp.buttons = 1ul << 2;
  }
  if (b > 640 && b <= 660) {
    gp.buttons = 1ul << 3;
  }
  if (b > 248 && b <= 268) {
    gp.buttons = 1ul << 4;
  }
  if (b > 184 && b <= 204) {
    gp.buttons = 1ul << 5;
  }
  if (b > 340 && b <= 360) {
    gp.buttons = 1ul << 6;
  }
  if (b > 410 && b <= 430) {
    gp.buttons = 1ul << 7;
  }
  blegamepad.report(&gp);

#if defined(debug)
  Serial.println(String(gp.x) + " " + String(gp.y) + " " + String(gp.z) + " " + String(gp.rz) + " " + String(gp.buttons) + " " + String(a0));
#endif

}
