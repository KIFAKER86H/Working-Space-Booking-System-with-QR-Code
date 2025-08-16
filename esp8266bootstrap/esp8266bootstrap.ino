#include <WiFiClient.h>
#include <ESP8266WebServer.h>

#define ssid      "AQ"      // WiFi SSID
#define password  "12345678"  // WiFi password
const uint8_t GPIOPIN[4] = {D5,D6,D7,D8};  // LED Pins
String etatGpio[4] = {"OFF","OFF","OFF","OFF"};
ESP8266WebServer server(80);

String getPage() {
  String page = "<html charset=UTF-8><head><meta name='viewport' content='width=device-width, initial-scale=1'/>";
  page += "<script src='https://ajax.googleapis.com/ajax/libs/jquery/3.1.1/jquery.min.js'></script>";
  page += "<script src='https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/js/bootstrap.min.js'></script>";
  page += "<link rel='stylesheet' href='https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css'>";
  page += "<title>ESP8266 LED Control</title></head><body>";
  page += "<div class='container'>";
  page += "<div class='row'><div class='col-md-12'><h1 class='text-center'>ESP8266 LED Control</h1><hr>";

  for (int i = 0; i < 4; i++) {
    page += "<div class='row'>";
    page += "<div class='col-md-4 col-xs-12 text-center'><h4>D" + String(GPIOPIN[i]) + " ";
    page += "<span class='badge " + String(etatGpio[i] == "ON" ? "badge-success" : "badge-danger") + "'>" + etatGpio[i] + "</span></h4></div>";
    page += "<div class='col-md-4 col-xs-6 text-center'><form action='/' method='POST'><button type='submit' name='D" + String(GPIOPIN[i]) + "' value='1' class='btn btn-success btn-block'>ON</button></form></div>";
    page += "<div class='col-md-4 col-xs-6 text-center'><form action='/' method='POST'><button type='submit' name='D" + String(GPIOPIN[i]) + "' value='0' class='btn btn-danger btn-block'>OFF</button></form></div>";
    page += "</div><br>";
  }

  page += "<hr><footer class='text-center'><p>ESP8266 LED Control © 2024</p></footer>";
  page += "</div></div></body></html>";
  return page;
}

void handleRoot() {
  if (server.hasArg("D5")) {
    handleGPIO(0, server.arg("D5"));
  } else if (server.hasArg("D6")) {
    handleGPIO(1, server.arg("D6"));
  } else if (server.hasArg("D7")) {
    handleGPIO(2, server.arg("D7"));
  } else if (server.hasArg("D8")) {
    handleGPIO(3, server.arg("D8"));
  } else {
    server.send(200, "text/html", getPage());
  }
}

void handleGPIO(int gpio, String value) {
  if (value == "1") {
    digitalWrite(GPIOPIN[gpio], HIGH);
    etatGpio[gpio] = "ON";
  } else if (value == "0") {
    digitalWrite(GPIOPIN[gpio], LOW);
    etatGpio[gpio] = "OFF";
  }
  server.send(200, "text/html", getPage());
}

void setup() {
  for (int i = 0; i < 4; i++) {
    pinMode(GPIOPIN[i], OUTPUT);
  }
  
  Serial.begin(9600);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}
