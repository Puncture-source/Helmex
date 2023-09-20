/*
  Helmex
  peripheral
*/

#include <Arduino_LSM6DS3.h>
#include <ArduinoBLE.h>

// Define custom BLE service for LED (read-only)
BLEService LEDService("11111111-aaaa-bbbb-cccc-0123456789ab");
BLEByteCharacteristic LEDOut("22222222-aaaa-bbbb-cccc-0123456789ab", BLERead);
BLEByteCharacteristic LEDIn("22222222-aaaa-bbbb-cccc-0123456789ac", BLEWrite);


void setup() {
  // Initialize internal LED (for visual debugging)
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  // Initialize Serial connection
  Serial.begin(115200);

  // Initialize IMU
  if (!IMU.begin()) {
    // Failed to initialize IMU, blink the internal LED
    Serial.println("Failed initializing IMU");
    while (1) {
      digitalWrite(LED_BUILTIN, HIGH);
      delay(100);
      digitalWrite(LED_BUILTIN, LOW);
      delay(100);
    }
  }

  // Initialize BLE
  if(!BLE.begin()) {
    // Failed to initialize BLE, blink the internal LED
    Serial.println("Failed initializing BLE");
    while (1) {
      digitalWrite(LED_BUILTIN, HIGH);
      delay(100);
      digitalWrite(LED_BUILTIN, LOW);
      delay(100);
    }

  }

  // Set advertised local name and services UUID
  BLE.setDeviceName("Arduino G7 Peripheral");
  BLE.setLocalName("Nano 33 IOT (Peripheral)");
  
  LEDService.addCharacteristic(LEDOut);
  LEDService.addCharacteristic(LEDIn);
  BLE.addService(LEDService);
  BLE.setAdvertisedService(LEDService);

  // Set default values for characteristics

  // Start advertising
  BLE.advertise();
}

void loop() {
  // Listen for BLE
  BLEDevice central = BLE.central();

  // If a central is connected
  if(central) {
    Serial.print("Connected to central: ");
    Serial.println(central.address());
  }

  //initialise ledValue
  byte ledOutValue = 0;
  
  // While central is still connected...
  while(central.connected()) {

    Serial.print("Connected to central: ");
    Serial.println(central.address());

    if(ledOutValue==1){
      ledOutValue = 0;
    }
    else {
      ledOutValue = 1;
    }

    Serial.print("ledOutValue is ");
    Serial.println(ledOutValue);
    Serial.print("ledInValue is ");
    Serial.println(LEDIn);
    
    LEDOut.writeValue((byte)ledOutValue);
    

    delay(1000);
  }

  // when the central disconnects, print it out:
  Serial.print("Disconnected from central: ");
  Serial.println(central.address());
  delay(1000);
}