#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

WiFiServer server(80);


void printWifiStatus();
int parseRequest(char *request, int len);
int getX(char *request, int len);
int getY(char *request, int len);
long myAtoL(char *message, int len);

typedef struct _xyValues {
  int x;
  int y;
} xyValues;

void netSetup(const char *ssid, const char *password) {
  Serial.begin(9600);
  // make the pushbutton's pin an input:
  delay(100);
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid,password);

  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
  printWifiStatus();
}

xyValues netLoop(int defaultX, int defaultY, const char *serverIP) {
  xyValues values;
  values.x = defaultX;
  values.y = defaultY;
  char request[1000] = {0};
  int counter = 0;
  // listen for incoming clients
  WiFiClient client = server.available();
  if (client) {
    Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        if (counter >= 900) {
          Serial.println("TOO MUCH TEXT IN REQUEST");
          break;
        }
        char c = client.read();
        request[counter] = c;
        counter++;
        Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          if(parseRequest(request, counter)) {
              Serial.println("Calculating x");
              values.x = getX(request, counter-1);
              Serial.println("Calculating y");
              values.y = getY(request, counter-1);
              Serial.println("Calculated");
              //Serial.println(values.y);
              //Serial.println(values.x);
              Serial.println("Done");
              break;
          } else {
            // send a standard http response header
            client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: text/html");
            client.println("Connection: close");  // the connection will be closed after completion of the response
            client.println();
            client.println("<html>\n");
            client.println("<head>\n");
            client.println("<meta charset=\"utf-8\">");
            client.println("<meta name=\"viewport\" content=\"width=device-width, user-scalable=no, minimum-scale=1.0, maximum-scale=1.0\">");
            client.println("<style>");
            client.println("#info {\n"
                         "position  : absolute;\n"
                         "top   : 0px;\n"
                         "width   : 100%;\n"
                         "padding   : 5px;\n"
                         "text-align  : center;\n"
                         "}");
            client.println("#container {\n"
                        "width   : 100%;\n"
                        "height    : 100%;\n"
                        "overflow  : hidden;\n"
                        "padding   : 0;\n"
                        "margin    : 0;\n"
                        "-webkit-user-select : none;\n"
                        "-moz-user-select  : none;\n"
                        "}");
            client.println("</style>");
                    
            client.println("</head>");
            client.println("<body>");
          
            client.println("<div id=\"container\"></div>");
            client.println("<div id=\"info\">");
            client.println("<span id=\"result\"></span>");
            client.println("</div>");
            client.println("<div id=\"test\"></div>");
            client.print("<script src=\"http://");
            client.print(serverIP);
            client.println("/javascript/post.js\"></script>");
            client.print("<script src=\"http://");
            client.print(serverIP);
            client.println("/javascript/virtualjoystick.js\"></script>");
            client.println("<script>\n"
                "console.log(\"touchscreen is\", VirtualJoystick.touchScreenAvailable() ? \"available\" : \"not available\");\n"
                "var joystick  = new VirtualJoystick({\n"
                "container : document.getElementById('container'),\n"
                "mouseSupport  : true,\n"
                 "});\n"
                "joystick.addEventListener('touchStart', function(){\n"
                "console.log('down')\n"
                "})\n"
                "joystick.addEventListener('touchEnd', function(){\n"
                "console.log('up')\n"
                "})\n"

                "setInterval(function(){\n"
                "console.log(loadDoc());\n"
                "var outputEl  = document.getElementById('result');\n"
                "outputEl.innerHTML  = '<b>Result:</b> '\n"
                "+ ' dx:'+joystick.deltaX()\n"
                "+ ' dy:'+joystick.deltaY()\n"
                "+ (joystick.right() ? ' right'  : '')\n"
                "+  (joystick.up()  ? ' up'   : '')\n"
                "+ (joystick.left()  ? ' left' : '')\n"
                "+ (joystick.down()  ? ' down'   : '') \n"
                "}, 1/30 * 30000);\n"
                "</script>\n"
                "</body>\n"
                "</html>");
            break;
            }
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        }
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      } else {
        //Serial.println("Client not available");
        client.stop();
      }
    }
    // give the web browser time to receive the data
    delay(1);
   
    // close the connection:
    Serial.println("Closing connection");
    client.stop();
    Serial.println("Connection Closed");
    //Serial.println("client disconnected");
  }
  return values;
}


int parseRequest(char *request, int len) {
  if (len < 4) {
    return false;
  } else if (request[0] == 'P' && request[1] == 'O' && request[2] == 'S' && request[3] == 'T') {
    return true;
  }else {
    return false;
  }
}

int getX(char *request, int len) {
  int counter = 0;
  long returnValue;
  while (request[counter] != 'x' && request[counter != '\0'] && counter < len-10) {
    counter++;
  }
  //Serial.println(counter);
  if (request[counter] == 'x') {
    returnValue = myAtoL(&request[counter+2], len-(counter+3));
  } else {
    returnValue = -1;
  }
  return returnValue;
}


int getY(char *request, int len) {
  int counter = 0;
  long returnValue;
  while (request[counter] != 'y' && request[counter != '\0'] && counter < len-10) {
    Serial.println("Iterating on getY");
    counter++;
  }
  //Serial.println(counter);
  if (request[counter] == 'y') {
    Serial.println("Sent to myAtoL");
    returnValue = myAtoL(&request[counter+2], len-(counter+3));
  } else {
    returnValue = -1;
  }
  return returnValue;
}

long myAtoL(char *message, int len) {
  long num = 0;
  int negative = false;
  int currentPosition = 0;
  if (message[currentPosition] == '-'){
    negative = true;
    currentPosition++;
  }
  while (message[currentPosition] >= '0' && message[currentPosition] <= '9') {
    int digitValue = message[currentPosition] - '0';
    num *= 10;
    num += digitValue;
    currentPosition++;
    if (currentPosition >= len-10){
      num = 0;
      break;
    }
  }
  if (negative) {
    num *= -1;
  }
  return num;
}

void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
