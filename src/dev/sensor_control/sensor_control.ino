// --------- //
// Libraries //
// --------- //
#include <Servo.h>      // Steering and motor
#include <Wire.h>       // Sonars
#include <Adafruit_NeoPixel.h> // Ledstrips




//SONARS  -- I2C
//Change these to Constants instead as it's preferred on the arduino
#define FC_08_ADDRESS (0xE6 >> 1) // Front Center Sonar
#define FR_08_ADDRESS (0xE0 >> 1) // Front Right Sonar
#define GAIN_REGISTER 0x0B        // Analog Gain
#define LOCATION_REGISTER 0x18    // 1 meter

//INFRAREDS  -- Analog
const int irFrontRightPin = 0;     // pin to which the front right infrared sensor is attached
const int irRearRightPin = 1;      // pin to which the rear right infrared sensor is attached
const int irRearCenterPin  = 2;    // pin to which the rear infrared sensor is attached

// Ledstrips
const int ledPin = 6;

// ----------------------- //
// Instatiation of objects //
// ----------------------- //
Adafruit_NeoPixel strip = Adafruit_NeoPixel(16, ledPin, NEO_GRB + NEO_KHZ800);

const int fifoSize = 3;             // Decides the size of the following arrays
int frCSArray[fifoSize] = {0};      // Front Center US
int frRSArray[fifoSize] = {0};      // Front Right US
int iRFRArray[fifoSize] = {0};      // Right Front IR
int iRRRArray[fifoSize] = {0};      // Right Rear IR
int iRRCArray[fifoSize] = {0};      // Rear Right Center IR


void setup() {

  Serial.begin(57600);

  strip.begin();
  strip.show();
  Wire.begin();
  // Setting up the sonars and limiting the range to 1 meter.

  
}

void loop() {
 
Serial.println(getIRData());
ledsBacking();
delay(200);
ledsRightTurn();
delay(200);

}

/*
 * Returns both US sensors value as a string.
 * " USF 'value' USR 'value'"
 */
String getUSData() {
  String USF = "USF ";
  USF.concat(usCalc(FC_08_ADDRESS));
  //USF.concat(fifo(frCSArray, usCalc(FC_08_ADDRESS))); // smooth values
  String USR = " USR ";
  USR.concat(usCalc(FR_08_ADDRESS));
  //USR.concat(fifo(frRSArray, usCalc(FR_08_ADDRESS))); // smooth values

  return USF + USR;
}
/*
 * Returns all 3 IR sensors value as a string.
 * " IRFR 'value' IRRR 'value' IRRC 'value'"
 */
String getIRData() {
  String IRFR = " IRFR ";
  IRFR.concat(irCalc(irFrontRightPin));
  //IRFR.concat(fifo(iRFRArray, irCalc(irFrontRightPin)));  // smooth values
  String IRRR = " IRRR ";
  IRRR.concat(irCalc(irRearRightPin));
  //IRRR.concat(fifo(iRRRArray, irCalc(irRearRightPin))); // smooth values
  String IRRC = " IRRC ";
  IRRC.concat(irCalc(irRearCenterPin));
  //IRRC.concat(fifo(iRRCArray, irCalc(irRearCenterPin)));  // smooth values

  return IRFR + IRRR + IRRC;
}

/*
 * Calculates the distance an IR sensor is reporting. Returns the value as
 * centimeters. Returns 0 if the value is outside 5-25.
 * Takes an analog pin as input.
 */
int irCalc(int pin) {
  float volt = analogRead(pin);
  int cm = ((2914 / (volt + 5 )) - 1); // gives the range in centimeters
  if (cm >= 5 && cm <= 25) {
    return cm;
  }
  return -1; // if the value is not in our range
}
/*
 * Calculates the distance a US sensor is reporting. Returns the value as
 * centimeters. Returns 0 if the value is outside 5-90.
 * Takes an int corresponding to a US sensor as input.
 */
