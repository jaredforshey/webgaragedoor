#ifndef __CREDENTIALS_H__
#define __CREDENTIALS_H__

//#include "Arduino.h"

// Wifi parameters *********************************************
char ssid[] = "RouterSSID";
char passphrase[] = "yourpassword";



// Ethernet params *********************************************

// CHANGE THIS TO YOUR OWN UNIQUE VALUE
static uint8_t mac[6] = { 0x90, 0xA2, 0xDA, 0x00, 0x02, 0xE3 };

// CHANGE THIS TO MATCH YOUR HOST NETWORK
static uint8_t ip[4] = { 192, 168, 1, 97 };

//CHANGE THIS TO MATCH YOUR GATEWAY
static byte gateway[] = { 192, 168, 1, 1 };

//CHANGE THIS TO MATCH YOUR SUBNET
static byte subnet[] = { 255, 255, 0, 0 };

//CHANGE THIS TO MATCH the base64_encoded Text of username:password of your supertweet.net account.  SEE:http://www.functions-online.com/en/base64_encode.html
static char* auth = "";

//CHANGE THIS TO MATCH THE BASE64 ENCODED TEXT OF YOUR PASSWORD FOR THE GARAGE DOOR WEBSITE.  THIS MUST BE LESS THAN 16 CHARS AFTER ENCODING.
static char* webPassword = "MTIzNA=="; 

//CHANGE THIS TO MATCH YOUR TWITTER USERNAME
static char* twitterUserName = "";

#endif
