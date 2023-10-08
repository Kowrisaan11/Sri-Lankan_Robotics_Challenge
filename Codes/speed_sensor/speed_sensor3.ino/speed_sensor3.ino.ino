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

void setup() {
  pinMode(motor_in1, OUTPUT);
  pinMode(motor_in2, OUTPUT);
  pinMode(motor_in3, OUTPUT);
  pinMode(motor_in4, OUTPUT);
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);

  digitalWrite(motor_in1, LOW);
  digitalWrite(motor_in2, HIGH);
  digitalWrite(motor_in3, HIGH);
  digitalWrite(motor_in4, LOW);
  analogWrite(enA, 80);
  analogWrite(enB, 80);


  Serial.begin(9600); // Starts the serial communication

  // Setup for speed sensor
  attachInterrupt(digitalPinToInterrupt(encoder_A),countpulse_A,RISING);
  attachInterrupt(digitalPinToInterrupt(encoder_B),countpulse_B,RISING);
  pinMode(encoder_A, INPUT); 
  pinMode(encoder_B, INPUT); 
  //digitalWrite(encoder_A, HIGH);
  //digitalWrite(encoder_B, HIGH);
}


void loop() { 
  speed_A();
  speed_B();
  Serial.print("Speed: ");
  Serial.print(rpm_A); 
  Serial.print("Speed: ");  
  Serial.println(rpm_B); 
  delay(1);
}

void countpulse_A(){
        counter_A++;
}

void countpulse_B(){
        counter_B++;
}

void speed_A(){
  static uint32_t previousMillis_A;
  if (millis() - previousMillis_A >= 1000) {
    rpm_A = (counter_A/20)*60;          
    counter_A = 0;
    previousMillis_A += 1000;
  }
}

void speed_B(){
  static uint32_t previousMillis_B;
  if (millis() - previousMillis_B >= 1000) {
    rpm_B = (counter_B/20)*60;          
    counter_B = 0;
    previousMillis_B += 1000;
  }
}