#ifndef __WIFLYSPECIFIC_H__
#define __WIFLYSPECIFIC_H__

EthernetClient client(80); 

#define PREFIX "/door"
WebServer webserver(PREFIX, 80);




//*****************************DEFAULT WEB SERVER CMD************************************
void doorCmd(WebServer &server, WebServer::ConnectionType type, char *, bool)
{


  if (type == WebServer::POST)
  {

    bool repeat;
    char name[16], value[16];
    do
    {
      /* readPOSTparam returns false when there are no more parameters
       * to read from the input.  We pass in buffers for it to store
       * the name and value strings along with the length of those
       * buffers. */
      repeat = server.readPOSTparam(name, 16, value, 16);

      /* this is a standard string comparison function.  It returns 0
       * when there's an exact match.*/
      if (strcmp(name, "cmd") == 0)
      {
        cmd = strtoul(value, NULL, 0);    
      }
      if (strcmp(name, "pwd") == 0)
      {
        strcpy(pwd,value); 
      }

    } 
    while (repeat);

    //zero out the command if the password doesn't match
    if (strcmp(pwd, webPassword) != 0)
    {
      loginattempt +=  1;
      Serial.println(loginattempt);
      if (cmd == 8)
      {
        server.httpSuccess();
        server.print("<login>");
        server.print("<auth>");
        server.print("0");
        server.print("</auth>");
        server.print("<count>");
        switch (loginattempt) {
        case 0:
          server.print("0");
          break;
        case 1:
          server.print("1");
          break;
        case 2:
          server.print("2");
          break;
        case 3:
          server.print("3");
          break;
        case 4:
          server.print("4");
          break;
        default:
          server.print("Locked Out");
        }
        server.print("</count>");
        server.print("</login>");
      }
      cmd = 0; 
    }

    /*if (loginattempt > 4) {  //lockout commands
     if (cmd == 8)
     {
     server.httpSuccess();
     server.print("<login>");
     server.print("<auth>");
     server.print("0");
     server.print("</auth>");
     server.print("<count>");
     server.print("Locked Out");
     server.print("</count>");
     server.print("</login>");
     }
     cmd = 0;  
     digitalWrite(errorLED, HIGH); 
     }*/
    //Return xml of status
    if (cmd == 7)
    {
      server.httpSuccess();
      server.print("<garage>");
      server.print("<left>");
      if (DoorOpen == true) server.print("open");
      if (DoorClosed == true) server.print("closed");
      if (DoorClosed == false && DoorOpen == false) server.print("unknown");
      server.print("</left>");
      if (doors == 2)
      {
        server.print("<right>");
        if (Door2Open == true) server.print("open");
        if (Door2Closed == true) server.print("closed");
        if (Door2Closed == false && Door2Open == false) server.print("unknown");
        server.print("</right>");
      }
      server.print("</garage>");

      cmd=0; 
    } 
    else if (cmd == 8)
    {
      loginattempt = 0;
      server.httpSuccess();
      server.print("<login>");
      server.print("<auth>");
      server.print("1");
      server.print("</auth>");
      server.print("<count>");
      server.print("0");
      server.print("</count>");
      server.print("</login>");
    }
  }  
  /* we don't output the body for a HEAD request */
  if (type == WebServer::GET)
  {
    /* for a GET or HEAD, send the standard "it's all OK headers" */
    server.httpSuccess();
    server.printP(htmlHeader);
    server.print(doors);
    server.printP(messageEnd);
  }
}
//*****************************END OF DEFAULT WEB SERVER CMD************************************

void EthernetSetup(){
  // setup the Ethernet library to talk to the Wiznet board
  Ethernet.begin(mac, ip);
  /* register our default command (activated with the request of
   * http://x.x.x.x/door */
  webserver.setDefaultCommand(&doorCmd);
  /* start the server to wait for connections */
  webserver.begin();
}
#endif

