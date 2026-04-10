#include <Arduino.h>
#include <ESP32Servo.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <UniversalTelegramBot.h>
bool should=false;
bool s=false;
int angle;
int mode=0;


Servo myServo;

#define BOTtoken "8736106243:AAGrqLYHwCOgMkVhuGf2SF3xWbIZ8ciPbMw"   
#define CHAT_ID "7960746015"     


WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

unsigned long lastTimeChecked;  
const int checkInterval = 2000;

const char* ssid = "Anishg54";
const char* password = "anish123";
int led = 18;
int trig = 4;
int echo = 16;
int lasser = 15;
void sendTelegramMessage(String message) {
  bot.sendMessage(CHAT_ID, message, "");
}


WebServer server(80);

void handleRoot() {
  server.send(200, "text/html", R"rawliteral(<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
<title>Radar System</title>

<style>
    body {
        background-color: black;
        margin: 0;
        font-family: Arial, sans-serif;
        display: flex;
        justify-content: center;
        align-items: center;
        height: 100vh;
        flex-direction: column;
    }

    h1 {
        color: white;
        margin-bottom: 40px;
        letter-spacing: 3px;
    }

    .buttons {
        display: flex;
        gap: 20px;
    }

    button {
        background-color: #6a0dad; /* lavish purple */
        color: red;
        border: none;
        padding: 15px 25px;
        font-size: 16px;
        border-radius: 10px;
        cursor: pointer;
        transition: 0.3s;
    }

    button:hover {
        transform: scale(1.1);
        background-color: #8a2be2;
    }
</style>
</head>

<body>

<h1>RADAR SYSTEM</h1>

<div class="buttons">
    <a href="http://10.50.133.154/h">
  <button>Rotate 90 degree</button>
</a>
    <a href="http://10.50.133.154/f">
  <button>Rotate 180 degree</button>
</a>
    <button onclick="scan()">Scanning</button>
</div>

<script>
    function rotateRight() {
        alert("Radar rotating right!");
    }

    function rotate180() {
        alert("Radar rotating 180 degrees!");
    }

    function scan() {
        alert("Radar scanning...");
    }
</script>

</body>
</html>
)rawliteral");
}
void handleH(){
  server.send(200,"text/html",R"rawliteral(<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
<title>Static Rotation</title>

<style>
    body {
        background-color: black;
        margin: 0;
        height: 100vh;
        display: flex;
        justify-content: center;
        align-items: center;
    }

    h1 {
        color: red;
        font-size: 60px;
        
    }
</style>
</head>

<body>

<h1>ROTATING 90°</h1>

</body>
</html>


)rawliteral");
mode=1;
for (angle = 0; angle <= 90; angle += 20) {
  myServo.write(angle);
  delay(500);

  // Trigger ultrasonic
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  // Measure echo
  long time = pulseIn(echo, HIGH, 30000);
  float dis = (0.0343 * time) / 2;  // Correct formula

  Serial.print("Angle: ");
  Serial.print(angle);
  Serial.print(" | Time: ");
  Serial.print(time);
  Serial.print(" µs | Distance: ");
  Serial.print(dis);
  Serial.println(" cm");

  if (dis < 20 && angle <= 90) {// removing angle
    digitalWrite(led, HIGH);
    digitalWrite(lasser, HIGH);
    String msg = "Object detected at north west " + String(dis) + " cm";
  
    sendTelegramMessage(msg);
    Serial.println(msg);
  } 
  else if (dis < 25 && angle >= 90) {  
    digitalWrite(led, HIGH);
    digitalWrite(lasser, HIGH);
    
    String msg = "Object detected at south west " + String(dis) + " cm";
    sendTelegramMessage(msg);
    Serial.println(msg);
  } 
  else {
    digitalWrite(led, LOW);
    digitalWrite(lasser, LOW);
  }
}
}
void handleF(){
  server.send(200,"text/html",R"rawliteral(<!DOCTYPE html>

<html lang="en">
<head>
<meta charset="UTF-8">
<title>Static Rotation</title>

<style>
    body {
        background-color: black;
        margin: 0;
        height: 100vh;
        display: flex;
        justify-content: center;
        align-items: center;
    }

    h1 {
        color: red;
        font-size: 60px;
        
    }
</style>
</head>

<body>

<h1>ROTATING 180°</h1>

</body>
</html>
  )rawliteral");
  mode=2;
for ( angle = 0; angle <= 180; angle += 30) {
  myServo.write(angle);
  delay(500);

  // Trigger ultrasonic
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  // Measure echo
  long time = pulseIn(echo, HIGH, 30000);
  float dis = (0.0343 * time) / 2;  //  formula

  Serial.print("Angle: ");
  Serial.print(angle);
  Serial.print(" | Time: ");
  Serial.print(time);
  Serial.print(" µs | Distance: ");
  Serial.print(dis);
  Serial.println(" cm");

  if (dis < 20 && angle <= 90) {
    digitalWrite(led, HIGH);
    digitalWrite(lasser, HIGH);
    String msg = "Object detected at north west " + String(dis) + " cm";
    sendTelegramMessage(msg);
    Serial.println(msg);
  } 
  else if (dis < 25 && angle >= 90) {  
    digitalWrite(led, HIGH);
    digitalWrite(lasser, HIGH);
    String msg = "Object detected at south west " + String(dis) + " cm";
    sendTelegramMessage(msg);
    Serial.println(msg);
  } 
  else {
    digitalWrite(led, LOW);
    digitalWrite(lasser, LOW);
  }
}
}


// Function to send Telegram message


void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  client.setCACert(TELEGRAM_CERTIFICATE_ROOT);

  Serial.println("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected!");
  Serial.print("ESP32 IP address: ");
  Serial.println(WiFi.localIP());

  bot.sendMessage(CHAT_ID, "Elconics BOT started up", "");

  if (MDNS.begin("anish")) {                                                            
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);
  server.on("/h",handleH);
  server.on("/f",handleF);
  server.begin();                 

  myServo.attach(19);
  pinMode(led, OUTPUT);                             
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  pinMode(lasser, OUTPUT);    
                                                                                                                                                      Serial.print(should);                
}

void loop() {
  server.handleClient(); // Keeps the web server alive
 
  if (mode == 1) {
    // Continuous 90° sweep
    for (angle = 0; angle <= 90; angle += 20) {
      myServo.write(angle);
      handleH();
      delay(500);
    }
    for (angle = 90; angle >= 0; angle -= 20) {
      myServo.write(angle);
    
      delay(500);
    }
  }

  else if (mode == 2) {
    // Continuous 180° sweep
    for (angle = 0; angle <= 180; angle += 20) {
      myServo.write(angle);
      handleF();
      delay(500);
    }
    for (angle = 180; angle >= 0; angle -= 20) {
      myServo.write(angle);
      delay(500);
    }
  }
}





    
