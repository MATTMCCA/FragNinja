/******* TESTED ON ACTUAL HARDWARE *******

   Frag-Firmware | MJM | 9/26/2020

   Shows up as HID Gamepad [DPAD Control]

   REF: https://github.com/dmadison/NintendoExtensionCtrl
        https://github.com/NicoHood/HID
        https://github.com/NicoHood/HID/issues/194

   Notes: Red LED Indicates Nunchuk Connection Failure (cycle power)

   Runs on Unofficial Test Hardware (9/26/2020)
   Runs on Frag-Ninja (9/26/2020)

 ******* TESTED ON ACTUAL HARDWARE *******/

#include "HID-Project.h"
#include <NintendoExtensionCtrl.h>

#define DEAD_ZONE       5
#define NULL_STICK      128 // center pos
#define _POSITIVE       (NULL_STICK + DEAD_ZONE)
#define _NEGITIVE       (NULL_STICK - DEAD_ZONE)

Nunchuk nchuk;
bool LED = LOW;

void setup() {
  // put your setup code here, to run once:
  digitalWrite(LED_BUILTIN, HIGH);
  nchuk.begin();
  Gamepad.begin();
  while (!nchuk.connect()) delay(1000);
}

void loop() {
  // put your main code here, to run repeatedly:
  bool success = nchuk.update();
  if (!success) {
    delay(1000);
  }
  else {
    bool zButton = nchuk.buttonZ();
    bool cButton = nchuk.buttonC();
    int joyY = nchuk.joyY();
    int joyX = nchuk.joyX();

    bool state[4] = {false};
    state[0] = (joyY >= _POSITIVE) ? true : false; /* UP */
    state[1] = (joyY <= _NEGITIVE) ? true : false; /* DOWN */
    state[2] = (joyX >= _POSITIVE) ? true : false; /* RIGHT */
    state[3] = (joyX <= _NEGITIVE) ? true : false; /* LEFT */

    uint8_t dpad1 = GAMEPAD_DPAD_CENTERED;    // 0
    if (state[0]) dpad1 = GAMEPAD_DPAD_UP;    // 1
    if (state[2]) dpad1 = GAMEPAD_DPAD_RIGHT; // 3
    if (state[1]) dpad1 = GAMEPAD_DPAD_DOWN;  // 5
    if (state[3]) dpad1 = GAMEPAD_DPAD_LEFT;  // 7
    if (state[0] & state[2]) dpad1 = GAMEPAD_DPAD_UP_RIGHT;   // 2
    if (state[1] & state[2]) dpad1 = GAMEPAD_DPAD_DOWN_RIGHT; // 4
    if (state[1] & state[3]) dpad1 = GAMEPAD_DPAD_DOWN_LEFT;  // 6
    if (state[0] & state[3]) dpad1 = GAMEPAD_DPAD_UP_LEFT;    // 8

    cButton == true ? Gamepad.press(1) : Gamepad.release(1);
    zButton == true ? Gamepad.press(2) : Gamepad.release(2);

    Gamepad.dPad1(dpad1);
    Gamepad.write();
  }

  if (success != LED) {
    LED = success;
    digitalWrite(LED_BUILTIN, !LED);
  }
}
