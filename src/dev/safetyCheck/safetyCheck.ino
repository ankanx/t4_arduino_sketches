/*

  safetyCheck.ino - Code for motor control on the Term 4 car at SEM.
  Created by Andreas Fransson, Mars 10, 2017.
  Released into the public domain.

Simple control sketch for the Safety Check.

Radio Controller trimming set to: < < V
                                    0 5

Values recieved from the Controller:

Steering values:
Idle: 146-148
MAX: 190-191
MIN; 107-108

Engine values:
Idle: 148-149
MAX:  196-197
MIN:  92-98

*/

/*
 * We wount the ESC and Steering Servo As Servos
 * using the Servo library provided by Arduino
 */
#include <Servo.h>

/*
 * The reviever pinOut for the Radio module.
 * (Expecting pulsein reading)
 */
byte Steering_Radio_PIN = 2;
byte Engine_Radio_PIN = 4; 

/*
 * The control pinOut for the ESC(Engine) and Steering Servo.
 */
#define Servo_Pin 5
#define Engine_Pin 3

/*
 * Storage variables for vales sent by the radio controler.
 */
int Steering_value;
int Engine_value;

/*
 * Variables storing valid comand ranges.
 * This is used to avoid stuttering, and dead comands when we turn the controller off.
 */
#define Idle 148
#define Max 200
#define Min 90

/*
 * Declare the two objects we will be using for steering and engine control.
 */
Servo ESC;
Servo steering;
 
void setup() {
  /*
   * Attach the right pins to our objects. 
   */
  ESC.attach(Engine_Pin);
  steering.attach(Servo_Pin);
  
  /*
   * Setup our reciever pin's as INPUT so we can start reading their values.
   */
  pinMode(Steering_Radio_PIN, INPUT);
  pinMode(Engine_Radio_PIN, INPUT);

  /*
   * Make use of a debugging serial.
   */
  Serial.begin(115200);
}


 /*
  * Main
  */
void loop() {

  /*
   * Read incoming values from the controller. (Divided by 10 for ease)
   * This reads lenght of pulses set to High.
   */
  Steering_value = pulseIn(Steering_Radio_PIN, HIGH)/10;
  Engine_value = pulseIn(Engine_Radio_PIN, HIGH)/10;

  /*
   * Setup a valid range to avoid stuttering and keep car idle.
   */
  if(Steering_value >= 135 && Steering_value < 150 ){
    Steering_value = Idle;
  }

  /*
   * Setup check for "dead" command confirming that the comand is in the valid range
   * else we set car to to idle.
   */
  if(Steering_value > Max || Steering_value < Min ){
    Steering_value = Idle;
  }

  /*
   * Setup check for "dead" command confirming that the comand is in the valid range
   * else we set car to to idle.
   */
  if(Engine_value > Max || Engine_value < Min ){
    Engine_value = Idle;
  }

  /*
   * Debug print incoming comand values.
   */
  /*
  Serial.print("Steering");
  Serial.println(Steering_value);
  Serial.print("Engine: ");
  Serial.println(Engine_value);
  */
  
  /*
   * For control we set a fixed value to the car speeds.
   * This to avoid the car from going to fast(Recomended by TA's)
   */

  /*
   * ESC (Enngine)
   */
  if(Steering_value > 160){
    steering.write(115);
  }else if(Steering_value < 140){
    steering.write(70);
  }else{
  steering.write(90);
  }

  /*
   * Steering Servo
   */
  if(Engine_value > 160){
    ESC.write(98);
  }else if(Engine_value < 140){
    ESC.write(70);
  }else{
  ESC.write(90);
  }
}


