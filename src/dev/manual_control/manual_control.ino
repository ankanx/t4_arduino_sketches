/*
 * Manual control 
 * 
 * Commands:
 * D - drive
 * S - steer
 * 
 * ex:
 *    DXX
 *    SXX
 * 
 *    D12 -> sets engine power to 12
 *    D-12 -> sets engine power to reverse 12
 *    S30 -> sets steering to 30 degrees right
 *    S-30 -> sets steering to 30 degrees left
 * 
 */

#include <MotorController.h>
byte byteArray[4];
boolean complete;
unsigned long currentTime;
unsigned long lastCommand = 0;
unsigned long delayTime = 0;
int value;

MotorController testMotor(9,10);
protocolParse parser;

void setup() {
  complete = false;

  Serial.begin(115200);
  testMotor.start();

}

void loop() {
  
  currentTime = millis();
  if(!complete && (currentTime - lastCommand) > 5000){

    // Dont flood the Serial
    if((currentTime - delayTime) > 1000){
      Serial.println("No comand in 5 seconds, Forcing IDLE");
      delayTime = millis();
    }
    
    testMotor.setAngle(0);
    testMotor.setSpeed(0);
  }

  if(complete){
    
    if(byteArray[0] == 0x44){
      if(byteArray[1] == 0x2D){
        Serial.println("Got Engine CMD");
        value = -((byteArray[2] - '0')*10 + byteArray[3] - '0');
        Serial.print("Setting Speed value to: ");
        Serial.println(value);
          
        testMotor.setSpeed(value);
      }else if(byteArray[1] == 0x30){
        Serial.println("Got Engine CMD");
        value = 0;
        Serial.print("Setting Speed value to: ");
        Serial.println(value);
           testMotor.setAngle(value);
      }else{
        Serial.println("Got Engine CMD");
        value = (byteArray[1] - '0')*10 + byteArray[2] - '0';
        Serial.print("Setting Speed value to: ");
        Serial.println(value);
          
          testMotor.setSpeed(value);
      }
      
    }

    if(byteArray[0] == 0x53){
      if(byteArray[1] == 0x2D){
        Serial.println("Got Steering CMD");
        value = -((byteArray[2] - '0')*10 + byteArray[3] - '0');
        Serial.print("Setting Steering value to: ");
        Serial.println(value);
           testMotor.setAngle(value);
    
      }else if(byteArray[1] == 0x30){
        Serial.println("Got Steering CMD");
        value = 0;
        Serial.print("Setting Steering value to: ");
        Serial.println(value);
           testMotor.setAngle(value);
      
      }else{
        Serial.println("Got Steering CMD");
        value = (byteArray[1] - '0')*10 + byteArray[2] - '0';
        Serial.print("Setting Steering value to: ");
        Serial.println(value);
           testMotor.setAngle(value);
    
      }
    }
    
    
    lastCommand = millis();
    for(int i = 0; i < 4; i++){
    byteArray[i] = 0;
    }
    complete = false;
  }
  
}

void serialEvent(){
  while (Serial.available()) {

    Serial.readBytes(byteArray, 4);
   
    complete = true;   
  }

}
