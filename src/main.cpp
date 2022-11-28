#include <Arduino.h>

#include <espnow.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncWebServer.h>
#include <ESPAsyncTCP.h>
#include <Arduino_JSON.h>
#include <DHT.h>

// set pin
#define led 2
int number = 0;

#define DHTPIN 5 // Digital pin connected to the DHT sensor

#define DHTTYPE DHT11 // DHT 11
//#define DHTTYPE    DHT22     // DHT 22 (AM2302)
//#define DHTTYPE    DHT21     // DHT 21 (AM2301)

DHT dht(DHTPIN, DHTTYPE);
//
// int readDHTTemperature()
//{
////  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
////  // Read temperature as Celsius (the default)
//  float t = dht.readTemperature();
//  // Read temperature as Fahrenheit (isFahrenheit = true)
//  // float t = dht.readTemperature(true);
//  // Check if any reads failed and exit early (to try again).
//  if (isnan(t))
//  {
//    Serial.println("Failed to read from DHT sensor!");
//    return 0.0;
//  }
//  else
//  {
//    Serial.println(t);
//    return int(t);
//  }
//}
//
// String readDHTHumidity()
//{
//  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
//  float h = dht.readHumidity();
//  if (isnan(h))
//  {
//    Serial.println("Failed to read from DHT sensor!");
//    return "==";
//  }
//  else
//  {
//    Serial.println(h);
//    return String(h);
//  }
//}

float Random(float a, float b)
{
  return a + (b - a) * rand() / RAND_MAX;
}

// set wifi interface
const char *ssid = "NNE";
const char *password = "01112002";

IPAddress staticIP(192, 168, 1, 22);
IPAddress gateway(192, 168, 1, 9);
IPAddress subnet(255, 255, 255, 0);

// Structure example to send data
// Must match the receiver structure
typedef struct struct_message
{
  int id;
  int port;
  int status;
} struct_message;

struct_message data_slave;
struct_message data_slave1;

JSONVar board;
// Create a struct_message called myData

AsyncWebServer server(80);
AsyncEventSource events("/events");
AsyncWebSocket ws("/ws");

// REPLACE WITH RECEIVER MAC Address
uint8_t slave1[] = {0x50, 0x02, 0x91, 0x74, 0x85, 0x0a};
uint8_t slave2[] = {0x84, 0xcc, 0xa8, 0xaa, 0x1b, 0xce};

// slayver test
//uint8_t slave2[] = {0xAC, 0x0B, 0xFB, 0xD8, 0x8E, 0xD5};

// Callback function that will be executed when data is received
void OnDataRecv(uint8_t *mac_addr, uint8_t *incomingData, uint8_t len)
{
  // Copies the sender mac address to a string
  // Serial.println("----------------------------------------");
  char macStr[18];
  // Serial.print("Packet received from: ");
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  // Serial.println(macStr);
  memcpy(&data_slave, incomingData, sizeof(data_slave));

  int Temperature = Random(28, 30);
  int Humidity = Random(70, 75);

  board["id"] = data_slave.id;
  board["slaver_port"] = data_slave.port;
  board["slaver_status"] = data_slave.status;
  board["slaver_Temperature"] = Temperature;
  board["slaver_Humidity"] = Humidity;
  board["slaver_number"] = number;
  String jsonString = JSON.stringify(board);
  events.send(jsonString.c_str(), "new_readings", millis()); // send new readings to web

  // Serial.print("ID slaver: ");Serial.println(board["id"]);
  // Serial.print("Port ");Serial.println(board["slaver_port"]);
  // Serial.print("STATUS PORT: ");Serial.println(board["slaver_status"]);
  // Serial.println("----------------------------------------");
}

// Callback when data is sent
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus)
{
  Serial.print("Last Packet Send Status: ");
  if (sendStatus == 0)
  {
    Serial.println("Delivery success");
  }
  else
  {
    Serial.println("Delivery fail");
  }
}

// board
// board 1
void onload_button1_1()
{
  data_slave1.id = 1;
  data_slave1.port = 1;
  data_slave1.status = 1;
  Serial.print("pess puttion:  ");
  esp_now_send(slave1, (uint8_t *)&data_slave1, sizeof(data_slave1));
  data_slave1.id = 0;
  data_slave1.port = 0;
  data_slave1.status = 0;
  digitalWrite(led, 1);
  delay(500);
}

