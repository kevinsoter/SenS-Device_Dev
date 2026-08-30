// __________________________________________________________//
// Arduino code for manual control of vibration frequency    //
// of the SenS-Device using bluetooth of the Ardino          //
// In addition the analog output sends a 3V-Out signal       //
// when the vibration starts, lasting the entire duration    //
// of the vibration. No contact to the PC is needed.         //
// ~80 Hz at 200 decimal, which equals C8 in hexadecimal     //                             
//                                                           //
// Code by Kevin Soter, last updated 14.08.2024              //

#include <Wire.h>
#include <ArduinoBLE.h>

//_Fill in by hand_//

u_int32_t waitTime = 1000;                              // time to wait before vibrating in miliseconds
u_int32_t vibTime = 5000;                               // in miliseconds

//______END_______ // 

const int motor_pin = 2;
const int anOutput = 4;
int vib_var = 0;
u_int32_t timer = 0;
u_int32_t wait_timer = 0;

// BLE LED Switch Characteristic - custom 128-bit UUID, read and writable by central
BLEService ledService("180A");                          // BLE LED Service
BLEByteCharacteristic switchCharacteristic("2A57", BLERead | BLEWrite);

void setup() {

  pinMode(anOutput, OUTPUT);

  // Initialize BLE
  if (!BLE.begin()) {
    while (1);
  }

  BLE.setLocalName("Nano rp 2040");                   // Name found in App when connecting to bluetooth
  BLE.setAdvertisedService(ledService);               // Set service UUID
  ledService.addCharacteristic(switchCharacteristic); // Add the characteristic to the service
  BLE.addService(ledService);                         // Add service
  switchCharacteristic.writeValue(0);                 // Set the initial value for the characteristic:
  BLE.advertise();                                    // Start advertising

}

void loop() {
  
  BLEDevice central = BLE.central();                  // Listen for Bluetooth peripherals to connect:

  if (switchCharacteristic.value() == 1) {
    timer = millis();
    while(millis() - timer <= vibTime) {              // Vibrating for the  duration of vibTime
      analogWrite(motor_pin, 125);                    // If sending "1" to device it starts at 125
      analogWrite(anOutput, 255);                     // Starting vibration sends out 3V signal through analog
    }
    switchCharacteristic.writeValue(0);
  }

  else if (switchCharacteristic.written() && switchCharacteristic.value() != 1 && switchCharacteristic.value() != 0) {
    wait_timer = millis();
    while(millis() - wait_timer <= waitTime) {
      analogWrite(anOutput, 127);
    }
    timer = millis();
    while(millis() - timer <= vibTime) {              // Vibrating for the  duration of vibTime
      analogWrite(motor_pin, switchCharacteristic.value()); // Vibrating at intensity sent vie bluetooth (in hexadecimal)
      analogWrite(anOutput, 255);                     // Starting vibration sends out 3V signal through analog
    }
    switchCharacteristic.writeValue(0);
  }

  else {                                             // No Vibration
    analogWrite(motor_pin, 0);
    analogWrite(anOutput, 85);
  }

}
