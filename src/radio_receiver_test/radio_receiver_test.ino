

int channel1 = 2; 
int channel2 = 4;   

int channel2_value;
int channel1_value;


void setup()
{
  Serial.begin(9600);
  pinMode(channel1, INPUT);      
  pinMode(channel2, INPUT);     
}

void loop()
{
  channel1_value = digitalRead(channel1);
  channel2_value = digitalRead(channel2);
  Serial.print("channel 1 val: ");
  Serial.println(channel1_value);
  Serial.print("channel 2 val: ");
  Serial.println(channel2_value);
  /*
   * To make it readeble.
   */
  delay(1000);
}
