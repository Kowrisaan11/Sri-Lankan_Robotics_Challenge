int LeftSensor = 13;

;

void setup() {

  pinMode(LeftSensor, INPUT);
 
  Serial.begin(9600);
}

void loop() {
  int LeftValue = digitalRead(LeftSensor);

  Serial.println(LeftValue);
  


}