void onload_button1_2()
{
  data_slave1.id = 1;
  data_slave1.port = 2;
  data_slave1.status = 1;
  Serial.print("pess puttion:  ");
  esp_now_send(slave1, (uint8_t *)&data_slave1, sizeof(data_slave1));
  data_slave1.id = 0;
  data_slave1.port = 0;
  data_slave1.status = 0;
}

void onload_button1_3()
{
  data_slave1.id = 1;
  data_slave1.port = 3;
  data_slave1.status = 1;
  Serial.print("pess puttion:  ");
  esp_now_send(slave1, (uint8_t *)&data_slave1, sizeof(data_slave1));
  data_slave1.id = 0;
  data_slave1.port = 0;
  data_slave1.status = 0;
}
// board 2
void onload_button2_1()
{
  data_slave1.id = 2;
  data_slave1.port = 1;
  data_slave1.status = 1;
  Serial.print("pess puttion:  ");
  esp_now_send(slave2, (uint8_t *)&data_slave1, sizeof(data_slave1));
  data_slave1.id = 0;
  data_slave1.port = 0;
  data_slave1.status = 0;
}

void onload_button2_2()
{
  data_slave1.id = 2;
  data_slave1.port = 2;
  data_slave1.status = 1;
  Serial.print("pess puttion:  ");
  esp_now_send(slave2, (uint8_t *)&data_slave1, sizeof(data_slave1));
  data_slave1.id = 0;
  data_slave1.port = 0;
  data_slave1.status = 0;
}

void onload_button2_3()
{
  data_slave1.id = 2;
  data_slave1.port = 3;
  data_slave1.status = 1;
  Serial.print("pess puttion:  ");
  esp_now_send(slave2, (uint8_t *)&data_slave1, sizeof(data_slave1));
  data_slave1.id = 0;
  data_slave1.port = 0;
  data_slave1.status = 0;
}
// html css

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html>

<head>
    <title>ESP HOME IOT</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <style>
        html {
            font-family: Arial;
            display: inline-block;
            text-align: center;
        }

        h1 {
            font-size: 2rem;
        }

        body {
            margin: 0;
        }

        .topnav {
            overflow: hidden;
            background-color: #2f4468;
            color: white;
            font-size: 1.7rem;
        }

        .content {
            padding: 20px;
        }

        .card {
            background-color: white;
            box-shadow: 2px 2px 12px 1px rgba(140, 140, 140, .5);
            padding-bottom: 20px;
        }

        .cardtime {
            background-color: white;
            box-shadow: 2px 2px 12px 1px rgba(140, 140, 140, .5);
            padding-bottom: 20px;
        }

        .cards {
            max-width: 800px;
            margin: 0 auto;
            display: grid;
            grid-gap: 2rem;
            grid-template-columns: repeat(auto-fit, minmax(320px, 1fr));
            margin-bottom: 30px;
        }

        .title {
            background-color: aqua;
            padding: 20px;
            margin-top: 0;
            color: crimson;
            font-size: 20 px;
            font-weight: bold;
        }

        .port {
            background-color: white;
            padding-top: 10px;
            padding-bottom: 10px;
            margin: 0 0 0 0;
            font-size: 15px;
            font-weight: bold;
        }

        .button {
            padding: 5px 20px;
            font-size: 15px;
            text-align: center;
            outline: none;
            color: #fff;
            background-color: #990000;
            border: none;
            border-radius: 5px;
            -webkit-touch-callout: none;
            -webkit-user-select: none;
            -khtml-user-select: none;
            -moz-user-select: none;
            -ms-user-select: none;
            user-select: none;
            -webkit-tap-highlight-color: rgba(0, 0, 0, 0);
        }

        .button:active {
            background-color: #0f8b8d;
            box-shadow: 2 2px #CDCDCD;
            transform: translateY(2px);
        }
    </style>
</head>

