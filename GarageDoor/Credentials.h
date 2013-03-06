#ifndef __CREDENTIALS_H__
#define __CREDENTIALS_H__

//#include "Arduino.h"

// Wifi parameters *********************************************
char ssid[] = "RouterSSID";
char passphrase[] = "yourpassword";



// Ethernet params *********************************************

// CHANGE THIS TO YOUR OWN UNIQUE VALUE
static uint8_t mac[6] = { 0x90, 0xA2, 0xDA, 0x0D, 0x75, 0x81 };

// CHANGE THIS TO MATCH YOUR HOST NETWORK
static uint8_t ip[4] = { 172, 24, 189, 1 };

//CHANGE THIS TO MATCH THE BASE64 ENCODED TEXT OF YOUR PASSWORD FOR THE GARAGE DOOR WEBSITE.  THIS MUST BE LESS THAN 16 CHARS AFTER ENCODING.
static char* webPassword = "cGFzc3dvcmQ="; //"cGFzc3dvcmQ=" = "password"  USE THIS FOR TESTING

#endif
