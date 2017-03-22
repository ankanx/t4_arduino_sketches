#include <Servo.h>

// 90 is Neautral angle.
int angle = 90;

int input = 0;

Servo steering;

void setup() {
// Servo attached to pin 9
steering.attach(5);
  
// start serial at 115200 baud
Serial.begin(9600);   
steering.write(angle); 

}

void loop() {


  Serial.print("Value :");
  Serial.println(angle);

 
    
  
steering.write(angle);

}


void serialEvent() {
  while (Serial.available()) {
        input = Serial.parseInt();

    if(Serial.read() == '\n'){

    angle = input;

     if(angle > 135){
      angle = 135;
     }else if(angle < 60){
      angle = 60;
     }
     }

  }
}