<body>
    <div class="topnav">
        <h1>ESP-NOW DASHBOARD</h1>
    </div>
    <div class="content">
        <div class="cards">
            <div class="cardtime">
                <p class="title">REAL TIME</p>
                <p class="port">TIME NOW: <span id="realtime"></span> </p>
                <p class="port">TEMPERATURE: <span id="temperature"></span> </p>
                <p class="port">HUMIDITY: <span id="humidity"></span> </p>
                <p class="port">NUMBER: <span id="number"></span> </p>
            </div>
            <div class="card">
                <p class="title"> BOARD #1 - CONTROLLER </p>
                <p class="port">PORT 1: STATE: <span id="state1_1"></span> <button id="button1_1"
                        class="button">Toggle</button></p>
                <p class="port">PORT 2: STATE: <span id="state1_2"></span> <button id="button1_2"
                        class="button">Toggle</button></p>
                <p class="port">PORT 3: STATE: <span id="state1_3"></span> <button id="button1_3"
                        class="button">Toggle</button></p>
            </div>
            <div class="card">
                <p class="title"> BOARD #2 - CONTROLLER </p>
                <p class="port">PORT 1: STATE: <span id="state2_1"></span> <button id="button2_1"
                        class="button">Toggle</button></p>
                <p class="port">PORT 2: STATE: <span id="state2_2"></span> <button id="button2_2"
                        class="button">Toggle</button></p>
                <p class="port">PORT 3: STATE: <span id="state2_3"></span> <button id="button2_3"
                        class="button">Toggle</button></p>
            </div>
            <p id="demo"></p>
        </div>
    </div>
    <script>

        function getDateTime() {
            var currentdate = new Date();
            var datetime =
                currentdate.getHours() + ":"
                + currentdate.getMinutes() + ":"
                + currentdate.getSeconds() + " ON "
                + currentdate.getDate() + "/"
                + (currentdate.getMonth() + 1) + "/"
                + currentdate.getFullYear();


            return datetime;
        }

        document.getElementById("realtime").innerHTML = getDateTime();


        if (!!window.EventSource) {
            var source = new EventSource('/events');

            source.addEventListener('open', function (e) {
                console.log("Events Connected");
            }, false);
            source.addEventListener('error', function (e) {
                if (e.target.readyState != EventSource.OPEN) {
                    console.log("Events Disconnected");
                }
            }, false);

            source.addEventListener('message', function (e) {
                console.log("message", e.data);
            }, false);

            source.addEventListener('new_readings', function (e) {
                console.log("new_readings", e.data);
                var obj = JSON.parse(e.data);
                if (obj.slaver_status.toFixed(2) == "1.00"){document.getElementById("state" + obj.id + "_" + obj.slaver_port).innerHTML = "ON";}
                else {document.getElementById("state" + obj.id + "_" + obj.slaver_port).innerHTML = "OFF";}
                document.getElementById("temperature").innerHTML = obj.slaver_Temperature;
                document.getElementById("humidity").innerHTML = obj.slaver_Humidity;
                document.getElementById("number").innerHTML = obj.slaver_number;
                document.getElementById("rt" + obj.id).innerHTML = getDateTime();
                document.getElementById("realtime").innerHTML = getDateTime();
            }, false);
        }


        var gateway = `ws://${window.location.hostname}/ws`;
        var websocket;
        window.addEventListener('load', onload);
        function initWebSocket() {
            console.log('Trying to open a WebSocket connection...');
            websocket = new WebSocket(gateway);
            websocket.onopen    = onOpen;
            websocket.onclose   = onClose;
            websocket.onmessage = onMessage; // <-- add this line
        }
        function onOpen(event) {
            console.log('Connection opened');
        }

        function onClose(event) {
            console.log('Connection closed');
            setTimeout(initWebSocket, 2000);
        }
        function onMessage(event) {
        }
        function onload(event) {
            initWebSocket();
            initButton1_1();
            initButton1_2();
            initButton1_3();
            initButton2_1();
            initButton2_2();
            initButton2_3();
        }
        function initButton1_1() {
            document.getElementById('button1_1').addEventListener('click', click_button1_1);
        }
        function click_button1_1(){
            websocket.send('click_button1_1');
        }
        function initButton1_2() {
            document.getElementById('button1_2').addEventListener('click', click_button1_2);
        }
        function click_button1_2(){
            websocket.send('click_button1_2');
        } 
        function initButton1_3() {
            document.getElementById('button1_3').addEventListener('click', click_button1_3);
        }
        function click_button1_3(){
            websocket.send('click_button1_3');
        }  
        function initButton2_1() {
            document.getElementById('button2_1').addEventListener('click', click_button2_1);
        }
        function click_button2_1(){
            websocket.send('click_button2_1');
        }
        function initButton2_2() {
            document.getElementById('button2_2').addEventListener('click', click_button2_2);
        }
        function click_button2_2(){
            websocket.send('click_button2_2');
        }
        function initButton2_3() {
            document.getElementById('button2_3').addEventListener('click', click_button2_3);
        }
        function click_button2_3(){
            websocket.send('click_button2_3');
        }      
    </script>
