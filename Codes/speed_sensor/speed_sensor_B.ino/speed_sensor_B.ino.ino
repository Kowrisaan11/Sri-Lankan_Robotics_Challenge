int enB = 5;
int motor_in3 = 6;
int motor_in4 = 4;
 
int encoder = 2;
 
volatile unsigned int counter;
int rpm;
 
void setup() {
  pinMode(motor_in3, OUTPUT);
  pinMode(motor_in4, OUTPUT);
  pinMode(enB, OUTPUT);

  digitalWrite(motor_in3, HIGH);
  digitalWrite(motor_in4, LOW);
  analogWrite(enB, 80);

  Serial.begin(9600); // Starts the serial communication

  // Setup for speed sensor
  attachInterrupt(digitalPinToInterrupt(encoder),countpulse,RISING);
  pinMode(encoder, INPUT); 
  digitalWrite(encoder, HIGH);
}

void loop() {
  speed();
  Serial.print("Speed: ");
  Serial.println(rpm); 
  delay(1);
}

void countpulse(){
        counter++;
}

// calculate speed in rpm
void speed(){
  static uint32_t previousMillis;
  if (millis() - previousMillis >= 1000) {
    rpm = (counter/20)*60;          
    counter = 0;
    previousMillis += 1000;
  }
}