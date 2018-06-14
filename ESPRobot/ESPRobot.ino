#include <Event.h>

/*
  This a simple example of the aREST Library for the ESP8266 WiFi chip.
  See the README file for more details.

  Written in 2015 by Marco Schwartz under a GPL license.
*/

// Import required libraries
#include <ESP8266WiFi.h>
#include <Timer.h>
#define AREST_BUFFER_SIZE 5500
#include "aREST.h"
Timer tim;
Timer M1;
Timer M2;
Timer M3;
Timer M4;
// Create aREST instance
aREST rest = aREST();

// WiFi parameters
const char* ssid = "LaurelleWireless";
const char* password = "LaMaisonDuBonheur";

// The port to listen for incoming TCP connections
#define LISTEN_PORT           80

// Create an instance of the server
WiFiServer server(LISTEN_PORT);

// Variables to be exposed to the API
// Declare functions to be exposed to the API
int ledControl(String command);
int mo1 = 0;
int mo2 = 0;
int mo3 = 0;
int mo4 = 0;
int tickEvent1;
int tickEvent2;
int tickEvent3;
int tickEvent4;

void setup(void)
{

  // Start Serial
  Serial.begin(115200);
  pinMode(D0, OUTPUT);
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D3, OUTPUT);
  pinMode(D4, OUTPUT);
  pinMode(D5, OUTPUT);
  pinMode(D6, OUTPUT);
  pinMode(D7, OUTPUT);
  pinMode(D8, OUTPUT);
  pinMode(D9, OUTPUT);

  // Init variables and expose them to REST API
  // Function to be exposed
  rest.function("led", ledControl);
  rest.function("MU", MUp);
  rest.function("MD", MDown);
  rest.function("MS", MStop);
  rest.function("RS", Reset);
  // page to be returned

  rest.page("html", "text/html", Html);
  rest.page("graph.svg", "image/svg+xml", drawGraph);
  rest.page("script", "text/javascript", script);

  // Give name & ID to the device (ID should be 6 characters long)
  rest.set_id("1");
  rest.set_name("esp8266");

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  // Start the server
  server.begin();
  Serial.println("Server started");

  // Print the IP address
  Serial.println(WiFi.localIP());


}

void loop() {
  tim.update();

  M1.update();
  M2.update();
  M3.update();
  M4.update();
  // Handle REST calls
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  while (!client.available()) {
    delay(1);
  }
  rest.handle(client);

}

String Html(String command) {
  String ret = "<html><head><meta charset='UTF-8'><title>Super robot</title><script src='script'></script></head><body><h1>Le bras robotisé</h1>Cliques sur les flèches pour bouger le bras<br><br>"; 
  ret += "<input onclick=\"btn('RS?p=1')\" type=\"button\" value=\"Reset position M1\"><br></br>";
  ret += "<input onclick=\"btn('RS?p=2')\" type=\"button\" value=\"Reset position M2\"><br></br>";
  ret += "<input onclick=\"btn('RS?p=3')\" type=\"button\" value=\"Reset position M3\"><br></br>";
  ret += "<input onclick=\"btn('RS?p=4')\" type=\"button\" value=\"Reset position M4\"><br>";
  ret += "<object data=\"graph.svg\" type=\"image/svg+xml\"></object>";
  ret += "</body></html>";
  return ret;
}

String script(String command) {
  String ret = "// <![CDATA[\r\
var xhr = new XMLHttpRequest();\
function btn(cmdSend) {\
xhr.open('GET', cmdSend + '&_=' + Math.random());\
xhr.send();\
}\r\
// ]]>";
  return ret;
}
// Custom function accessible by the API
int ledControl(String command) {

  // Get state from command
  int state = command.toInt();

  digitalWrite(6, state);
  return 1;
}

String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = { 0, -1 };
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

int MUp(String command) {
  int state = command.toInt();

  if (state == 1) {
    if (mo1 < 7100) {
      M1.stop(tickEvent1);
      tickEvent1 = M1.every(10, m, (void*)1);
      digitalWrite(D0, HIGH);
      digitalWrite(D1, LOW);
    }
  }
  if (state == 2) {
    if (mo2 < 5850) {
      M2.stop(tickEvent2);
      tickEvent2 = M2.every(10, m, (void*)3);
      digitalWrite(D2, HIGH);
      digitalWrite(D3, LOW);
    }
  }
  if (state == 3) {
    if (mo3 < 6800) {
      M3.stop(tickEvent3);
      tickEvent3 = M3.every(10, m, (void*)5);
      digitalWrite(D4, HIGH);
      digitalWrite(D5, LOW);
    }
  }
  if (state == 4) {
    if (mo4 < 4100) {
      M4.stop(tickEvent4);
      tickEvent4 = M4.every(10, m, (void*)7);
      digitalWrite(D6, HIGH);
      digitalWrite(D7, LOW);
    }
  }
  if (state == 5) {
    digitalWrite(D8, HIGH);
    digitalWrite(D9, LOW);
    tim.after(500, StopMotor, (void*)state);
}
}


