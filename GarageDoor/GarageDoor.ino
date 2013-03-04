//***********************************************************************
//
//                          DIRECTIVES
//
//***********************************************************************
#define SHIELD_TYPE 1
// ETHERNET = 0
// WIFLY = 1
// WIFI = 2

#define DOOR_SENSOR_TYPE 1 //Jared, Peter: Change this to a 0 on yours.
// 0:digital
// 1:analog

#define CRLF "\r\n"
#define TWEETING_ENABLED 0 //Jared, Peter: Change this to a 1 on yours.
#define WIFLY_RECONFIGURATE 0 //Set to 1 to re-confiure the WiFly Shield with different credentials. 
//Set to 0 to use the internally stored credentials.







//***********************************************************************
//
//                          DECLARATIONS
//
//***********************************************************************
//Left door = the only door in a 1 door config
//SET THIS TO HOW MANY DOORS YOU HAVE.  CURRENTLY SUPPORTS 1 OR 2
static int doors = 1;

boolean DoorOpen = false;
boolean DoorClosed = false;
boolean Door2Open = false;
boolean Door2Closed = false;

int errorLED = 6; //error LED
int ledPin =  7; // LED connected to digital pin 7
int ledPin2 =  8; // LED connected to digital pin 8

int cmd = 0; //command int 0= no cmd, 1=open, 2=close, 3=toggle, 4=open door 2, 5=close door 2, 6=toggle door 2, 7=get status, 8=Authenticate
char pwd[16];
unsigned long leftStateTimer;
unsigned long rightStateTimer;

#if DOOR_SENSOR_TYPE == 0
int closeSensor1 = 3; // close sensor door 1
int openSensor1 = 2; // open sensor door 1
int closeSensor2 = 5; // close sensor door 2
int openSensor2 = 4; // open sensor door 2
#elif DOOR_SENSOR_TYPE == 1
int analogOpenSensor1 = 0; // open analog sensor for Limit switches
int analogOpenSensor2 = 1; // open analog sensor for Limit switches
int analogCloseSensor1 = 2; // Close analog sensor for Limit switches
int analogCloseSensor2 = 3; // Close analog sensor for Limit switches
#endif

int leftState = 0;
int rightState = 0;

const float analogHigh = 4.9;
const float analogVoltageCoeff = 5.0 / 1023.0;

int loginattempt = 0; //login attempt counter

// Decide which Shield Code to load  
#include "Credentials.h"
#include "Common.h"

#if SHIELD_TYPE == 0
// Ethernet Shield used
//#include "Ethernet.h"
//#include "EthernetDNS.h"
//#include "WebServer.h"
//#include "NTP.h"
#include "EthernetSpecific.h"
#elif SHIELD_TYPE == 1
// Wifly Shield
#include <SPI.h>
#include <string.h>
#include <Time.h>
#include "Common.h"
#include "HtmlCode.h"
#include "WiFly.h"
#include "WiFlySpecific.h"

#endif


//*****************************PROCESS COMMANDS***************************************
void processCommands() {
  switch (cmd) {
  case 1:
    //Open command
    if (leftState == 0) activateDoor(&leftState, 1, &leftStateTimer, DoorClosed, DoorOpen, ledPin); //JARED
    break;
  case 2:
    //Close command
    if (leftState == 0) activateDoor(&leftState, 2, &leftStateTimer, DoorClosed, DoorOpen, ledPin); //JARED
    break;
  case 3:
    //toggle command
    if (leftState == 0) { //JARED
      digitalWrite(ledPin, HIGH);
      delay(500);
      digitalWrite(ledPin, LOW);
    }
    break;
  case 4:
    //Open door 2 command
    if (rightState == 0) activateDoor(&rightState, 1, &rightStateTimer, Door2Closed, Door2Open, ledPin2); //JARED
    break;
  case 5:
    //close door 2 command
    if (rightState == 0) activateDoor(&rightState, 2, &rightStateTimer, Door2Closed, Door2Open, ledPin2); //JARED
    break;
  case 6:
    //toggle door 2 command
    if (rightState == 0) { //JARED
      digitalWrite(ledPin2, HIGH);
      delay(500);
      digitalWrite(ledPin2, LOW);
    }
    break;
  }
  cmd = 0;
  activateDoor(&leftState, 0, &leftStateTimer, DoorClosed, DoorOpen, ledPin); //JARED
  activateDoor(&rightState, 0, &rightStateTimer, Door2Closed, Door2Open, ledPin2); //JARED
  //Serial.print(leftState); //debugging
  //Serial.print("  <--Left  Right-->  ");
  //Serial.println(rightState);
  if (leftState == 100) { //JARED whole funtion
    if (doors != 2)
    {
      tweet("Door Error @ "); 
    } 
    else 
    {
      tweet("Left Door Error @ "); 
    }
    activateDoor(&leftState, 3, &leftStateTimer, DoorClosed, DoorOpen, ledPin); //clear error
  }
  if (rightState == 100) { //JARED whole function
    tweet("Right Door Error @ ");
    activateDoor(&rightState, 3, &rightStateTimer, Door2Closed, Door2Open, ledPin2); //clear error
  }
}



