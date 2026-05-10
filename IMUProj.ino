#include "USB.h"
// #include "USBHIDMouse.h"
#include "USBHIDKeyboard.h"
//https://github.com/espressif/arduino-esp32/blob/master/libraries/USB/src/USBHIDKeyboard.h
#define green_pin 2
#define blue_pin 3

USBHIDKeyboard Keyboard;
int green_state = 0;
int blue_state = 0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  USB.begin();
  Keyboard.begin();

  pinMode(green_pin, INPUT);
  pinMode(blue_pin, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  green_state = digitalRead(green_pin);
  blue_state = digitalRead(blue_pin);

  Serial.print("Green Button: ");
  Serial.println(green_state);
  Serial.print("Blue Button: ");
  Serial.println(blue_state);
  Serial.println();


//for some reason the var names don't work so i put the hex codes from here https://github.com/espressif/arduino-esp32/blob/master/libraries/USB/src/USBHIDKeyboard.h
  if (green_state == HIGH){
    Keyboard.press(KEY_LEFT_SHIFT);
    Keyboard.press(0xE4);
    Keyboard.releaseAll();
  }

  if (blue_state == HIGH){
    Keyboard.press(KEY_LEFT_SHIFT);
    Keyboard.press(0xE6);
    Keyboard.releaseAll();
  }
  delay(100);
}
