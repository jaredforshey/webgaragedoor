#ifndef __WIFLYSPECIFIC_H__
#define __WIFLYSPECIFIC_H__



byte time_dot_nist_dot_gov[] = { 
  192, 43, 244, 18}; // time.nist.gov
NTP ntp(time_dot_nist_dot_gov);

//Twitter info
byte server[] = { 
  66, 180, 175, 246 };  // supertweet ip 

// Original Code, not valid after Arduino 1.0
//Client client(server, 80); // client is referencing twitter
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
      "<!DOCTYPE html PUBLIC '-//W3C//DTD XHTML 1.0 Transitional//EN' 'http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd'>"
      "<html xmlns='http://www.w3.org/1999/xhtml'>"
      "<head>"
      "<meta name='viewport' content='width=device-width; initial-scale=1.0; maximum-scale=1.0; user-scalable=0;' />"
      "<meta name='apple-touch-fullscreen' content='YES' />"
      "<title>Web Enabled Garage Door</title>"
      "<script type='text/javascript' src='http://ajax.googleapis.com/ajax/libs/jquery/1.4.2/jquery.min.js'></script>"
      "<script type='text/javascript' src='http://ajax.googleapis.com/ajax/libs/jqueryui/1.8.2/jquery-ui.min.js'></script>"
      "<script type='text/javascript' src='http://dl.dropbox.com/u/3547649/ScriptLib/tweet/jquery.tweet.js'></script>"
      "<link rel='stylesheet' href='http://ajax.googleapis.com/ajax/libs/jqueryui/1.8.2/themes/dot-luv/jquery-ui.css' />"
      "<link rel='stylesheet' href='http://dl.dropbox.com/u/3547649/ScriptLib/tweet/jquery.tweet.css' />"
      "<style type='text/css'>"
      "#slider{margin: 10px;}"
      ".body{vertical-align: top;text-align: center;color: White;margin: 0 auto;border: 1px solid white;}"
      "#wrapper{width: 305px;height:400px;border:none;margin: 0 auto;}"
      "</style>"
      "<script type='text/javascript'>"
      "var pwd = '';"
      "$(function () {"
      "$('button').button();"
      "$('#tabs').tabs();"
      "showlogin();"
      "});"
      "function login() {"
      "encode();"
      "$('#pwdInput').val('');"
      "$.post('/door', { cmd: '8', pwd: pwd },"
      "function(data){"
      "passfail(data);"
      "});"
      "}"
      "function passfail(xml) {"
      "var pass = 'test';"
      "var count = 'test';"
      "$(xml).find('auth').each(function(){"
      "pass = $(this).text();"
      "});" 
      "$(xml).find('count').each(function(){"
      "count = $(this).text();"
      "});" 
      "if (pass == 1) {"
      "$('#bodyPage').show();"
      "$('#loginPage').hide();"
      "status();"
      "history();"
      "} else {"
      "$('#loginCounter').show();"
      "if (count != 'Locked Out') {"
      "$('#loginCounter strong').text('Login failed!  Used ' + count + ' of 5 attempts.');"
      "} else {"
      "$('#loginCounter strong').text(count + '!');"
      "}"
      "}"
      "}" 
      "function showlogin() {"
      "$('#loginPage').show();"
      "$('#pwdInput').focus();"
      "}" 
      "function encode() {"
      "var input = $('#pwdInput').val();"
      "var _keyStr = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/='; "
      "var output ='';"
      "var chr1, chr2, chr3, enc1, enc2, enc3, enc4;"
      "var i = 0;"
      "while (i < input.length) {"
      "chr1 = input.charCodeAt(i++);"
      "chr2 = input.charCodeAt(i++);"
      "chr3 = input.charCodeAt(i++);"
      "enc1 = chr1 >> 2;"
      "enc2 = ((chr1 & 3) << 4) | (chr2 >> 4);"
      "enc3 = ((chr2 & 15) << 2) | (chr3 >> 6);"
      "enc4 = chr3 & 63;"
      "if (isNaN(chr2)) {"
      "enc3 = enc4 = 64;"
      "} else if (isNaN(chr3)) {"
      "enc4 = 64;"
      "}"
      "output = output + _keyStr.charAt(enc1) + _keyStr.charAt(enc2) + _keyStr.charAt(enc3) + _keyStr.charAt(enc4);"
      "}"
      "pwd = output;"
      "}"

      "function process(xml) {"
      "$('#leftopen').hide();"
      "$('#leftclosed').hide();"
      "$('#leftunknown').hide();"
      "$('#rightopen').hide();"
      "$('#rightclosed').hide();"
      "$('#rightunknown').hide();"
      "var leftstatus = 0;"
      "var rightstatus = 0;"
      "$(xml).find('left').each(function(){"
      "leftstatus = $(this).text();"
      "});" 
      "$(xml).find('right').each(function(){"
      "rightstatus = $(this).text();"
      "});"
      "if( leftstatus == 'open') { $('#leftopen').show();}"
      "if( leftstatus == 'closed') { $('#leftclosed').show();}"
      "if( leftstatus == 'unknown') { $('#leftunknown').show();}"
      "if( rightstatus == 'open') { $('#rightopen').show();}"
      "if( rightstatus == 'closed') { $('#rightclosed').show();}"
      "if( rightstatus == 'unknown') { $('#rightunknown').show();}"
      "}"
      "function history() {"
      "$('.tweet').html('');"
      "$('.tweet').tweet({"
      "username: '";
    server.printP(message);               
    server.print(twitterUserName);
    P(message2) =                 
      "',join_text: 'auto',"
      "avatar_size: 32,"
      "count: 5,"
      "auto_join_text_default: '',"
      "auto_join_text_ed: '',"
      "auto_join_text_ing: '',"
      "auto_join_text_reply: '',"
      "auto_join_text_url: '',"
      "loading_text: 'loading tweets...'"
      "});"
      "}"
      "function status() {"
      "window.scrollTo(0, 1);"
      "$.post('/door', { cmd: '7', pwd: pwd },"
      "function(data){"
      "process(data);"
      "});"
      "}"
      "function OpenDoor() {"
      "if(confirm('Open door?')){"
      "$.post('/door', { cmd: '1', pwd: pwd });"
      "}}"
      "function CloseDoor() {"
      "if(confirm('Close door?')){"
      "$.post('/door', { cmd: '2', pwd: pwd });"
      "}}"
      "function ActuateDoor() {"
      "if(confirm('Toggle door?')){"
      "$.post('/door', { cmd: '3', pwd: pwd });"
      "}}"
      "function OpenDoor2() {"
      "if(confirm('Open door?')){"
      "$.post('/door', { cmd: '4', pwd: pwd });"
      "}}"
      "function CloseDoor2() {"
      "if(confirm('Close door?')){"
      "$.post('/door', { cmd: '5', pwd: pwd });"
      "}}"
      "function ActuateDoor2() {"
      "if(confirm('Toggle door?')){"
      "$.post('/door', { cmd: '6', pwd: pwd });"
      "}}"
      "</script>"
      "</head>"
      "<body style='font-size: 62.5%; background-color: #5F5A59;' onload='setTimeout(function() { window.scrollTo(0, 1) }, 100);' >"
      "<div id='wrapper' class='body'>"
      "<div id='loginPage'>"
      "<div class='ui-widget ui-corner-all ui-widget-content'><div class='ui-widget ui-corner-all ui-widget-header' style='height:30px;'><div class='ui-datepicker-month'>Login</div></div>"
      "<div style='padding-bottom: 13px;padding-left: 18px;padding-right: 18px;padding-top: 13px;'>"
      "<input type='password' id='pwdInput'></input>"
      "<br/><br/><button id='loginSubmit' onclick='javascript:login();'>Authenticate</button><br /><br />"
      "<div id='loginCounter' class='ui-widget' style='display:none'> "
      "<div class='ui-state-error ui-corner-all' style='padding: 0 .7em;'> "
      "<p><span class='ui-icon ui-icon-alert' style='float: left; margin-right: .3em;'></span> "
      "<strong>Door Partially Open!</strong></p> "
      "</div> "
      "</div></div></div>"
      "</div>"
      "<div id='bodyPage' style='display:none'>"
      "<button id='refresh' onclick='javascript:status();'>Refresh</button><br /><br />"
      "<div id='tabs'>"
      "<ul>"
      "<li><a href='#tabs-1'>Controls</a></li>"
      "<li><a href='#tabs-2'>History</a></li>"
      "</ul>"
      "<div id='tabs-1'>"
      "<br />";
    server.printP(message2);
    if (doors != 2) 
      //show 1 door buttons and controls **********************************************************************************************************************************
    {
      P(statusbuttons)=
        "<button style='width:145px;height:40px;' id='opendoor' onclick='javascript:OpenDoor();'>Open Door</button><br />"
        "<button style='width:145px;height:40px;margin-top:4px;' id='closedoor' onclick='javascript:CloseDoor();'>Close Door</button><br />"
        "<button style='width:145px;height:40px;margin-top:4px;' id='actuate' onclick='javascript:ActuateDoor();'>Actuate Door</button><br />"
        "<br /><hr /><br />";
      server.printP(statusbuttons);

      P(OneDoorStatus) =
        "<div id='leftopen' onmousedown='javascript:status(); class='ui-widget' style='display:none'> "
        "<div class='ui-state-error ui-corner-all' style='padding: 0 .7em;'> "
        "<p><span class='ui-icon ui-icon-alert' style='float: left; margin-right: .3em;'></span> "
        "<strong>Door Open!</strong></p> "
        "</div> "
        "</div>"
        "<div id='leftclosed' onmousedown='javascript:status(); class='ui-widget' style='display:none'> "
        "<div class='ui-state-highlight ui-corner-all' style='padding: 0 .7em;'> "
        "<p><span class='ui-icon ui-icon-info' style='float: left; margin-right: .3em;'></span> "
        "<strong>Door Closed.</strong></p> "
        "</div> "
        "</div>"
        "<div id='leftunknown' onmousedown='javascript:status(); class='ui-widget' style='display:none'> "
        "<div class='ui-state-error ui-corner-all' style='padding: 0 .7em;'> "
        "<p><span class='ui-icon ui-icon-alert' style='float: left; margin-right: .3em;'></span> "
        "<strong>Door Partially Open!</strong></p> "
        "</div> "
        "</div>";
      server.printP(OneDoorStatus);
    } 
    else
      //show 2 door buttons and controls **********************************************************************************************************************************
    {
      P(statusbuttonsTWO)=
        "<button style='width:130px;height:40px;display:inline' id='opendoor' onclick='javascript:OpenDoor();'>Open Left</button><button style='width:130px;height:40px;display:inline' id='opendoor2' onclick='javascript:OpenDoor2();'>Open Right</button><br />"
        "<button style='width:130px;height:40px;margin-top:4px;display:inline' id='closedoor' onclick='javascript:CloseDoor();'>Close Left </button><button style='width:130px;height:40px;margin-top:4px;display:inline' id='closedoor2' onclick='javascript:CloseDoor2();'>Close Right</button><br />"
        "<button style=width:130px;height:40px;margin-top:4px;display:inline' id='actuate' onclick='javascript:ActuateDoor();'>Run Left </button><button style='width:130px;height:40px;margin-top:4px;display:inline' id='actuate2' onclick='javascript:ActuateDoor2();'>Run Right</button>"
        "<br /><br /><hr /><br />";
      server.printP(statusbuttonsTWO);
      //Left-Only Door Status
      P(StatusLeft) =
        "<div id='leftopen' onmousedown='javascript:status(); class='ui-widget' style='display:none'> "
        "<div class='ui-state-error ui-corner-all' style='padding: 0 .7em;'> "
        "<p><span class='ui-icon ui-icon-alert' style='float: left; margin-right: .3em;'></span> "
        "<strong>Left Door Open!</strong></p> "
        "</div> "
        "</div>" 

        "<div id='leftclosed' onmousedown='javascript:status(); class='ui-widget' style='display:none'> "
        "<div class='ui-state-highlight ui-corner-all' style='padding: 0 .7em;'> "
        "<p><span class='ui-icon ui-icon-info' style='float: left; margin-right: .3em;'></span> "
        "<strong>Left Door Closed.</strong></p> "
        "</div> "
        "</div>"

        "<div id='leftunknown' onmousedown='javascript:status(); class='ui-widget' style='display:none'> "
        "<div class='ui-state-error ui-corner-all' style='padding: 0 .7em;'> "
        "<p><span class='ui-icon ui-icon-alert' style='float: left; margin-right: .3em;'></span> "
        "<strong>Left Door Partially Open!</strong></p> "
        "</div> "
        "</div>"; 
      server.printP(StatusLeft);

      //RIGHT DOOR STATUS
      P(StatusRight) =
        "<br />"
        "<div id='rightopen' onmousedown='javascript:status(); class='ui-widget' style='display:none'> "
        "<div class='ui-state-error ui-corner-all' style='padding: 0 .7em;'> "
        "<p><span class='ui-icon ui-icon-alert' style='float: left; margin-right: .3em;'></span> "
        "<strong>Right Door Open!</strong></p> "
        "</div> "
        "</div>"

        "<div id='rightclosed' onmousedown='javascript:status(); class='ui-widget' style='display:none'> "
        "<div class='ui-state-highlight ui-corner-all' style='padding: 0 .7em;'> "
        "<p><span class='ui-icon ui-icon-info' style='float: left; margin-right: .3em;'></span> "
        "<strong>Right Door Closed.</strong></p> "
        "</div> "
        "</div>"

        "<div id='rightunknown' onmousedown='javascript:status(); class='ui-widget' style='display:none'> "
        "<div class='ui-state-error ui-corner-all' style='padding: 0 .7em;'> "
        "<p><span class='ui-icon ui-icon-alert' style='float: left; margin-right: .3em;'></span> "
        "<strong>Right Door Partially Open!</strong></p> "
        "</div> "
        "</div>";
      server.printP(StatusRight);
    }
    P(messageEnd) =

      "</div>"
      "<div id='tabs-2'>"
      "<div class='tweet'></div>"
      "</div>"
      "</div>"
      "</div>"
      "</div>"
      "</body>"
      "</html>";
    server.printP(messageEnd);
  }
}
//*****************************END OF DEFAULT WEB SERVER CMD************************************

void EthernetSetup(){
  // setup the Ethernet library to talk to the Wiznet board
  Ethernet.begin(mac, ip, gateway, subnet);
  /* register our default command (activated with the request of
   * http://x.x.x.x/door */
  webserver.setDefaultCommand(&doorCmd);
  /* start the server to wait for connections */
  webserver.begin();
}
#endif
