#ifndef __WIFLYSPECIFIC_H__
#define __WIFLYSPECIFIC_H__


WiFlyServer server(80);

//debug
int timesAround = 1;
int iConnectionDelayLimit = 0;

boolean readPOSTparam(char *name, int nameLen, char *value, int valueLen, WiFlyClient &client) {
  // assume name is at current place in stream
  int ch;

  Serial.println("readPOSTparam");

  // clear out name and value so they'll be NUL terminated
  memset(name, 0, nameLen);
  memset(value, 0, valueLen);

  // decrement length so we don't write into NUL terminator
  --nameLen;
  --valueLen;

  while ((ch = client.read()) != -1)
  {
    delay(1);
    Serial.print((char)ch);
    if (ch == '+')
    {
      ch = ' ';
    }
    else if (ch == '=')
    {
      /* that's end of name, so switch to storing in value */
      nameLen = 0;
      continue;
    }
    else if (ch == '&')
    {
      /* that's end of pair, go away */
      return true;
    }
    else if (ch == '%')
    {
      /* handle URL encoded characters by converting back to original form */
      int ch1 = client.read();
      int ch2 = client.read();
      if (ch1 == -1 || ch2 == -1)
        return false;
      char hex[3] = { 
        ch1, ch2, 0                                                 };
      ch = strtoul(hex, NULL, 16);
    }

    // output the new character into the appropriate buffer or drop it if
    // there's no room in either one.  This code will malfunction in the
    // case where the parameter name is too long to fit into the name buffer,
    // but in that case, it will just overflow into the value buffer so
    // there's no harm.
    if (nameLen > 0)
    {
      *name++ = ch;
      --nameLen;
    }
    else if (valueLen > 0)
    {
      *value++ = ch;
      --valueLen;
    }
  }

  // if we get here, we hit the end-of-file, so POST is over and there
  // are no more parameters

  Serial.println("FALSE");
  return false;
}

void authenticate(WiFlyClient &client){
  if (strcmp(pwd, webPassword) != 0)
  {
    loginattempt +=  1;
    //Serial.println(loginattempt);
    if (cmd == 8)
    {
      //server.httpSuccess();
      client.println("HTTP/1.0 200 OK\r\n"
        "Access-Control-Allow-Origin: *\r\n"
        "Content-Type: ");
      client.println("text/html; charset=utf-8");
      client.print("\r\n");

      client.print("<login><auth>0</auth><count>");
      switch (loginattempt) {
      case 0:
        client.print("0");
        break;
      case 1:
        client.print("1");
        break;
      case 2:
        client.print("2");
        break;
      case 3:
        client.print("3");
        break;
      case 4:
        client.print("4");
        break;
      default:
        client.print("Locked Out");
      }  
      client.print("</count>");
      client.print("</login>");
    }
    cmd = 0; 
  }

  if (cmd == 8)
  {
    //Serial.println("cmd=8 login passed");
    loginattempt = 0;

    client.println("HTTP/1.0 200 OK" CRLF
      "Access-Control-Allow-Origin: *" CRLF
      "Content-Type: ");
    client.println("text/html; charset=utf-8");
    client.print("\r\n");

    //    client.println("<login><auth>1</auth><count>0</count></login>");

    client.print("<login>");
    client.print("<auth>");
    client.print("1");
    client.print("</auth>");
    client.print("<count>");
    client.print("0");
    client.print("</count>");
    client.print("</login>");
  }
}

void processConnection(){ 
  WiFlyClient client = server.available();

  if (client) {
    // an http request ends with a blank line
    boolean current_line_is_blank = true;
    String message;
    bool repeat;

    while (client.connected()) {
      if (millis() >= 60000 * timesAround){
        Serial.print(millis() / 60000);
        Serial.println(" minutes have elapsed since power up");

        timesAround++;
      }
      if (client.available()) {
        char c;
        iConnectionDelayLimit=0;
        c = client.read();
        // if we've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so we can send a reply
        if (message.length() < 30)
          message.concat(c);

        if (message.indexOf("Disconn from") > -1) {
          Serial.println("Disconnection found");
          break;
        }
          
        if (message.indexOf("favicon")>-1) {
          Serial.println("favicon");
          break;
        }

        if (c == '\n' && current_line_is_blank) {
          Serial.print(message.substring(0,message.length()-1)); 

          if(message.indexOf("GET")>-1){
            client.printP(httpHeader);
            client.printP(htmlHeader);
           
            client.printP(messageEnd);
            Serial.println("messageEnd done");
            break;
          }
          else if (message.indexOf("POST")>-1){
            Serial.println("POST");
            char name[16], value[16];
            delay(100);
            do
            {
              repeat = readPOSTparam(name, 16, value, 16, client);
              if (strcmp(name, "cmd") == 0)
                cmd = strtoul(value, NULL, 0);

              if (strcmp(name, "pwd") == 0)
                strcpy(pwd,value);

            } 
            while (repeat);

            if (cmd==8){
              Serial.println("authenticate");
              authenticate(client);
              Serial.println("authenticated");
            }

            //Return xml of status
            if (cmd == 7){
              leftStatus();

              client.print("<garage>");
              client.print("<left>");

              if (DoorClosed == true && DoorOpen == true) 
                client.print("unknown");
              else if (DoorClosed == false && DoorOpen == false)
                client.print("unknown");
              else if (DoorOpen == true)
                client.print("open");
              else if (DoorClosed == true)
                client.print("closed");   

              client.print("</left>");

              if (doors == 2){
                client.print("<right>");
                if (Door2Open == true) client.print("open");
                if (Door2Closed == true) client.print("closed");
                if (Door2Closed == false && Door2Open == false) client.print("unknown");
                client.print("</right>");
              }
              client.print("</garage>");

              cmd=0; 
            }
            break;
          }
        }
        if (c == '\n')
          current_line_is_blank = true;
        else if (c != '\r')
          current_line_is_blank = false;
      } // End of "if client available"
    } // End of "While client Connected"

    // give the web browser time to receive the data
    delay(100);

    Serial.println("Stopping");
    client.stop();
    Serial.println("Stopped");
  } // End of "if(client)"

  if (millis() >= 60000 * timesAround){
    Serial.print(millis() / 60000);
    Serial.println(" minutes have elapsed since power up 2");

    timesAround++;
  }

}


void WiFlySetup(){
  Serial.println("Wifly Setup begin");

  //WiFly.FactoryReset();
  //WiFly.UpdateFirmware();

  WiFly.begin();

//This will set up automatic joining of your network using the credentials in Credentials.h
//You only need to do this once, the information will be stored in memory, even after you reboot
#if WIFLY_RECONFIGURATE == 1
  WiFly.setWiFiConfiguration(ssid, passphrase);
#endif

  server.begin();
}

#endif
