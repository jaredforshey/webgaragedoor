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
    P(message) = 
      "<!DOCTYPE html>"
      "<html>"
      "<head>"
      "<title>Web Garage Door</title>"
      "<meta name='viewport' content='width=device-width; initial-scale=1.0; maximum-scale=1.0; user-scalable=0;' />"
      "<meta name='apple-touch-fullscreen' content='YES' />"
      "<script type='text/javascript' src='http://ajax.googleapis.com/ajax/libs/jquery/1.4.2/jquery.min.js'></script>"
      "<script type='text/javascript' src='http://ajax.googleapis.com/ajax/libs/jqueryui/1.8.2/jquery-ui.min.js'></script>"
      "<link rel='stylesheet' href='http://ajax.googleapis.com/ajax/libs/jqueryui/1.8.2/themes/dot-luv/jquery-ui.css' />"
      "<script type='text/javascript'>var doors=";
      server.printP(message);
      server.print(doors);
      P(message2) = 
      ";</script>"
      "<script src='http://ROV2776/WebGarageDoor/js/main.js' type='text/javascript'></script>"
      "<style type='text/css'>"
      "#slider{margin: 10px;}"
      ".body{vertical-align: top;text-align: center;color: White;margin: 0 auto;border: 1px solid white;}"
      "#wrapper{width: 305px;height: 400px;border: none;margin: 0 auto;}"
      "</style>"
      "</head>"
      "<body id='page' style='font-size: 62.5%; background-color: #5F5A59;' onload='setTimeout(function() { window.scrollTo(0, 1) }, 100);' >"
      "</body>"
      "</html>";
    server.printP(message2);
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
