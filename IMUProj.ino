#include <Adafruit_ISM330DHCX.h>
#include "USB.h"
// #include "USBHIDMouse.h"
#include "USBHIDKeyboard.h"
//https://github.com/espressif/arduino-esp32/blob/master/libraries/USB/src/USBHIDKeyboard.h


#define green_pin 2
#define blue_pin 3

USBHIDKeyboard Keyboard;
Adafruit_ISM330DHCX ism330dhcx;


int green_state = 0;
int blue_state = 0;
float gyro_angle = 0;
double old_time = 0;
double blender_angle = 0;

bool hold_condition = false; //if true then sys will work

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  USB.begin();
  Keyboard.begin();

  pinMode(green_pin, INPUT_PULLDOWN);
  pinMode(blue_pin, INPUT_PULLDOWN);

  if (!ism330dhcx.begin_I2C()) {
    Serial.println("Failed to find ISM330DHCX chip");
    while (1) { //safe crash
      delay(10);
    }
  }
  Serial.println("ISM330DHCX Found!");

  ism330dhcx.configInt1(false, false, true); // accelerometer DRDY on INT1
  ism330dhcx.configInt2(false, true, false); // gyro DRDY on INT2
}

void loop() {
  // put your main code here, to run repeatedly:
  sensors_event_t accel;
  sensors_event_t gyro;
  sensors_event_t temp;
  ism330dhcx.getEvent(&accel, &gyro, &temp);
  
  
  double current_time = millis();
  double dt = (current_time - old_time) / 1000;
  old_time = current_time;

  gyro_angle += (gyro.gyro.z * dt) * (180/PI);
  Serial.print("Gyro Z Angle: ");
  Serial.println(gyro_angle);
  Serial.print(green_state);
  Serial.print("     ");
  Serial.print(blue_state);
  Serial.println();
  if (hold_condition){
    if ((gyro_angle-blender_angle) >= 15){ //AS LONG AS COLLECTION IS CONTINUOUS MEANING HOLD CONDITOIN IS ALWAYS TRUE: mthe gyro won't really ever have an angle change of more than 15˚ because of how frequently it collects data
      ccw_15();
    }
    else if ((gyro_angle-blender_angle)<=-15){
      cw_15();
    }
  }


  green_state = digitalRead(green_pin);
  blue_state = digitalRead(blue_pin);

  //for some reason the var names don't work so i put the hex codes from here https://github.com/espressif/arduino-esp32/blob/master/libraries/USB/src/USBHIDKeyboard.h
  if (green_state == HIGH){ //resets gyro angle
    gyro_angle = 0;
    blender_angle = 0;
  }

  if (blue_state == HIGH){hold_condition = true;}
  else{hold_condition = false;}
  delay(100);
}

void cw_15(){ //move 15˚ cw
    Keyboard.press(KEY_LEFT_SHIFT);
    Keyboard.press(0xE6); //moves right
    Keyboard.releaseAll();
    blender_angle-=15;
}

void ccw_15(){ //move 15˚ ccw
  Keyboard.press(KEY_LEFT_SHIFT);
    Keyboard.press(0xE4); //moves left
    Keyboard.releaseAll();
    blender_angle+=15;
}
