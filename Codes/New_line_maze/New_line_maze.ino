// Motor A connections
int enA = 5;
int motor_in1 = 6;
int motor_in2 = 4;

// Motor B connections
int enB = 9;
int motor_in3 = 8;
int motor_in4 = 7;

# define STOPPED 0
# define FOLLOWING_LINE 1
# define NO_LINE 2
# define CONT_LINE 3
# define POS_LINE 4
# define RIGHT_TURN 5
# define LEFT_TURN 6

int farLeftSensor = 0;
int farRightSensor = 0;

int mode = 0;
const int left_initial_motor_speed = 85;
const int right_initial_motor_speed = 80;
const int adj = 0;
float adjTurn = 8;
int extraInch = 200;
int adjGoAndTurn = 800;

// PID Constants
float Kp = 25;
float Ki = 0;
float Kd = 15;

float error=0, P=0, I=0, D=0, PID_value=0;
float previousError=0, previousI=0;

#define RIGHT 1
#define LEFT -1

const int farLeftSensorPin = 13;
const int lineFollowSensor0 = 12; 
const int lineFollowSensor1 = 14; 
const int lineFollowSensor2 = 11; 
const int farRightSensorPin = 10; 

int LFSensor[3]={0, 0, 0};

//-------------------------------------------------
//Specific Maze Phase 2 (optimization) definitions and variables 

unsigned char dir; 

// The path variable will store the path that the robot has taken:
//  'L' for left
//  'R' for right
//  'S' for straight (going straight through an intersection)
//  'B' for back (U-turn)

char path[100] = "";
unsigned char pathLength = 0; // the length of the path
int pathIndex = 0;
unsigned int status = 0; // solving = 0; reach end = 1

void setup() {
  pinMode(lineFollowSensor0, INPUT);
  pinMode(lineFollowSensor1, INPUT);
  pinMode(lineFollowSensor2, INPUT);
  pinMode(farLeftSensorPin, INPUT);
  pinMode(farRightSensorPin, INPUT);

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
	analogWrite(enA, 90);
	analogWrite(enB, 80);

  Serial.begin(9600);
}

void loop()
{
  readLFSsensors();
}

//completed
void readLFSsensors()
{
  LFSensor[0] = digitalRead(lineFollowSensor0);
  LFSensor[1] = digitalRead(lineFollowSensor1);
  LFSensor[2] = digitalRead(lineFollowSensor2);
  
  farRightSensor = digitalRead(farRightSensorPin);
  farLeftSensor = digitalRead(farLeftSensorPin);
  
  if     ((farRightSensor == 1 )&&(LFSensor[0]== 1 )&&(LFSensor[1]== 1 )&&(LFSensor[2]== 1 )&&(farLeftSensor == 1 ))  {mode = CONT_LINE; error = 0;}
  else if((farRightSensor == 0 )&&(LFSensor[0]== 0 )&&(LFSensor[1]== 0 )&&(LFSensor[2]== 0 )&&(farLeftSensor == 0 ))  {mode = NO_LINE; error = 0;}

  else if((farRightSensor == 0 )&&(LFSensor[0]== 0 )&&(LFSensor[1]== 1 )&&(LFSensor[2]== 1 )&&(farLeftSensor == 1 ))  {mode = RIGHT_TURN; error = 1;}
  else if((farRightSensor == 0 )&&(LFSensor[0]== 1 )&&(LFSensor[1]== 1 )&&(LFSensor[2]== 1 )&&(farLeftSensor == 1 ))  {mode = RIGHT_TURN; error = 0;}
  else if((farRightSensor == 1 )&&(LFSensor[0]== 1 )&&(LFSensor[1]== 1 )&&(LFSensor[2]== 1 )&&(farLeftSensor == 0 ))  {mode = LEFT_TURN; error = 0;}
  else if((farRightSensor == 1 )&&(LFSensor[0]== 1 )&&(LFSensor[1]== 1 )&&(LFSensor[2]== 0 )&&(farLeftSensor == 0 ))  {mode = LEFT_TURN; error = -1;}

  else if((LFSensor[0]== 0 )&&(LFSensor[1]== 0 )&&(LFSensor[2]== 1 ))  {mode = FOLLOWING_LINE; error = 2;}
  else if((LFSensor[0]== 0 )&&(LFSensor[1]== 1 )&&(LFSensor[2]== 1 ))  {mode = FOLLOWING_LINE; error = 1;}
  else if((LFSensor[0]== 1 )&&(LFSensor[1]== 1 )&&(LFSensor[2]== 1 ))  {mode = FOLLOWING_LINE; error = 0;}
  else if((LFSensor[0]== 1 )&&(LFSensor[1]== 1 )&&(LFSensor[2]== 0 ))  {mode = FOLLOWING_LINE; error = -1;}
  else if((LFSensor[0]== 1 )&&(LFSensor[1]== 0 )&&(LFSensor[2]== 0 ))  {mode = FOLLOWING_LINE; error = -2;}

  /*
  Serial.print (farLeftSensor);
  Serial.print (" ");
  Serial.print (LFSensor[0]);
  Serial.print (" ");
  Serial.print (LFSensor[1]);
  Serial.print (" ");
  Serial.print (LFSensor[2]);
  Serial.print (" ");
  Serial.print (farRightSensor);
  Serial.print (" ");
  Serial.print ("  mode: ");
  Serial.print (mode);
  Serial.print ("  error:");
  Serial.println (error);
  */  
  
}

