// 1 -Motor 1 Left
// 2 -Motor 1 Stop
// 3 -Motor 1 Right
//
// 4 -Motor 2 Left
// 5 -Motor 2 Stop
// 6 -Motor 2 Right

// Declare L298N Dual H-Bridge Motor Controller directly since there is not a library to load.

// Motor 1
int dir1PinA = 4;
int dir2PinA = 5;
int speedPinA = 3; // Needs to be a PWM pin to be able to control motor speed

// Motor 2
int dir1PinB = 7;
int dir2PinB = 8;
int speedPinB = 6; // Needs to be a PWM pin to be able to control motor speed

// pins for the LEDs:
const int ledPin = 13;

void setup() {  // Setup runs once per reset
  // initialize serial communication @ 9600 baud:
  Serial.begin(9600);

  //Define L298N Dual H-Bridge Motor Controller Pins

  pinMode(dir1PinA, OUTPUT);
  pinMode(dir2PinA, OUTPUT);
  pinMode(speedPinA, OUTPUT);
  pinMode(dir1PinB, OUTPUT);
  pinMode(dir2PinB, OUTPUT);
  pinMode(speedPinB, OUTPUT);
  pinMode(ledPin, OUTPUT);
}

int moveData = 100;
int rightSpeed = 100;
int leftSpeed = 100;

void loop() {

  // if there's any serial available, read it:
  if (Serial.available() > 0)
  {
    do
    {
      char readData = Serial.read();
      if (readData == 'l')
      {
        Serial.println("l");
        leftSpeed = Serial.parseInt();
      }
      else if (readData == 'r')
      {
        Serial.println("r");
        rightSpeed = Serial.parseInt();
      }
      else if (readData == 'm')
      {
        Serial.println("m");
        moveData = Serial.parseInt();
      }
      else if (readData == 'f')
      {
        Serial.println("f");
        updateRotation();
      }
    } while (Serial.available() > 0);
    updateRotation();
  }
}

void updateRotation()
{
  if (moveData == 100 && rightSpeed == 100 && leftSpeed == 100)
  {
    digitalWrite(ledPin, HIGH);
    //freeRun
    activateMotorByCommand(2, 0);
    activateMotorByCommand(5, 0);
  }
  else
  {
    digitalWrite(ledPin, LOW);
  }
  int calculatedLeftSpeed = calculateSpeed(leftSpeed);
  int calculatedRightSpeed = calculateSpeed(rightSpeed);
  int calculatedMoveSpeed = calculateSpeed(moveData);

  if (moveData >= 100)
  {
    //forward

    if (rightSpeed >= 100)
    {
      activateMotorByCommand(1, calculatedMoveSpeed + calculatedRightSpeed);
    }
    else// if (rightSpeed < 100)
    {
      if (calculatedMoveSpeed > calculatedRightSpeed)
        activateMotorByCommand(1, calculatedMoveSpeed - calculatedRightSpeed);
      else
        activateMotorByCommand(3, calculatedRightSpeed - calculatedMoveSpeed);
    }

    if (leftSpeed >= 100)
    {
      activateMotorByCommand(4, calculatedMoveSpeed + calculatedLeftSpeed);
    }
    else// if (leftSpeed < 100)
    {
      if (calculatedMoveSpeed > calculatedLeftSpeed)
        activateMotorByCommand(4, calculatedMoveSpeed - calculatedLeftSpeed);
      else
        activateMotorByCommand(6, calculatedLeftSpeed - calculatedMoveSpeed);
    }

  }
  else if (moveData < 100)
  {
    if (rightSpeed <= 100)
    {
      activateMotorByCommand(3, calculatedMoveSpeed + calculatedRightSpeed);
    }
    else //if (rightSpeed > 100)
    {
      if (calculatedMoveSpeed > calculatedRightSpeed)
        activateMotorByCommand(3, calculatedMoveSpeed - calculatedRightSpeed);
      else
        activateMotorByCommand(1, calculatedRightSpeed - calculatedMoveSpeed);
    }

    if (leftSpeed <= 100)
    {
      activateMotorByCommand(6, calculatedMoveSpeed + calculatedLeftSpeed);
    }
    else// if (leftSpeed > 100)
    {
      if (calculatedMoveSpeed > calculatedLeftSpeed)
        activateMotorByCommand(6, calculatedMoveSpeed - calculatedLeftSpeed);
      else
        activateMotorByCommand(4, calculatedLeftSpeed - calculatedMoveSpeed);

    }
  }
}


int calculateSpeed(int data)
{
  if (data == 100)
  {
    return 0;
  }
  else if (data > 100)
  {
    return (data - 100) * 2;
  }
  else if (data < 100)
  {
    return (100 - data) * 2;
  }
}

void activateMotorByCommand(int command, int speed) {

  switch (command) {

    //______________Motor 1______________

    case 1: // Motor 1 Forward
      analogWrite(speedPinA, speed);//Sets speed variable via PWM
      digitalWrite(dir1PinA, LOW);
      digitalWrite(dir2PinA, HIGH);
      //Serial.println("Motor 1 Forward ");
      break;

    case 2: // Motor 1 Stop (Freespin)
      analogWrite(speedPinA, 0);
      digitalWrite(dir1PinA, LOW);
      digitalWrite(dir2PinA, LOW);
      //Serial.println("Motor 1 Stop");
      break;

    case 3: // Motor 1 Reverse
      analogWrite(speedPinA, speed);
      digitalWrite(dir1PinA, HIGH);
      digitalWrite(dir2PinA, LOW);
      //Serial.println("Motor 1 Reverse");
      break;

    //______________Motor 2______________

    case 4: // Motor 2 Forward
      analogWrite(speedPinB, speed);
      digitalWrite(dir1PinB, LOW);
      digitalWrite(dir2PinB, HIGH);
      //Serial.println("Motor 2 Forward");
      break;

    case 5: // Motor 1 Stop (Freespin)
      analogWrite(speedPinB, 0);
      digitalWrite(dir1PinB, LOW);
      digitalWrite(dir2PinB, LOW);
      //Serial.println("Motor 2 Stop");
      break;

    case 6: // Motor 2 Reverse
      analogWrite(speedPinB, speed);
      digitalWrite(dir1PinB, HIGH);
      digitalWrite(dir2PinB, LOW);
      //Serial.println("Motor 2 Reverse ");
      break;

    default:
      // turn all the connections off if an unmapped key is pressed:
      for (int thisPin = 2; thisPin < 11; thisPin++) {
        digitalWrite(thisPin, LOW);
      }
  }
}

