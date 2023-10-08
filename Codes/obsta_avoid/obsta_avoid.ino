// Motor A connections  
int enA = 5;
int motor_in1 = 6;
int motor_in2 = 4;

// Motor B connections
int enB = 9;
int motor_in3 = 8;
int motor_in4 = 7;

// defines pins numbers
const int trigPin = 10;
const int echoPin = 11;
// defines variables
long duration;
int distance;

void setup() {

  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  Serial.begin(9600); // Starts the serial communication

	// Set all the motor control pins to outputs
	pinMode(enA, OUTPUT);
	pinMode(enB, OUTPUT);
	pinMode(motor_in1, OUTPUT);
	pinMode(motor_in2, OUTPUT);
	pinMode(motor_in3, OUTPUT);
	pinMode(motor_in4, OUTPUT);
	
	// Turn off motors - Initial state
	digitalWrite(motor_in1, LOW);
	digitalWrite(motor_in2, LOW);
	digitalWrite(motor_in3, LOW);
	digitalWrite(motor_in4, LOW);

  // Set motors to maximum speed
	// For PWM maximum possible values are 0 to 255
	analogWrite(enA, 80);
	analogWrite(enB, 80);

}

void loop() {

  dist();
  if (distance>10){
    digitalWrite(motor_in1, LOW);
    digitalWrite(motor_in2, HIGH);
    digitalWrite(motor_in3, HIGH);
    digitalWrite(motor_in4, LOW);
  }

  if (distance<10){
  
  	// Turn off motors
    digitalWrite(motor_in1, LOW);
    digitalWrite(motor_in2, LOW);
    digitalWrite(motor_in3, LOW);
    digitalWrite(motor_in4, LOW);
    delay(500);

    digitalWrite(motor_in1, HIGH);
	  digitalWrite(motor_in2, LOW);
	  digitalWrite(motor_in3, HIGH);
	  digitalWrite(motor_in4, LOW);
	  delay(500);

    digitalWrite(motor_in1, LOW);
	  digitalWrite(motor_in2, HIGH);
	  digitalWrite(motor_in3, HIGH);
	  digitalWrite(motor_in4, LOW);

  }

}

void dist(){
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2;
  // Prints the distance on the Serial Monitor
  Serial.print("Distance: ");
  Serial.println(distance);  
}