int MDown(String command) {
  int state = command.toInt();

  if (state == 1) {
    if (mo1 > -7100) {
      M1.stop(tickEvent1);
      tickEvent1 = M1.every(10, m, (void*)2);
      digitalWrite(D0, LOW);
      digitalWrite(D1, HIGH);
    }
  }
  if (state == 2) {
    if (mo2 > -5850) {
      M2.stop(tickEvent2);
      tickEvent2 = M2.every(10, m, (void*)4);
      digitalWrite(D2, LOW);
      digitalWrite(D3, HIGH);
    }
  }
  if (state == 3) {
    if (mo3 > -6800) {
      M3.stop(tickEvent3);
      tickEvent3 = M3.every(10, m, (void*)6);
      digitalWrite(D4, LOW);
      digitalWrite(D5, HIGH);
    }
  }
  if (state == 4) {
    if (mo4 > -3600) {
      M4.stop(tickEvent4);
      tickEvent4 = M4.every(10, m, (void*)8);
      digitalWrite(D6, LOW);
      digitalWrite(D7, HIGH);
    }
  }

  if (state == 5) {
    digitalWrite(D8, LOW);
    digitalWrite(D9, HIGH);
    tim.after(500, StopMotor, (void*)state);
  }
}



void StopMotor(void* number) {
  int state = (int)number;
  MStop(String(state));
}

int MStop(String command) {
  if (command == "")
  {
    digitalWrite(D0, LOW);
    digitalWrite(D1, LOW);
    digitalWrite(D2, LOW);
    digitalWrite(D3, LOW);
    digitalWrite(D4, LOW);
    digitalWrite(D5, LOW);
    digitalWrite(D6, LOW);
    digitalWrite(D7, LOW);
    digitalWrite(D8, LOW);
    digitalWrite(D9, LOW);
  }
  else
  {
    int state = command.toInt();
    if (state == 1) {

      M1.stop(tickEvent1);
      digitalWrite(D0, LOW);
      digitalWrite(D1, LOW);
    }
    if (state == 2) {

      M2.stop(tickEvent2);
      digitalWrite(D2, LOW);
      digitalWrite(D3, LOW);
    }

    if (state == 3) {

      M3.stop(tickEvent3);
      digitalWrite(D4, LOW);
      digitalWrite(D5, LOW);
    }
    if (state == 4) {

      M4.stop(tickEvent4);
      digitalWrite(D6, LOW);
      digitalWrite(D7, LOW);
    }

    if (state == 5) {
      digitalWrite(D8, LOW);
      digitalWrite(D9, LOW);
    }
    if (state == 6) {
      digitalWrite(D4, LOW);
      digitalWrite(D5, LOW);
      digitalWrite(D2, LOW);
      digitalWrite(D3, LOW);
    }
    if (state == 7) {
      digitalWrite(D6, LOW);
      digitalWrite(D7, LOW);
      digitalWrite(D4, LOW);
      digitalWrite(D5, LOW);
    }
  }
}

void m(void* motor) {  int state = (int)motor;
  
  if (state == 1) {
    Serial.println(mo1);
    mo1 += 10;
    if (mo1 > 7000) {
      MStop(String(1));
    }
  }

  if (state == 2) {
    Serial.println(mo1);
    mo1 -= 10;
    if (mo1 < -7000) {
      MStop(String(1));
    }
  }

  if (state == 3) {
    Serial.println(mo2);
    mo2 += 10;
    if (mo2 > 5750) {
      MStop(String(2));
    }
  }

  if (state == 4) {
    Serial.println(mo2);
    mo2 -= 10;
    if (mo2 < -5750) {
      MStop(String(2));
    }
  }

  if (state == 5) {
    Serial.println(mo3);
    mo3 += 10;

    if (mo3 > 6700) {
      MStop(String(3));
    }
  }

  if (state == 6) {

    Serial.println(mo3);
    mo3 -= 10;
    if (mo3 < -6700) {
      MStop(String(3));
    }
  }
  if (state == 7) {

    Serial.println(mo4);
    mo4 += 10;

    if (mo4 > 3500) {
      MStop(String(4));
    }
  }

  if (state == 8) {

    Serial.println(mo4);
    mo4 -= 10;
    if (mo4 < -3500) {   
      MStop(String(4));
    }
  }
}

int Reset(String command) {
  int state = command.toInt();
  if (state == 1) {
    M1.stop(tickEvent1);
    mo1 = 0;
  }
  if (state == 2) {
    M2.stop(tickEvent2);
    mo2 = 0;
  }
  if (state == 3) {
    M3.stop(tickEvent3);
    mo3 = 0;
  }
  if (state == 4) {
    M4.stop(tickEvent4);
    mo4 = 0;
  }
}