</body>

</html>
)rawliteral";

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len)
{
  AwsFrameInfo *info = (AwsFrameInfo *)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT)
  {
    data[len] = 0;
    if (strcmp((char *)data, "click_button1_1") == 0)
    {
      onload_button1_1();
    }
    else if (strcmp((char *)data, "click_button1_2") == 0)
    {
      onload_button1_2();
    }
    else if (strcmp((char *)data, "click_button1_3") == 0)
    {
      onload_button1_3();
    }
    else if (strcmp((char *)data, "click_button2_1") == 0)
    {
      onload_button2_1();
    }
    else if (strcmp((char *)data, "click_button2_2") == 0)
    {
      onload_button2_2();
    }
    else if (strcmp((char *)data, "click_button2_3") == 0)
    {
      onload_button2_3();
    }
  }
}

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
             void *arg, uint8_t *data, size_t len)
{
  switch (type)
  {
  case WS_EVT_CONNECT:
    Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
    break;
  case WS_EVT_DISCONNECT:
    Serial.printf("WebSocket client #%u disconnected\n", client->id());
    break;
  case WS_EVT_DATA:
    handleWebSocketMessage(arg, data, len);
    break;
  case WS_EVT_PONG:
  case WS_EVT_ERROR:
    break;
  }
}

void initWebSocket()
{
  ws.onEvent(onEvent);
  server.addHandler(&ws);
}
// set up
// String processor(const String &var)
//{
//  // Serial.println(var);
//  if (var == "TEMPERATURE")
//  {
//    return readDHTTemperature();
//  }
//  else if (var == "HUMIDITY")
//  {
//    return readDHTHumidity();
//  }
//  return String();
//}

void setup()
{
  pinMode(13, INPUT);
  //  Init Serial Monitor
  Serial.begin(9600);
  // Mac address
  Serial.print("ESP8266 Board MAC Address:  ");
  Serial.println(WiFi.macAddress());

  // Set the device as a Station and Soft Access Point simultaneously
  // WiFi.mode(WIFI_AP_STA);

  // Set device as a Wi-Fi Station
  WiFi.begin(ssid, password);
  // WiFi.config(staticIP, gateway, subnet);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());

  pinMode(2, OUTPUT);
  digitalWrite(2, 0);

  // Init ESP-NOW
  if (esp_now_init() != 0)
  {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_set_self_role(ESP_NOW_ROLE_COMBO);
  esp_now_register_recv_cb(OnDataRecv);
  esp_now_register_send_cb(OnDataSent);

  // Register peer
  esp_now_add_peer(slave1, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);

  // on web
  initWebSocket();

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send_P(200, "text/html", index_html); });

  events.onConnect([](AsyncEventSourceClient *client)
                   {
    if(client->lastId()){
      Serial.printf("Client reconnected! Last message ID that it got is: %u\n", client->lastId());
    }
    // send event with message "hello!", id current millis
    // and set reconnect delay to 1 second
    client->send("hello!", NULL, millis(), 100); });
  server.addHandler(&events);
  server.begin();
}

void loop()
{
  ws.cleanupClients();
  if (digitalRead(13) == 0)
  {
    number++;
    Serial.print("number ");
    Serial.println(number);
    delay(1000);
  }
}