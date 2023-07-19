#include <HX711.h>

// Initialize the load cell amplifier
HX711 scale;
const int LOADCELL_DOUT_PIN = 4; //we are using the 4 and 5 pins of the arduino to communicate with the HX711 amplifier
const int LOADCELL_SCK_PIN = 5;
//The DOUT (data output) and SCK (serial clock) pins 
//of the HX711 amplifier are digital input/output pins that are used to communicate with the amplifier and read the load cell output.



// Define weight thresholds to separate objects into different weight categories
const float SMALL_OBJECT_WEIGHT_THRESHOLD = 110.0; // in grams
const float MEDIUM_OBJECT_WEIGHT_THRESHOLD = 350.0; // in grams

// Initialize variables to keep track of object counts
int smallObjectCount = 0;
int mediumObjectCount = 0;
int largeObjectCount = 0;

void setup() {
  // Initialize the load cell and amplifier
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.set_scale(-216.49);  // so we got this value from an initial calibration that we did prior to this demonstration.
  scale.tare();

  // Initialize the serial communication and display
  Serial.begin(57600); //serial communication between a computer and a device, the baud rate determines the speed at which data is transmitted.
  // A higher baud rate means that more data can be transmitted per second, but it also means that there is a higher risk of errors due to signal interference.
}

void loop() {
  static unsigned long lastPrintTime = 0;
  unsigned long currentTime = millis();  //The millis() function in Arduino is a built-in function 
  //that returns the number of milliseconds that have elapsed since the Arduino board started running its current program. 
  //It is a useful function for timing and scheduling tasks in an Arduino program.

  // Only print the output if 10 seconds have elapsed since the last print
  if (currentTime - lastPrintTime >= 5000) {
    // Read the weight from the load cell and convert it to grams
    float weight = scale.get_units(10);
    // Check the weight against the weight thresholds and increment the appropriate count
    if (weight < SMALL_OBJECT_WEIGHT_THRESHOLD &&  weight>10) {
      smallObjectCount++;
    } else if (weight < MEDIUM_OBJECT_WEIGHT_THRESHOLD && weight> SMALL_OBJECT_WEIGHT_THRESHOLD ) {
      mediumObjectCount++;
    } else if (weight > MEDIUM_OBJECT_WEIGHT_THRESHOLD){
      largeObjectCount++;
    }

    // Print the weight and object counts to the serial monitor and display
    Serial.print("Weight: ");
    Serial.print(weight);
    Serial.print(" g | Small object count: ");
    Serial.print(smallObjectCount);
    Serial.print(" | Medium object count: ");
    Serial.print(mediumObjectCount);
    Serial.print(" | Large object count: ");
    Serial.println(largeObjectCount);

    // Update the last print time
    lastPrintTime = currentTime;
  }

  // Wait for a short time before reading the weight again
  delay(500);
}
