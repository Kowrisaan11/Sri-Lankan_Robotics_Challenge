int enA = 9;
int motor_in1 = 8;
int motor_in2 = 7;
 
int encoder = 2;
 
volatile unsigned int counter;
int rpm;
 
void setup() {
  pinMode(motor_in1, OUTPUT);
  pinMode(motor_in2, OUTPUT);
  pinMode(enA, OUTPUT);

  digitalWrite(motor_in1, HIGH);
  digitalWrite(motor_in2, LOW);
  analogWrite(enA, 80);

  Serial.begin(9600); // Starts the serial communication

  // Setup for speed sensor
  attachInterrupt(0,countpulse,RISING);
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