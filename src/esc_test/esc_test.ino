#include <Servo.h>

// 90 is ESC Neautral
int Speed = 90;

int input = 0;

Servo ESC;

void setup() {
// ESC attached to pin 9 
ESC.attach(9);
  
// start serial at 115200 baud
Serial.begin(115200);   
ESC.write(Speed); 

}

void loop() {


//  Serial.print("Value :");
  Serial.println(Speed);

 
    
  
ESC.write(Speed);

}


void serialEvent() {
  while (Serial.available()) {
        input = Serial.parseInt();

    if(Serial.read() == '\n'){

    Speed = input;

     if(Speed > 180){
      Speed = 180;
     }else if(Speed < 0){
      Speed = 0;
     }
     }

  }
}
