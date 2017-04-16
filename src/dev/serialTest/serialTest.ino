char buffer[60];
String minGstring;
boolean gotmsg = false;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(250000);
}

void loop() {

if(gotmsg){
  Serial.println((unsigned char) buffer);
  //Serial.println(minGstring);
gotmsg = false;
}


}

void serialEvent() {
  while (Serial.available()) {
    
    //minGstring = Serial.readString();
    //Serial.println(minGstring);
    Serial.readBytes(buffer,60);
    gotmsg = true;
    }
    
  }