String drawGraph(String command) {

  String ret = "<svg width=\"640\" height=\"480\" xmlns=\"http://www.w3.org/2000/svg\" xmlns:svg=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\">\
<defs>\
<marker orient=\"auto\" markerHeight=\"5\" markerWidth=\"5\" markerUnits=\"strokeWidth\" refY=\"50\" refX=\"50\" viewBox=\"0 0 100 100\" id=\"se\">\
<path d=\"m100,50l-100,40l30,-40l-30,-40l100,40z\" stroke-width=\"10\" stroke=\"#000000\" fill=\"#000000\" id=\"s12\"/>\
</marker>\
</defs>\
<g>\
<line y2=\"155\" x2=\"323\" y1=\"389\" x1=\"323\" stroke-width=\"10\" stroke=\"#000000\" id=\"s2\"/>\
<rect height=\"64\" width=\"156\" y=\"390\" x=\"245\" stroke-width=\"5\" stroke=\"#000000\" fill=\"#ff0000\" id=\"s1\" onclick=\"btn('MS?p=1')\"/>\
<circle r=\"14\" cy=\"374\" cx=\"323\" stroke-width=\"8\" stroke=\"#000000\" fill=\"#ff0000\" id=\"s6\" onclick=\"btn('MS?p=2')\"/>\
<circle r=\"14\" cy=\"220\" cx=\"323\" stroke-width=\"8\" stroke=\"#000000\" fill=\"#ff0000\" id=\"s7\" onclick=\"btn('MS?p=4')\"/>\
<circle r=\"14\" cy=\"297\" cx=\"323\" stroke-width=\"8\" stroke=\"#000000\" fill=\"#ff0000\" id=\"s8\" onclick=\"btn('MS?p=3')\"/>\
<line y2=\"100\" x2=\"292\" y1=\"156\" x1=\"322\" stroke-width=\"5\" stroke=\"#000000\" id=\"s9\"/>\
<line y2=\"100\" x2=\"318\" y1=\"156\" x1=\"348\" transform=\"rotate(45 333 126)\" stroke-width=\"5\" stroke=\"#000000\" id=\"s10\"/>\
<line y2=\"337\" x2=\"445\" y1=\"337\" x1=\"357\" stroke-width=\"8\" stroke=\"#000000\" marker-end=\"url('#se')\" id=\"s3\" onclick=\"btn('MU?p=2')\"/>\
<line y2=\"262\" x2=\"445\" y1=\"262\" x1=\"357\" stroke-width=\"8\" stroke=\"#000000\" marker-end=\"url('#se')\" id=\"s4\" onclick=\"btn('MU?p=3')\"/>\
<line y2=\"178\" x2=\"445\" y1=\"178\" x1=\"357\" stroke-width=\"8\" stroke=\"#000000\" marker-end=\"url('#se')\" id=\"s5\" onclick=\"btn('MU?p=4')\"/>\
<line y2=\"422\" x2=\"504\" y1=\"422\" x1=\"416\" stroke-width=\"8\" stroke=\"#000000\" marker-end=\"url('#se')\" id=\"s11\" onclick=\"btn('MU?p=1')\"/>\
<line y2=\"337\" x2=\"288\" y1=\"337\" x1=\"200\" transform=\"rotate(180 244.5 337)\" stroke-width=\"8\" stroke=\"#000000\" marker-end=\"url('#se')\" id=\"s13\" onclick=\"btn('MD?p=2')\"/>\
<line y2=\"262\" x2=\"284\" y1=\"262\" x1=\"196\" transform=\"rotate(180 240.5 262)\" stroke-width=\"8\" stroke=\"#000000\" marker-end=\"url('#se')\" id=\"s14\" onclick=\"btn('MD?p=3')\"/>\
<line y2=\"178\" x2=\"286\" y1=\"178\" x1=\"198\" transform=\"rotate(180 242.5 178)\" stroke-width=\"8\" stroke=\"#000000\" marker-end=\"url('#se')\" id=\"s15\" onclick=\"btn('MD?p=4')\"/>\
<line y2=\"422\" x2=\"226\" y1=\"422\" x1=\"138\" transform=\"rotate(180 182.5 422)\" stroke-width=\"8\" stroke=\"#000000\" marker-end=\"url('#se')\" id=\"s16\" onclick=\"btn('MD?p=1')\"/>\
<line y2=\"99\" x2=\"442\" y1=\"99\" x1=\"354\" stroke-width=\"8\" stroke=\"#000000\" marker-end=\"url('#se')\" id=\"s17\" onclick=\"btn('MU?p=5&amp;t=5')\"/>\
<line y2=\"99\" x2=\"283\" y1=\"99\" x1=\"195\" transform=\"rotate(180 239 99)\" stroke-width=\"8\" stroke=\"#000000\" marker-end=\"url('#se')\" id=\"s18\" onclick=\"btn('MD?p=5&amp;t=5')\"/>\
<script xlink:href=\"script\" />\
</g>\
</svg>";

  return ret;
}

