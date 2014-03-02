//***********************************************************************
//
//                          DIRECTIVES
//
//***********************************************************************
#define SHIELD_TYPE 2
// ETHERNET = 0
// WIFLY = 1
// WIFI = 2

#define CRLF "\r\n"
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

#if SHIELD_TYPE == 2
int ledPin =  9; //The WiFi shield uses pin 7 for handshake with arduino -- cannot be used for Rev A of garage door opener board.
#else
int ledPin =  7; // Output connected to digital pin 7. Open/Close door.
#endif

int ledPin2 =  8; // Output connected to digital pin 8. Open/Close door.


int cmd = 0; //command int 0= no cmd, 1=open, 2=close, 3=toggle, 4=open door 2, 5=close door 2, 6=toggle door 2, 7=get status, 8=Authenticate
char pwd[16];
unsigned long leftStateTimer;
unsigned long rightStateTimer;

int closeSensor1 = 3; // close sensor door 1
int openSensor1 = 2; // open sensor door 1
int closeSensor2 = 5; // close sensor door 2
int openSensor2 = 4; // open sensor door 2

int leftState = 0;
int rightState = 0;

int loginattempt = 0; //login attempt counter

// Decide which Shield Code to load  
#include "Credentials.h"
#include "Common.h"
#include "HtmlCode.h"

#if SHIELD_TYPE == 0
// Ethernet Shield used
#include "Ethernet.h"
#include "WebServer.h"
#include "SPI.h"
//#include "SD.h"
#include "EthernetSpecific.h"

#elif SHIELD_TYPE == 1
// Wifly Shield
#include <SPI.h>
#include <string.h>
#include <Time.h>
#include "WiFly.h"
#include "WiFlySpecific.h"

#elif SHIELD_TYPE == 2
// Wifi shield
#include <SPI.h>
#include <WiFi.h>
#include "WiFiSpecific.h"
#endif

void setup() {

  Serial.begin(9600);
  //Serial.println("Serial Started");
  
  pinMode(errorLED, OUTPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(ledPin2, OUTPUT);

  //Set internal pull-up resistors
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

#if SHIELD_TYPE == 0
  EthernetSetup();
#elif SHIELD_TYPE == 1
  WiFlySetup();
#elif SHIELD_TYPE == 2
  WiFiSetup();
#endif
}

void loop() {
  leftStatus();
  rightStatus();

#if SHIELD_TYPE == 0
  webserver.processConnection();
#elif SHIELD_TYPE == 1
  processConnection();  
#elif SHIELD_TYPE == 2
  processConnection();  
#endif

  processCommands();
}


