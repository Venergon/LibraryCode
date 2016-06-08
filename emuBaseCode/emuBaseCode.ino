/* Base EMU Robot Code
 * Base code for getting data to write to motors
 * Add your own code to make the robot move
 */



#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
//#include "net.h"
#include "motors.h"

/*const char* ssid = "Bowen-Library"; //The wifi network to connect to
const char* password = ""; //The password for that network
const char *serverID = "192.168.89.146"; //The IP of the server
int x;
int y;*/
int mult = 8;




// the setup routine runs once when you press reset:
void setup() {
  //netSetup(ssid, password);
  motorSetup();
  /*x = 0;
  y = 0;*/
}

// the loop routine runs over and over again forever:
void loop() {
  /*xyValues values = netLoop(x, y, serverID);
  x = values.x;
  y = values.y;*/
  leftMotor(1024, mult);
  rightMotor(1024, mult);
}





