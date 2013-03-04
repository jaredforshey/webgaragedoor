#ifndef __COMMON_H__
#define __COMMON_H__




//***********************************************************************
//
//                          FUNCTIONS
//
//***********************************************************************


//*****************************TWEET*************************************
void tweet(char* msg){
#if TWEETING_ENABLED == 1
  Serial.println("connecting...");
  unsigned long time = ntp.get_unix_tz(-5);
  if (client.connect()) {
    Serial.println("connected");
    client.println("POST http://api.supertweet.net/1/statuses/update.json HTTP/1.1");
    client.println("Host: supertweet.net");
    client.print("Authorization: Basic ");
    client.println(auth);
    client.println("Content-type: application/x-www-form-urlencoded");
    client.print("Content-length: ");	
    client.println(9+strlen(msg)+10);				
    client.println("Connection: Close");
    client.println();
    client.print("status=");
    client.print(msg);
    client.println(time);
    Serial.println("Tweet complete");

  } 
  else {
    Serial.println("connection failed");
  }
  while(client.connected())
  {
    if (client.available()) {
      char c = client.read();
      Serial.print(c);
    }
  }
  Serial.println("Disconnecting...");
  client.stop();
#endif
}

boolean isHigh(int pin) {
  if ((analogRead(pin) * analogVoltageCoeff) > analogHigh)
    return true;
  else
    return false;
}



//***************************** LEFT DOOR *************************************************************************
#if DOOR_SENSOR_TYPE == 1
void leftStatusDoorUnknownHigh(boolean &DoorClosed, boolean &DoorOpen) {
  DoorClosed = false;
  DoorOpen = false;
}

void leftStatusDoorUnknownLow(boolean &DoorClosed, boolean &DoorOpen) {
  DoorClosed = false;
  DoorOpen = false;
}

void leftStatusDoorOpen(int doors, boolean &DoorClosed, boolean &DoorOpen) {
  DoorClosed = false;
  DoorOpen = true;

  if (doors == 2 && (leftState == 0 || leftState == 20)) { //JARED
    //tweet("Left Door Opened @ ");
    //Serial.println("Left door opened");
  } 
  else if (leftState == 0 || leftState == 20) { //JARED
    //tweet("Opened @ ");
    //Serial.println("opened");
  }
}

void leftStatusDoorClosed(int doors, boolean &DoorClosed, boolean &DoorOpen) {
  DoorClosed = true;
  DoorOpen = false;

  if (doors == 2 && (leftState == 0 || leftState == 60)) { //JARED
    //tweet("Left Door Closed @ ");
    //Serial.println("Left door closed");
  } 
  else if (leftState == 0 || leftState == 60) { //JARED
    //tweet("Closed @ ");
    //Serial.println("closed");
  }
}
#endif

void leftStatus(){
#if DOOR_SENSOR_TYPE == 0
  //get garage door status
  if ((digitalRead(openSensor1) == LOW) && (DoorOpen != true)) { //open
    DoorClosed = false;
    DoorOpen = true;
    if (doors == 2 && (leftState == 0 || leftState == 20)) { //JARED
      //tweet("Left Door Opened @ ");
      Serial.println("Left door opened");
    } 
    else if (leftState == 0 || leftState == 20) { //JARED
      //tweet("Opened @ ");
      Serial.println("opened");
    }
  }
  else if ((digitalRead(closeSensor1) == LOW) && (DoorClosed != true)) { //closed
    DoorClosed = true;
    DoorOpen = false;
    if (doors == 2 && (leftState == 0 || leftState == 60)) { //JARED
      //tweet("Left Door Closed @ ");
      Serial.println("Left door closed");
    } 
    else if (leftState == 0 || leftState == 60){ //JARED
      //tweet("Closed @ ");
      Serial.println("closed");
    }
  } 
  else if (digitalRead(closeSensor1) == HIGH && digitalRead(openSensor1) == HIGH  && (DoorClosed == true || DoorOpen == true)) {
    delay(250);
    if (digitalRead(closeSensor1) == HIGH && digitalRead(openSensor1) == HIGH) {
      DoorClosed = false;
      DoorOpen = false;
    }
  }
#elif DOOR_SENSOR_TYPE == 1
  if ((isHigh(analogOpenSensor1) == false) && (DoorOpen != true)) {
    leftStatusDoorOpen(doors, DoorOpen, DoorClosed);
  } 
  else if ((isHigh(analogCloseSensor1) == false) && (DoorClosed != true)) {
    leftStatusDoorClosed(doors, DoorOpen, DoorClosed);
  } 
  else if ((isHigh(analogOpenSensor1) == isHigh(analogCloseSensor1)) && (DoorClosed == true || DoorOpen == true)) {
    delay(250);
    if (isHigh(analogOpenSensor1) == isHigh(analogCloseSensor1)) {
      leftStatusDoorUnknownHigh(DoorOpen, DoorClosed);
    }
  }

#endif
}



//***************************************** RIGHT DOOR ******************************************
void rightStatus() {
  #if DOOR_SENSOR_TYPE == 0
  //get door 2 status
  if (doors == 2) {
    if ((digitalRead(openSensor2) == LOW) && (Door2Open != true)) {//open
      Door2Closed = false;
      Door2Open = true;
      if (rightState == 0 || rightState == 20) 
        tweet("Right Door Opened @ "); //JARED
      Serial.println("Right door opened");
    }
    else if ((digitalRead(closeSensor2) == LOW) && (Door2Closed != true)) {//closed
      Door2Closed = true;
      Door2Open = false;
      if (rightState == 0 || rightState == 60) 
        tweet("Right Door Closed @ "); //JARED
      Serial.println("Right door closed");
    }
    else if (digitalRead(closeSensor2) == HIGH && digitalRead(openSensor2) == HIGH && (Door2Closed == true || Door2Open == true)) {
      delay(250);
      if (digitalRead(closeSensor2) == HIGH && digitalRead(openSensor2) == HIGH) {
        Door2Closed = false;
        Door2Open = false;
      }
    }
  }
  #elif DOOR_SENSOR_TYPE == 1
  //get door 2 status
  if (doors == 2) {
    if ((digitalRead(analogOpenSensor2) == LOW) && (Door2Open != true)) {//open
      Door2Closed = false;
      Door2Open = true;
      if (rightState == 0 || rightState == 20) 
        tweet("Right Door Opened @ "); //JARED
      Serial.println("Right door opened");
    }
    else if ((digitalRead(analogCloseSensor2) == LOW) && (Door2Closed != true)) {//closed
      Door2Closed = true;
      Door2Open = false;
      if (rightState == 0 || rightState == 60) 
        tweet("Right Door Closed @ "); //JARED
      Serial.println("Right door closed");
    }
    else if (digitalRead(analogCloseSensor2) == HIGH && digitalRead(analogOpenSensor2) == HIGH && (Door2Closed == true || Door2Open == true)) {
      delay(250);
      if (digitalRead(analogCloseSensor2) == HIGH && digitalRead(analogOpenSensor2) == HIGH) {
        Door2Closed = false;
        Door2Open = false;
      }
    }
  }
  #endif
}

#endif