int usCalc(int sensorAddress) {
  int range;
  Wire.beginTransmission(sensorAddress);
  Wire.write(0x00);                      // First adress, used for range
  Wire.write(0x51);                      // Send 0x51 to start a ranging in cm
  Wire.endTransmission();
  rangingNotDone(sensorAddress);
  //delay(100);

  Wire.beginTransmission(sensorAddress);    // start communicating with SRFmodule
  Wire.write(0x02);              // Call the register for start of ranging data
  Wire.endTransmission();

  Wire.requestFrom(sensorAddress, 2);           // Request 2 bytes from SRF module
  if (Wire.available() < 2){      // If there is not enough data return -1
    return -1;
  }
  byte highByte = Wire.read();                 // Get high byte
  byte lowByte = Wire.read();                  // Get low byte
  range = (highByte << 8) + lowByte;           // Put them together
  if (range >= 5 && range <= 90) {
    return range;
  }
  return -1; // if the value is not in our range
}
int rangingNotDone(int sensorAddress){
  int done = -1;
  Wire.beginTransmission(sensorAddress);    // Begin communication with the SRF module
  Wire.write(0x00);                // Sends the command bit, when this bit is read it returns the software revision
  Wire.endTransmission();
  while (done == -1){
    Wire.requestFrom(sensorAddress, 1); // Request 1 byte
    while (Wire.available() < 0); // While byte available
    done = Wire.read(); // Get byte
    delay(1);
  }
  return 1;
}
/*
 * Takes an array of integers as input and a new integer value
 * the new int value will be added and the oldest value of the
 * array will be removed. Oldest value is at the top.
 */
int fifo(int array[], int newValue) {
  int sum = 0;
  int divideby = 0;
  for (int i = 0; i < fifoSize - 1; i++) { // mvoe the values around in the queue
    array[i] = array[i + 1];
    if (array[i + 1] > 0){ // smoothing is only done with real ranges
      sum += array[i + 1];
      divideby++;
    }
  }
  array[fifoSize - 1] = newValue;
  if (newValue > 0){
    sum += newValue;
    divideby++;
  }
  if (divideby < 2){ // less than 3 values is treated as ghost values
    return -1;
  }
  return sum / divideby;
}


/*
 * Sets pixels to imitate lights when driving
 */
void ledsDriving(){
  for (int i = 0; i < 2; i++) {
    strip.setPixelColor(i, strip.Color(20, 20, 100)); // blue/white
    strip.setPixelColor(i + 6, strip.Color(20, 20, 100)); // blue/white
    strip.setPixelColor((i + 8), strip.Color(100, 0, 0)); // red
    strip.setPixelColor((i + 8) + 6, strip.Color(100, 0, 0)); // red
  }
  strip.show();
}
/*
 * Sets pixels to imitate lights when breaking
 */
void ledsFullStop(){
  for (int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, strip.Color(100, 0, 0)); // red
  }
  strip.show();
}
/*
 * Sets pixels to all blue for when rc-controller is on
 */
void ledsRCController(){
 for (int i = 0; i < strip.numPixels(); i++) {
   strip.setPixelColor(i, strip.Color(0, 0, 100)); // blue
 }
 strip.show();
}
/*
 * Sets pixels to imitate lights when backing
 */
void ledsBacking(){
  for (int i = 0; i < 2; i++) {
    strip.setPixelColor(i, strip.Color(20, 20, 100)); // blue/white
    strip.setPixelColor(i + 6, strip.Color(20, 20, 100)); // blue/white
    strip.setPixelColor((i + 8), strip.Color(100, 0, 0)); // red
    strip.setPixelColor((i + 8) + 6, strip.Color(100, 0, 0)); // red
  }
  strip.setPixelColor(2, strip.Color(100, 100, 100)); // white
  strip.setPixelColor(5, strip.Color(100, 100, 100)); // white
  strip.setPixelColor(10, strip.Color(100, 100, 100)); // white
  strip.setPixelColor(13, strip.Color(100, 100, 100)); // white

  strip.show();
}
/*
 * Sets pixels to imitate light when turning left
 */
void ledsLeftTurn(){
  for (int i = 0; i < 2; i++) {
    strip.setPixelColor(i, strip.Color(20, 20, 100)); // blue/white
    strip.setPixelColor((i + 14), strip.Color(100, 0, 0)); // red
    strip.setPixelColor((i + 6), strip.Color(100, 100, 0)); // yellow
    strip.setPixelColor((i + 8), strip.Color(100, 100, 0)); // yellow
  }
  strip.show();
}
/*
 * Sets pixels to imitate light when turning right
 */
void ledsRightTurn(){
  for (int i = 0; i < 2; i++) {
    strip.setPixelColor(i, strip.Color(100, 100, 0)); // yellow
    strip.setPixelColor((i + 14), strip.Color(100, 100, 0)); // yellow
    strip.setPixelColor((i + 6), strip.Color(20, 20, 100)); // white/blue
    strip.setPixelColor((i + 8), strip.Color(100, 0, 0)); // red
  }
  strip.show();
}
/*
 * Sets all pixels to off
 */
void ledsOff(){
for (int i = 0; i < strip.numPixels(); i++) {
  strip.setPixelColor(i, strip.Color(0, 0, 0)); // blue
}
strip.show();
}
