const prog_uchar http_404header[] PROGMEM = "HTTP/1.1 404 Not Found\nServer: arduino\nContent-Type: text/html\n\n<html><head><title>404</title></head><body><h1>404: Sorry, that page cannot be found!</h1></body>";
const  prog_uchar httpHeader[] PROGMEM = "HTTP/1.0 200 OK\nServer: arduino\nContent-Type: text/html\n\n";
const  prog_uchar htmlHeader[] PROGMEM = "<!DOCTYPE html>"
      "<html>"
      "<head>"
      "<title>Web Garage Door</title>"
      "<meta name='viewport' content='width=device-width; initial-scale=1.0; maximum-scale=1.0; user-scalable=0;' />"
      "<meta name=apple-mobile-web-app-capable' content='yes'>"
      "<script type='text/javascript' src='http://ajax.googleapis.com/ajax/libs/jquery/1.4.2/jquery.min.js'></script>"
      "<script type='text/javascript' src='http://ajax.googleapis.com/ajax/libs/jqueryui/1.8.2/jquery-ui.min.js'></script>"
      "<script type='text/javascript' src='http://fgnass.github.io/spin.js/spin.js'></script>"
      "<link rel='stylesheet' href='http://ajax.googleapis.com/ajax/libs/jqueryui/1.8.2/themes/dot-luv/jquery-ui.css' />"
      "<script type='text/javascript'>var doors=";
const  prog_uchar messageEnd[] PROGMEM = ";</script>"
      "<script src='https://dl.dropbox.com/u/29513279/main.js' type='text/javascript'></script>"
      "<style type='text/css'>"
      "#slider{margin: 10px;}"
      ".body{vertical-align: top;text-align: center;color: White;margin: 0 auto;border: 1px solid white;}"
      "#wrapper{width: 305px;height: 400px;border: none;margin: 0 auto;}"
      "</style>"
      "</head>"
      "<body id='page' style='font-size: 62.5%; background-color: #5F5A59;' onload='setTimeout(function() { window.scrollTo(0, 1) }, 100);' >"
      "</body>"
      "</html>";
      
      //"<script type='text/javascript' src='http://fgnass.github.com/spin.js/dist/spin.js'></script>"
