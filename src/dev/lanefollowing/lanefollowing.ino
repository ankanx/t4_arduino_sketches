/*
 * Created by Andreas Fransson, 2017-04-15
 * 
 * Lanefollowing motor control not utilizing sensors
 */

#include <Adafruit_NeoPixel.h>
byte byteArray[6];

// Message size counter
int j=0;    
boolean complete = false;  

void setup() {
  // initialize serial at a baudrate of 250000:
  Serial.begin(250000);
}

void loop() {

  if (complete) {
    Serial.print("cmd = speed:");
    Serial.print((int)byteArray[3]);
    Serial.print(" angle: ");
    Serial.println((int)byteArray[4]);

    if((int)byteArray[3] == 0){
      breakIndicate();
    }

    if((int)byteArray[3] > 0){
      driveIndicate();
    }
 
    complete = false;
    j=0;
    
    for(int i = 0; i < 5; i++){
      byteArray[i] = 0;
    }
  }
}


void serialEvent() {
  while (Serial.available()) {
    byte start;
    byte command;
    
    byte inByte = Serial.read();
  
    byteArray[j] = inByte;
        
    // Increment in wait for new byte
    j++;
    
    if(inByte == 0x85){
     start = 0x85;
    }
    if(inByte == 0x77){
     command = 0x77;
    }
    
    if (start == 0x85 && command == 0x77 && inByte == 0x60) {
      complete = true;
    }
  }
}

/*
 * Sets pixels to indicate driving
 */
void driveIndicate(){
  for (int i = 0; i < 2; i++) {
    strip.setPixelColor(i, strip.Color(20, 20, 100)); // blue/white
    strip.setPixelColor(i + 6, strip.Color(20, 20, 100)); // blue/white
    strip.setPixelColor((i + 8), strip.Color(100, 0, 0)); // red
    strip.setPixelColor((i + 8) + 6, strip.Color(100, 0, 0)); // red
  }
  strip.show();
}
/*
 * Sets pixels to indicate breaking
 */
void breakIndicate(){
  for (int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, strip.Color(100, 0, 0)); // red
  }
  strip.show();
}

/*
 * Sets pixels to all blue for when rc-controller is on
 */
void rcControllIndicate(){
 for (int i = 0; i < strip.numPixels(); i++) {
   strip.setPixelColor(i, strip.Color(0, 0, 100)); // blue
 }
 strip.show();
}
/*
 * Sets pixels to indicate reverseing
 */
void reverseIndicate(){
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
 * Sets pixels to indicate turning left
 */
void leftTurnIndicate(){
  for (int i = 0; i < 2; i++) {
    strip.setPixelColor(i, strip.Color(20, 20, 100)); // blue/white
    strip.setPixelColor((i + 14), strip.Color(100, 0, 0)); // red
    strip.setPixelColor((i + 6), strip.Color(100, 100, 0)); // yellow
    strip.setPixelColor((i + 8), strip.Color(100, 100, 0)); // yellow
  }
  strip.show();
}
/*
 * Sets pixels to indicate turning right
 */
void rightTurnIndicate(){
  for (int i = 0; i < 2; i++) {
    strip.setPixelColor(i, strip.Color(100, 100, 0)); // yellow
    strip.setPixelColor((i + 14), strip.Color(100, 100, 0)); // yellow
    strip.setPixelColor((i + 6), strip.Color(20, 20, 100)); // white/blue
    strip.setPixelColor((i + 8), strip.Color(100, 0, 0)); // red
  }
  strip.show();
}

/*
 * Turn off the LED strips
 */
void ledsOff(){
  for (int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, strip.Color(0, 0, 0)); // blue
  }
  strip.show();
}
