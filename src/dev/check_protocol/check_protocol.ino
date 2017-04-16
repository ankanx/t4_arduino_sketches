/*
 * Created by Andreas Fransson, 2017-04-15
 */
byte byteArray[6];

// Message size counter
int j=0;    
boolean complete = false;  

void setup() {
  // initialize serial:
  Serial.begin(250000);
}

void loop() {

  if (complete) {
    Serial.print("cmd = speed:");
    Serial.print((int)byteArray[3]);
    Serial.print(" angle: ");
    Serial.println((int)byteArray[4]);
 
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
