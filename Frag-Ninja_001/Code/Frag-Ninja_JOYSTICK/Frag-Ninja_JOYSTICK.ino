/******* TESTED ON ACTUAL HARDWARE *******

   Frag-Firmware | MJM | 9/26/2020

   Shows up as HID Gamepad [JOYSTICK Control]

   REF: https://github.com/dmadison/NintendoExtensionCtrl
        https://github.com/NicoHood/HID
        https://github.com/NicoHood/HID/issues/194

   Notes: Red LED Indicates Nunchuk Connection Failure (cycle power)

   Runs on Unofficial Test Hardware (9/26/2020)
   Runs on Frag-Ninja (9/26/2020)

 ******* TESTED ON ACTUAL HARDWARE *******/

#include "HID-Project.h"
#include <NintendoExtensionCtrl.h>

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

    int16_t _y = map(joyY, 0, 255, 32767, -32768);
    int16_t _x = map(joyX, 0, 255, -32768, 32767);

    Gamepad.yAxis(_y);
    Gamepad.xAxis(_x);

    cButton == true ? Gamepad.press(1) : Gamepad.release(1);
    zButton == true ? Gamepad.press(2) : Gamepad.release(2);

    Gamepad.write();
  }

  if (success != LED) {
    LED = success;
    digitalWrite(LED_BUILTIN, !LED);
  }
}