//*****************************ACTIVATE COMMAND***************************************
int activateDoor(int *doorState, int action,  unsigned long *stateTimer, boolean closed, boolean open, int buttonOutput)  { //JARED
  switch (*doorState) {
  case 0: //Default state.  If action is 1, begin opening door.  If action is 2, begin closing door.
    if (action == 1) *doorState = 10; //Opening Door
    if (action == 2) *doorState = 50; //Closing Door
    *stateTimer = millis(); //entire process can take 20 seconds total
    break;

  case 10: //Door is idle.  If door not already open, trigger button, record timestamp, and move to next case.
    if (open != true) { 
      digitalWrite(buttonOutput, HIGH);
      delay(500);
      digitalWrite(buttonOutput, LOW);
      delay(500);
      *doorState = 20;
    }
    else if (open == true && closed == false) {
      *doorState = 0;
    }
    break;

  case 20: //Door is moving.  Wait for open or closed.  If open, set case to 0.  If closed, set case to 10.   If timeout expired, return 1
    if (open == true && closed == false) *doorState = 0;
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
    else if (open == false && closed == true) {
      *doorState = 0;
    }
    break;

  case 60: //Door is moving.  Wait for open or closed.  If closed, set case to 0.  If oo, set case to 10.   If timeout expired, return 1
    if (closed == true && open == false) *doorState = 0;
    else if (millis() - *stateTimer > 20000) *doorState = 100;
    else if (open == true) *doorState = 50;
    break;

  case 100: //Shit's wrong.  If action == 3, the error is ack'd
    digitalWrite(errorLED, HIGH);
    if (action == 3) *doorState = 0; //Ack'd
    break;
  }

  return *doorState;
}

void setup() {

  pinMode(errorLED, OUTPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(ledPin2, OUTPUT);

  Serial.begin(57600);


  //Set internal pull-up resistors
#if DOOR_SENSOR_TYPE == 0
  pinMode(openSensor1, INPUT);
  pinMode(closeSensor1, INPUT);
  pinMode(openSensor2, INPUT);
  pinMode(closeSensor2, INPUT);

  //By writing HIGH to input pins, we sets the pull-up resistors
  digitalWrite(openSensor1, HIGH);
  digitalWrite(closeSensor1, HIGH);
  digitalWrite(openSensor2, HIGH);
  digitalWrite(closeSensor2, HIGH);

  //Prevent arduino from tweeting on reset.
  if (digitalRead(openSensor1) == LOW) DoorOpen = true;
  if (digitalRead(closeSensor1) == LOW) DoorClosed = true;
  if (digitalRead(openSensor2) == LOW) Door2Open = true;
  if (digitalRead(closeSensor2) == LOW) Door2Closed = true; 
#elif DOOR_SENSOR_TYPE == 0
  //Set internal pull-up resistors
  pinMode(analogOpenSensor1, INPUT);
  pinMode(analogCloseSensor1, INPUT);
  pinMode(analogOpenSensor2, INPUT);
  pinMode(analogOpenSensor2, INPUT);

  //By writing HIGH to input pins, we sets the pull-up resistors
  digitalWrite(analogOpenSensor1, HIGH);
  digitalWrite(analogCloseSensor1, HIGH);
  digitalWrite(analogOpenSensor2, HIGH);
  digitalWrite(analogOpenSensor2, HIGH);

  //Prevent arduino from tweeting on reset.
  if (isHigh(analogOpenSensor1) == false) DoorOpen = true;
  if (isHigh(analogCloseSensor1) == false) DoorOpen = true;
  if (isHigh(analogOpenSensor2) == false) Door2Open = true;
  if (isHigh(analogOpenSensor2) == false) Door2Open = true;
#endif



#if SHIELD_TYPE == 0
  EthernetSetup();
#elif SHIELD_TYPE == 1
  WiFlySetup();
#endif
}

void loop() {
  leftStatus();
  rightStatus();

#if SHIELD_TYPE == 0
  webserver.processConnection();
#elif SHIELD_TYPE == 1
  processConnection();  
#endif

  processCommands();
}