//---------------------------------------------------
void motorPIDcontrol()
{
  // Calculating the effective motor speed:
  int left_motor_speed = left_initial_motor_speed - PID_value;
  int right_motor_speed = left_initial_motor_speed + PID_value;

  // The motor speed should not exceed the max PWM value
  left_motor_speed = constrain(left_motor_speed, 0, 255);
  right_motor_speed = constrain(right_motor_speed, 0, 255);

  /*Serial.print(PID_value);
    Serial.print("\t");
    Serial.print(left_motor_speed);
    Serial.print("\t");
    Serial.println(right_motor_speed);*/

  analogWrite(enA, left_motor_speed); //Left Motor Speed
  analogWrite(enB, right_motor_speed - 30); //Right Motor Speed

  //following lines of code are to make the bot move forward
  motorForward();
}

//--------------------------------------------------------
void calculatePID()
{
  P = error;
  I = I + error;
  D = error-previousError;
  PID_value = (Kp*P) + (Ki*I) + (Kd*D);
  previousError = error;
}

void mazeSolve(void)
{
    while (!status)
    {
        readLFSsensors();  
        switch (mode)
        {   
          case NO_LINE:  
            motorStop();
            goAndTurn (LEFT, 180);
            recIntersection('B');
            break;
          
          case CONT_LINE: 
            runExtraInch();
            readLFSsensors();
            if (mode != CONT_LINE) {goAndTurn (LEFT, 90); recIntersection('L');} // or it is a "T" or "Cross"). In both cases, goes to LEFT
            else mazeEnd(); 
            break;
            
          case RIGHT_TURN: 
            runExtraInch();
            readLFSsensors();
            if (mode == NO_LINE) {goAndTurn (RIGHT, 90); recIntersection('R');}
            else recIntersection('S');
            break;   
            
          case LEFT_TURN: 
            goAndTurn (LEFT, 90); 
            recIntersection('L');
            break;   
         
          case FOLLOWING_LINE: 
            followingLine();
            break;      
        
         }
    }
}

//---------------------------------------------
void recIntersection(char direction)
{
  path[pathLength] = direction; // Store the intersection in the path variable.
  pathLength ++;
  simplifyPath(); // Simplify the learned path.
}

//----------------------------------------------
void mazeEnd(void)
{
  motorStop();
  Serial.print("  pathLenght ==> ");
  Serial.println(pathLength);
  status = 1;
  mode = STOPPED;
}

//----------------------------------------------
void followingLine(void)
{
   readLFSsensors(); 
   calculatePID();
   motorPIDcontrol();   
}

//------------------------------------------------------------------------------------------
// Path simplification.  The strategy is that whenever we encounter a
// sequence xBx, we can simplify it by cutting out the dead end.  For
// example, LBL -> S, because a single S bypasses the dead end
// represented by LBL.
void simplifyPath()
{
  // only simplify the path if the second-to-last turn was a 'B'
  if(pathLength < 3 || path[pathLength-2] != 'B')
    return;

  int totalAngle = 0;
  int i;
  for(i=1;i<=3;i++)
  {
    switch(path[pathLength-i])
    {
      case 'R':
        totalAngle += 90;
	    break;
      case 'L':
	      totalAngle += 270;
	    break;
      case 'B':
	      totalAngle += 180;
	    break;
    }
  }

  // Get the angle as a number between 0 and 360 degrees.
  totalAngle = totalAngle % 360;

  // Replace all of those turns with a single one.
  switch(totalAngle)
  {
    case 0:
	path[pathLength - 3] = 'S';
	break;
    case 90:
	path[pathLength - 3] = 'R';
	break;
    case 180:
	path[pathLength - 3] = 'B';
	break;
    case 270:
	path[pathLength - 3] = 'L';
	break;
  }

  // The path is now two steps shorter.
  pathLength -= 2;
  
} 

