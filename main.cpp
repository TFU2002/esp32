

// Import required libraries
#include "WiFi.h"
//#include "ESPAsyncWebServer.h"
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <WebServer.h>
//#include <ESPAsyncTCP.h>
//#include <ArduinoJson.h>
#include <Arduino.h>
// Replace with your network credentials

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta http-equiv="X-UA-Compatible" content="IE=edge">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>ESP32</title>
    <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.7.2/css/all.css" integrity="sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr" crossorigin="anonymous">
    <link rel="icon" href="data:,">
    <style>

      html {font-family: Arial; display: inline-block; text-align: center;}
      p {font-size: 1.2rem;}
      body {margin: 0;}
      .logo {overflow: hidden; background-color: #6C0BA9; color: white; font-size: 1.7rem;}
      .container {padding: 20px; }
      .card {background-color: white; box-shadow: 0px 0px 10px 1px rgba(140,140,140,.5); border: 1px solid #6C0BA9; border-radius: 15px;}
      .card.header {background-color: #6C0BA9; color: white; border-bottom-right-radius: 0px; border-bottom-left-radius: 0px; border-top-right-radius: 12px; border-top-left-radius: 12px;}
      .cards {max-width: 700px; margin: 0 auto; display: grid; grid-gap: 2rem; grid-template-columns: repeat(auto-fit, minmax(300px, 1fr));}
      .reading {font-size: 2.8rem;}
      .packet {color: #bebebe;}
      .temperatureColor {color: #fd7e14;}
      .humidityColor {color: #1b78e2;}
      .moisureColor {color: #1b78e2;}
      .statusreadColor {color: #183153; font-size:15px;}
    </style>
</head>
<body>
  <div class="logo">
     <h2>ESP32 DHT11</h2>
  </div>
  <br>
  <div class="container">
    <div class="cards">
      <div class="card">
        <div class="cardheader">
          <h2>ESP32 1</h2>
        </div>
        <br>
          <h4 class="temperature"><i class="fas fa-thermometer-half"></i> TEMPERATURE</h4>
          <p class="temperature"><span class="reading"><span id="temp1"></span> &deg;C</span></p>
          <h4 class="humidityColor"><i class="fas fa-tint"></i> HUMIDITY</h4>
          <p class="humidityColor"><span class="reading"><span id="humd1"></span> &percnt;</span></p>
          <h4 class="moisureColor"><i class="fas fa-tint"></i> Moisure</h4>
          <p class="moisureColor"><span class="reading"><span id="moi1"></span> &percnt;</span></p>
          <p class="statusreadColor"><span>Pumper: </span><span id="status_read_DHT11_ESP32_1">%BUTTONPLACEHOLDER%</span></p>
          <p class="statusreadColor" ><span>Last time to receive data : </span><span id="LTRD_ESP32_1" ></span></p>
      </div>

    </div>
  </div>
</body>
</html>
<script>
  setInterval(function () {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
        console.log("hello")
      document.getElementById("temp1").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/temperature", true);
  xhttp.send();
}, 1000 ) ;

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("humd1").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/humidity", true);
  xhttp.send();
}, 1000 ) ;

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("moi1").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/moisure", true);
  xhttp.send();
}, 1000 ) ;
function pump(){
  //ON
  if(document.getElementById("pump_status").innerHTML == "OFF"){
    var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("pump_status").innerHTML = "ON";
    }
  };
  xhttp.open("GET", "/pumper", true);
  xhttp.send();
  //document.getElementById("pump_status").innerHTML = "ON";
 // document.getElementById("pump_status").innerHTML = "ON";
 
  } else {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("pump_status").innerHTML = "OFF";
    }
  };
  xhttp.open("GET", "/pumper", true);
  xhttp.send(); 
  //document.getElementById("pump_status").innerHTML = "OFF";
  }
  // document.getElementById("pump_status").innerHTML = "OFF";
  
}
 setInterval(function () {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("pump_status").innerHTML = this.responseText;
    }
  };
   xhttp.open("GET", "/getpump_status", true);
   xhttp.send();
}, 1000 ) ;

window.addEventListener("load",function clock(){

var today = new Date();
var time = leading_zeros(today.getHours()) + ":" + leading_zeros(today.getMinutes()) + ":" + leading_zeros(today.getSeconds());



function leading_zeros(x) { 
        return (x < 10 ? '0' : '') + x;
      }
  document.getElementById("LTRD_ESP32_1").innerHTML = time ;
  setTimeout(clock, 1000);
});
</script>
)rawliteral";

const char* ssid = "ESP32-Access-Point";
const char* password = "123456789";
///////////////////////////////////
IPAddress local_IP(192, 168, 4, 1);
IPAddress gateway(192, 168, 4, 1);
IPAddress subnet(255, 255, 255, 0);
WebServer server(80);

#define DHTPIN 27     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11   // DHT 11
DHT dht(DHTPIN, DHTTYPE);
#define moisure 26
#define LED 2
String readmoisure()
{
  int moisurevalue = analogRead(moisure);
  int outputvalue = map(moisurevalue, 0, 1023, 255, 0);
  return String(outputvalue);
}



String readDHTTemperature() {
  float t = dht.readTemperature();
  if (isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return "Failed to read from DHT sensor!";
  }
  else {
    Serial.print("Temperature: ");
    Serial.println(t);
    return String(t);
  }
}

String readDHTHumidity() {
  float h = dht.readHumidity();
  if (isnan(h)) {
    Serial.println("Failed to read from DHT sensor!");
    return "Failed to read from DHT sensor!";
  }
  else {
    Serial.print("Humidity: ");
    Serial.println(h);
    return String(h);
  }
}

void handle_OnConnect() {
    server.send(200,"text/html",index_html);
}
void handel_temperature(){
server.send(200, "text/plain", readDHTTemperature());
}
void handel_humidity(){
server.send(200, "text/plain", readDHTHumidity());
}
void handel_moisure(){
  server.send(200,"text/plain", readmoisure());
}

void handel_pumper(){
  //server.send(200,"text/plain", readmoisure());
 if(digitalRead(LED) == LOW){
    digitalWrite(LED, HIGH);
    server.send(200,"text/plain", "ON");
  }
  else{
    digitalWrite(LED, LOW);
    server.send(200,"text/plain", "OFF");
  }
  
}

void handel_pumperstatus(){
  if(digitalRead(LED) == LOW){
    server.send(200,"text/plain", "OFF");
  }
  else{
    server.send(200,"text/plain", "ON");
  }
}

void setup(){
  Serial.begin(115200);
  Serial.print(local_IP);
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_IP, gateway, subnet);
 // Serial.println(WiFi.softAPIP());
  delay(100);
  server.on("/", handle_OnConnect);
  server.on("/temperature", handel_temperature);
  server.on("/hudimity", handel_humidity);
  server.on("/hudimity", handel_moisure);
  server.on("/pumper", handel_pumper);
  server.on("/getpump_status", handel_pumperstatus);
  server.begin();
  Serial.println("Server started");
}
 
void loop(){
  //server.handleClient();
  
}