// Motor A connections  
int enA = 5;
int motor_in1 = 6;
int motor_in2 = 4;

// Motor B connections
int enB = 9;
int motor_in3 = 8;
int motor_in4 = 7;

// defines pins numbers
const int center_trigPin = 30;
const int center_echoPin = 31;

const int left_trigPin = 32;
const int left_echoPin = 33;

const int right_trigPin = 34;
const int right_echoPin = 35;

// defines variables
long duration;
int distance;

void setup() {

  pinMode(center_trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(center_echoPin, INPUT); // Sets the echoPin as an Input
  pinMode(left_trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(left_echoPin, INPUT); // Sets the echoPin as an Input
  pinMode(right_trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(right_echoPin, INPUT); // Sets the echoPin as an Input
  
  Serial.begin(9600); // Starts the serial communication

}

void loop() {
  dist(center_trigPin, center_echoPin);
  delay(1000);
  dist(left_trigPin, left_echoPin);
  delay(1000);
  dist(right_trigPin, right_echoPin);
  delay(1000);
}

void dist(int trigPin, int echoPin){
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



