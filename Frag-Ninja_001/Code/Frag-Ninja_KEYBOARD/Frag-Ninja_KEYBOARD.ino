/******* TESTED ON ACTUAL HARDWARE *******

   Frag-Firmware | MJM | 9/26/2020

   Shows up as HID Keyboard [WASD Control]

   REF: https://github.com/dmadison/NintendoExtensionCtrl
        https://www.arduino.cc/reference/en/language/functions/usb/keyboard/
        https://www.arduino.cc/reference/en/language/functions/usb/keyboard/keyboardmodifiers/

   Notes: Red LED Indicates Nunchuk Connection Failure (cycle power)

   Runs on Unofficial Test Hardware (9/26/2020)
   Runs on Frag-Ninja (9/26/2020)

 ******* TESTED ON ACTUAL HARDWARE *******/

#include "Keyboard.h"
#include <NintendoExtensionCtrl.h>

#define DEAD_ZONE       5
#define NULL_STICK      128 //center pos
#define _POSITIVE       (NULL_STICK + DEAD_ZONE)
#define _NEGITIVE       (NULL_STICK - DEAD_ZONE)

#define UP              'w' // W
#define LEFT            'a' // A
#define DOWN            's' // S
#define RIGHT           'd' // D
#define Z               ' ' // space
#define C               'f' // F

Nunchuk nchuk;
bool LED = LOW;
bool _state[4] = {false};

void setup() {
  // put your setup code here, to run once:
  digitalWrite(LED_BUILTIN, HIGH);
  nchuk.begin();
  Keyboard.begin();
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

    bool state[4] = {_state[0], _state[1], _state[2], _state[3]};
    state[0] = (joyY >= _POSITIVE) ? true : false; /* UP */
    state[1] = (joyY <= _NEGITIVE) ? true : false; /* DOWN */
    state[2] = (joyX >= _POSITIVE) ? true : false; /* RIGHT */
    state[3] = (joyX <= _NEGITIVE) ? true : false; /* LEFT */

    cButton == true ? Keyboard.press(C) : Keyboard.release(C);
    zButton == true ? Keyboard.press(Z) : Keyboard.release(Z);

    char key_table[4] = {UP, DOWN, RIGHT, LEFT};
    for (int i = 0; i < 4; i++) {
      if (_state[i] != state[i]) {
        state[i] == true ? Keyboard.press(key_table[i]) : Keyboard.release(key_table[i]);
        _state[i] = state[i];
      }
    }
  }

  if (success != LED) {
    LED = success;
    digitalWrite(LED_BUILTIN, !LED);
  }
}
