#ifndef __COMMON_H__
#define __COMMON_H__




//***********************************************************************
//
//                          FUNCTIONS
//
//***********************************************************************

//***************************** LEFT DOOR *************************************************************************
void leftStatus(){
  //get garage door status
  if ((digitalRead(openSensor1) == LOW) && (DoorOpen != true)) { //delay and check again for debounce
    delay(500);
    if ((digitalRead(openSensor1) == LOW) && (DoorOpen != true)){ //open
      DoorClosed = false;
      DoorOpen = true;
      if (doors == 2 && (leftState == 0 || leftState == 20)) {
        Serial.println("Left door opened");
      } 
      else if (leftState == 0 || leftState == 20) {
        Serial.println("opened");
      }
    }
  }
  else if ((digitalRead(closeSensor1) == LOW) && (DoorClosed != true)) { //delay and check again for debounce
    delay(500);
    if ((digitalRead(closeSensor1) == LOW) && (DoorClosed != true)){
      DoorClosed = true;
      DoorOpen = false;
      if (doors == 2 && (leftState == 0 || leftState == 60)) {
        Serial.println("Left door closed");
      } 
      else if (leftState == 0 || leftState == 60){
        Serial.println("closed");
      }
    }
  } 
  else if (digitalRead(closeSensor1) == HIGH && digitalRead(openSensor1) == HIGH  && (DoorClosed == true || DoorOpen == true)) {
    delay(500);
    if (digitalRead(closeSensor1) == HIGH && digitalRead(openSensor1) == HIGH) {
      DoorClosed = false;
      DoorOpen = false;
      Serial.println("Left Door Semi-open");
    }
  }
}



//***************************************** RIGHT DOOR ******************************************
void rightStatus() {
  //get door 2 status
  if (doors == 2) {
    if ((digitalRead(openSensor2) == LOW) && (Door2Open != true)) {//delay and check again for debounce
      delay(500);
      if ((digitalRead(openSensor2) == LOW) && (Door2Open != true)){ //open
        Door2Closed = false;
        Door2Open = true;
        if (rightState == 0 || rightState == 20)
        Serial.println("Right door opened");
      }
    }
    else if ((digitalRead(closeSensor2) == LOW) && (Door2Closed != true)) { //delay and check again for debounce
      delay(500);
      if ((digitalRead(closeSensor2) == LOW) && (Door2Closed != true)){
        Door2Closed = true;
        Door2Open = false;
        if (rightState == 0 || rightState == 60)
        Serial.println("Right door closed");
      }
    }
    else if (digitalRead(closeSensor2) == HIGH && digitalRead(openSensor2) == HIGH && (Door2Closed == true || Door2Open == true)) {
      delay(500);
      if (digitalRead(closeSensor2) == HIGH && digitalRead(openSensor2) == HIGH) {
        Door2Closed = false;
        Door2Open = false;
        Serial.println("Right Door Semi-open");
      }
    }
  }
}

//*****************************ACTIVATE COMMAND***************************************
int activateDoor(int *doorState, int action,  unsigned long *stateTimer, boolean closed, boolean opened, int buttonOutput)  { //JARED
  switch (*doorState) {
  case 0: //Default state.  If action is 1, begin opening door.  If action is 2, begin closing door.
    digitalWrite(errorLED, LOW);
    if (action == 1) *doorState = 10; //Opening Door
    if (action == 2) *doorState = 50; //Closing Door
    *stateTimer = millis(); //entire process can take 20 seconds total
    break;

  case 10: //Door is idle.  If door not already open, trigger button, record timestamp, and move to next case.
    if (opened != true) { 
      digitalWrite(buttonOutput, HIGH);
      delay(500);
      digitalWrite(buttonOutput, LOW);
      delay(500);
      *doorState = 20;
    }
    else if (opened == true && closed == false) {
      *doorState = 0;
    }
    break;

  case 20: //Door is moving.  Wait for open or closed.  If open, set case to 0.  If closed, set case to 10.   If timeout expired, return 1
    if (opened == true && closed == false) *doorState = 0;
    else if (millis() - *stateTimer > 20000) *doorState = 100;
    else if (closed == true) *doorState = 10;
    break;

  case 50: //Door is idle.  If door not already closed, trigger button, record timestamp, and move to next case.
    if (closed != true) { 
      digitalWrite(buttonOutput, HIGH);
      delay(500);
      digitalWrite(buttonOutput, LOW);
      delay(500);
      *doorState =60;
    }
    else if (opened == false && closed == true) {
      *doorState = 0;
    }
    break;

  case 60: //Door is moving.  Wait for open or closed.  If closed, set case to 0.  If oo, set case to 10.   If timeout expired, return 1
    if (closed == true && opened == false) *doorState = 0;
    else if (millis() - *stateTimer > 20000) *doorState = 100;
    else if (opened == true) *doorState = 50;
    break;

  case 100: //Shit's wrong.  If action == 3, the error is ack'd
    digitalWrite(errorLED, HIGH);
    if (action == 3) *doorState = 0; //Ack'd
    break;
  }

  return *doorState;
}

//*****************************PROCESS COMMANDS***************************************
void processCommands() {
  switch (cmd) {
  case 1:
    //Open command
    if (leftState == 0) activateDoor(&leftState, 1, &leftStateTimer, DoorClosed, DoorOpen, ledPin);
    break;
  case 2:
    //Close command
    if (leftState == 0) activateDoor(&leftState, 2, &leftStateTimer, DoorClosed, DoorOpen, ledPin);
    break;
  case 3:
    //toggle command
    if (leftState == 0) {
      digitalWrite(ledPin, HIGH);
      delay(500);
      digitalWrite(ledPin, LOW);
    }
    break;
  case 4:
    //Open door 2 command
    if (rightState == 0) activateDoor(&rightState, 1, &rightStateTimer, Door2Closed, Door2Open, ledPin2);
    break;
  case 5:
    //close door 2 command
    if (rightState == 0) activateDoor(&rightState, 2, &rightStateTimer, Door2Closed, Door2Open, ledPin2);
    break;
  case 6:
    //toggle door 2 command
    if (rightState == 0) {
      digitalWrite(ledPin2, HIGH);
      delay(500);
      digitalWrite(ledPin2, LOW);
    }
    break;
  }
  cmd = 0;
  activateDoor(&leftState, 0, &leftStateTimer, DoorClosed, DoorOpen, ledPin);
  activateDoor(&rightState, 0, &rightStateTimer, Door2Closed, Door2Open, ledPin2);
  //Serial.print(leftState); //debugging
  //Serial.print("  <--Left  Right-->  ");
  //Serial.println(rightState);
  if (leftState == 100) {
    activateDoor(&leftState, 3, &leftStateTimer, DoorClosed, DoorOpen, ledPin); //clear error
  }
  if (rightState == 100) {
    activateDoor(&rightState, 3, &rightStateTimer, Door2Closed, Door2Open, ledPin2); //clear error
  }
}
#endif
