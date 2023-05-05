#include <Arduino.h>
#include "LiulinTask.h"

/* Dependancy objects to the LIULIN_t type */
#ifdef USE_SOFTWARE_SERIAL_LIUL_READ
  SoftwareSerial LiulSerialPort; // Make sure to configure the corresponding pins in GPS.h
#else
  HardwareSerial LiulSerialPort(2); // Make sure to configure the corresponding pins in GPS.h ({RX, TX} :: Serial0 -> 03, 01; Serial1 -> 17, 16; Serial2 -> 27, 26)
#endif // USE_SOFTWARE_SERIAL

LIULIN_t Liulin;

void setup() 
{
  Serial.begin(115200);
  InitLiulinTask( &Liulin, 3000, &LiulSerialPort);
}

void loop() {
  // put your main code here, to run repeatedly:
}