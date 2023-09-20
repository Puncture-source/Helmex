/*
  Base Station Code
*/

#include <ArduinoBLE.h>
  
const char* deviceServiceUuid = "11111111-aaaa-bbbb-cccc-0123456789ab";
const char* deviceServiceCharacteristicUuid = "22222222-aaaa-bbbb-cccc-0123456789ab";
const char* deviceServiceCharacteristicUuid2 = "22222222-aaaa-bbbb-cccc-0123456789ac";

byte temp;

//------------[Sensor Smoothing]-----------------
// Define the number of samples to keep track of. The higher the number, the
// more the readings will be smoothed, but the slower the output will respond to
// the input. Using a constant rather than a normal variable lets us use this
// value to determine the size of the readings array.
const int numReadings = 10;

int readings[numReadings];  // the readings from the analog input
int readIndex = 0;          // the index of the current reading
int total = 0;              // the running total
int average = 0;            // the average

//-----------------------------------------------

void setup() {
  Serial.begin(9600);
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
  readings[thisReading] = 0;
  }

  while (!Serial);

  pinMode(LED_BUILTIN, OUTPUT);
  


 
  if (!BLE.begin()) {
    Serial.println("* Starting BluetoothÂ® Low Energy module failed!");
    while (1);
  }

  BLE.setDeviceName("Arduino G7 Central");
  BLE.setLocalName("Nano 33 IOT (Central)"); 
  BLE.advertise();

  Serial.println("Arduino Nano 33 IOT (Central Device)");
  Serial.println(" ");
}

void loop() {
  connectToPeripheral();
}

void connectToPeripheral(){
  BLEDevice peripheral;
  
  Serial.println("- Discovering peripheral device...");
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
  

  do
  {
    BLE.scanForUuid(deviceServiceUuid);
    peripheral = BLE.available();
    Serial.print(".");
    delay(100);
  } while (!peripheral);

  digitalWrite(LED_BUILTIN, LOW);
  
  if (peripheral) {
    
    Serial.println("* Peripheral device found!");
    Serial.print("* Device MAC address: ");
    Serial.println(peripheral.address());
    Serial.print("* Device name: ");
    Serial.println(peripheral.localName());
    Serial.print("* Advertised service UUID: ");
    Serial.println(peripheral.advertisedServiceUuid());
    Serial.println(" ");
    BLE.stopScan();
    controlPeripheral(peripheral);
  }
}

void controlPeripheral(BLEDevice peripheral) {
  Serial.println("- Connecting to peripheral device...");

  if (peripheral.connect()) {
    Serial.println("* Connected to peripheral device!");
    Serial.println(" ");
  } else {
    Serial.println("* Connection to peripheral device failed!");
    Serial.println(" ");
    return;
  }

  Serial.println("- Discovering peripheral device attributes...");
  if (peripheral.discoverAttributes()) {
    Serial.print("* Peripheral device attributes discovered! ");
    //Serial.println(peripheral.discoverAttributes());  //outputs a 1
    Serial.println(" ");
  } else {
    Serial.println("* Peripheral device attributes discovery failed!");
    Serial.println(" ");
    peripheral.disconnect();
    return;
  }

  BLECharacteristic LEDOut = peripheral.characteristic("22222222-aaaa-bbbb-cccc-0123456789ab");
    
  if (!LEDOut) {
    Serial.println("* Peripheral device does not have LEDOut characteristic!");
    peripheral.disconnect();
    return;
  } else if (!LEDOut.canRead()) {
    Serial.println("* Peripheral does not have a writable LEDOut characteristic!");
    peripheral.disconnect();
    return;
  }
  else if (LEDOut) {
    Serial.println("* Peripheral does have LEDOut characteristic");
    Serial.println(" ");
  }

  BLECharacteristic LEDIn = peripheral.characteristic("22222222-aaaa-bbbb-cccc-0123456789ac");
    
  if (!LEDIn) {
    Serial.println("* Peripheral device does not have LEDOut characteristic!");
    peripheral.disconnect();
    return;
  } else if (!LEDIn.canRead()) {
    Serial.println("* Peripheral has a writable LEDIn characteristic!");
  }
  
  while (peripheral.connected()) {


  /*
    if (1 != 1) {  
      Serial.print("* Writing value to gesture_type characteristic: ");
      LEDOut.writeValue((byte)temp);
      Serial.println("* Writing value to gesture_type characteristic done!");
      Serial.println(" ");
    }
  */

  do {
        
    //-------------[Smoothing]----------

     // total = total -readings[readIndex];
     // readings[readIndex] = peripheral.rssi();
     // total = total +readings[readIndex];
     // readIndex = readIndex + 1;
    // if we're at the end of the array...
  if (readIndex >= numReadings) {
    // ...wrap around to the beginning:
    readIndex = 0;
  }
    // calculate the average:
  //  average = total / numReadings;
    // send it to the computer as ASCII digits
   //Serial.println(average);
    total = 0;
    delay(300);
    Serial.println(peripheral.rssi());
  }while(1!=1);

  //delay(10);  // delay in between reads for stability
  //---------------------------------------------------------



  
  }
  
  Serial.println("- Peripheral device disconnected!");
}