//----------------------------------------------------------------------------------------------
void mazeOptimization (void)
{
  while (!status)
  {
    readLFSsensors();  
    switch (mode)
    {
      case FOLLOWING_LINE:
        followingLine();
        break;    
      case CONT_LINE:
        if (pathIndex >= pathLength) mazeEnd (); 
        else {mazeTurn (path[pathIndex]); pathIndex++;}
        break;  
      case LEFT_TURN:
        if (pathIndex >= pathLength) mazeEnd (); 
        else {mazeTurn (path[pathIndex]); pathIndex++;}
        break;  
      case RIGHT_TURN:
        if (pathIndex >= pathLength) mazeEnd (); 
        else {mazeTurn (path[pathIndex]); pathIndex++;}
        break;   
    }    
   }  
}

//-----------------------------------------------------
void mazeTurn (char dir) 
{
  switch(dir)
  {
    case 'L': // Turn Left
       goAndTurn (LEFT, 90);      
       break;   
    
    case 'R': // Turn Right
       goAndTurn (RIGHT, 90);     
       break;   
       
    case 'B': // Turn Back
       goAndTurn (RIGHT, 800);     
       break;   
       
    case 'S': // Go Straight
       runExtraInch(); 
       break;
  }
}

//motor functions
//---------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------

//motor stop
void motorStop()
{
  digitalWrite(motor_in1, LOW);
  digitalWrite(motor_in2, LOW);
  digitalWrite(motor_in3, LOW);
  digitalWrite(motor_in4, LOW);
  
}

//move forward
void motorForward()
{
  digitalWrite(motor_in1, LOW);
  digitalWrite(motor_in2, HIGH);
  digitalWrite(motor_in3, LOW);
  digitalWrite(motor_in4, HIGH);
}

//move backward
void motorBackward()
{
  digitalWrite(motor_in1, HIGH);
	digitalWrite(motor_in2, LOW);
	digitalWrite(motor_in3, HIGH);
	digitalWrite(motor_in4, LOW);
}

//---------------------------------------------
void motorFwTime (unsigned int time)
{
  motorForward();
  delay (time);
  motorStop();
}

//---------------------------------------------
void motorBwTime (unsigned int time)
{
  motorBackward();
  delay (time);
  motorStop();
}

//------------------------------------------------
void motorTurn(int direction, int degrees)
{
  if (direction == LEFT && degrees == 90){
    sharpLeftTurn();
    delay(500);    
  }  
  else if (direction == RIGHT && degrees == 90){
    sharpRightTurn();
    delay(500);         
  } 
  else if (direction == LEFT && degrees == 180){
    sharpLeftTurn();
    delay(500); 
  }
  else if (direction == RIGHT && degrees == 180){
    sharpRightTurn();
    delay(500); 
  } 
  
  motorStop();
}

//---------------------------------------------------
void runExtraInch(void)
{
  motorPIDcontrol();
  delay(extraInch);
  motorStop();
}

//---------------------------------------------------
void goAndTurn(int direction, int degrees)
{
  motorPIDcontrol();
  delay(adjGoAndTurn);
  motorTurn(direction, degrees);
}

void right()
{
  analogWrite(enA, 80 ); //Left Motor Speed
  analogWrite(enB, 0 ); //Right Motor Speed

  /*The pin numbers and high, low values might be different depending on your connections */
  digitalWrite(motor_in1, LOW);
  digitalWrite(motor_in2, HIGH);
  digitalWrite(motor_in3, LOW);
  digitalWrite(motor_in4, LOW);
}

void left()
{
  analogWrite(enA, 80 ); //Left Motor Speed
  analogWrite(enB, 0 ); //Right Motor Speed

  /*The pin numbers and high, low values might be different depending on your connections */
  digitalWrite(motor_in1, LOW);
  digitalWrite(motor_in2, LOW);
  digitalWrite(motor_in3, LOW);
  digitalWrite(motor_in4, HIGH);
}

void sharpRightTurn() 
{
  analogWrite(enA, 80 ); //Left Motor Speed
  analogWrite(enB, 80 ); //Right Motor Speed

  /*The pin numbers and high, low values might be different depending on your connections */
  digitalWrite(motor_in1, LOW);
  digitalWrite(motor_in2, HIGH);
  digitalWrite(motor_in3, HIGH);
  digitalWrite(motor_in4, LOW);
}
void sharpLeftTurn() 
{
  analogWrite(enA, 80 ); //Left Motor Speed
  analogWrite(enB, 80 ); //Right Motor Speed

  /*The pin numbers and high, low values might be different depending on your connections */
  digitalWrite(motor_in1, HIGH);
  digitalWrite(motor_in2, LOW);
  digitalWrite(motor_in3, LOW);
  digitalWrite(motor_in4, HIGH);
}
