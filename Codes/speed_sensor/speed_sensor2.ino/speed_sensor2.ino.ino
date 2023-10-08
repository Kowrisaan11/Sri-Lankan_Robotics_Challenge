// Motor A connections
int enA = 9;
int motor_in1 = 8;
int motor_in2 = 7;

// Motor B connections
int enB = 5;
int motor_in3 = 6;
int motor_in4 = 4;
 
int encoder_A = 2;
int encoder_B = 3;
 
volatile unsigned int counter_A;
volatile unsigned int counter_B;
int rpm_A;
int rpm_B;
int rpm;

void setup() {
  pinMode(motor_in1, OUTPUT);
  pinMode(motor_in2, OUTPUT);
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);

  digitalWrite(motor_in1, HIGH);
  digitalWrite(motor_in2, LOW);
  digitalWrite(motor_in3, HIGH);
  digitalWrite(motor_in4, LOW);
  analogWrite(enA, 80);
  analogWrite(enB, 80);

  Serial.begin(9600); // Starts the serial communication

  // Setup for speed sensor
  attachInterrupt(0,countpulse_A,RISING);
  attachInterrupt(0,countpulse_B,RISING);
  pinMode(encoder_A, INPUT); 
  pinMode(encoder_B, INPUT); 
  digitalWrite(encoder_A, HIGH);
  digitalWrite(encoder_B, HIGH);
}

void loop() {
  rpm_A = speed(counter_A);
  rpm_B = speed(counter_B);
  Serial.print("Speed: ");
  Serial.print(rpm_A); 
  Serial.println(rpm_B); 
  delay(1);
}

void countpulse_A(){
        counter_A++;
}

void countpulse_B(){
        counter_B++;
}

// calculate speed in rpm
int speed(int counter){
  static uint32_t previousMillis;
  if (millis() - previousMillis >= 1000) {
    rpm = (counter/20)*60;          
    counter = 0;
    previousMillis += 1000;
    return rpm;
  }
}