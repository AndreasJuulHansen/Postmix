#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <Update.h>
#include <MQTTPubSubClient.h>
#include <FastLED.h>
#include <cmath>
#include <iostream>

// Setting up all the necessary stuff, such as variables, initializing modules, config for Wifi, OTA and MQTT info.  



const char *host = "kegglyMeter1";
const char *ssid = "ScaleWiFi";
const char *password = "12345678";

int LEDStatus[2] = {0, 0};
int previousTime[2] = {0,0};
bool isRed;

#define LED_PIN 5
#define NUM_LEDS 2

String clientID = "kegglyMeter1";
String clientUser = "Team13Beer";
String clientPass = "TeamBeerPWD";

CRGB leds[NUM_LEDS];

WiFiClientSecure client;
MQTTPubSubClient mqtt;
WebServer server(80);



//Here we define the custom function that handles the incoming MQTT msg. We take a tap-ID and its status from MQTT and converts that to an LED reading.
// The function is modular and can handle any amount of taps (length of LED strip).

void updateMeter(uint8_t ID, uint8_t status)
{
  // Serial.println((String) "ID: " + ID + ". Status: " + status);

  if (status == 0)
  {
    leds[ID] = CRGB(0, 0, 0);
  }
  if (status == 1)
  {
    leds[ID] = CRGB(160, 160, 160);
  }
  if (status == 2)
  {
    leds[ID] = CRGB(128, 0, 0);
  }

  if (status == 3)
  {
    // Serial.println("BLINKING TIME");

    if (millis() - previousTime[ID] >= 500)
    {
      //Serial.println("TOGGLE");
      previousTime[ID] = millis();

      if (leds[ID] == CRGB(255, 0, 0))
      {
        //Serial.println("TO BLACK");
        leds[ID] = CRGB(0, 0, 0);
      }
      else
      {
        //Serial.println("TO RED");
        leds[ID] = CRGB(255, 0, 0);
      }
    }
    //Serial.println("Timecheck not passed");
    //Serial.println((String)"nowTime=" + millis());
    //Serial.println((String)"previousTime=" + previousTime);
  }
  FastLED.show();
}

/* Style */
String style =
    "<style>#file-input,input{width:100%;height:44px;border-radius:4px;margin:10px auto;font-size:15px}"
    "input{background:#f1f1f1;border:0;padding:0 15px}body{background:#3498db;font-family:sans-serif;font-size:14px;color:#777}"
    "#file-input{padding:0;border:1px solid #ddd;line-height:44px;text-align:left;display:block;cursor:pointer}"
    "#bar,#prgbar{background-color:#f1f1f1;border-radius:10px}#bar{background-color:#3498db;width:0%;height:10px}"
    "form{background:#fff;max-width:258px;margin:75px auto;padding:30px;border-radius:5px;text-align:center}"
    ".btn{background:#3498db;color:#fff;cursor:pointer}</style>";

/* Login page */
String loginIndex =
    "<form name=loginForm>"
    "<h1>ESP32 Login</h1>"
    "<input name=userid placeholder='User ID'> "
    "<input name=pwd placeholder=Password type=Password> "
    "<input type=submit onclick=check(this.form) class=btn value=Login></form>"
    "<script>"
    "function check(form) {"
    "if(form.userid.value=='admin' && form.pwd.value=='admin')"
    "{window.open('/serverIndex')}"
    "else"
    "{alert('Error Password or Username')}"
    "}"
    "</script>" +
    style;

/* Server Index Page */
String serverIndex =
    "<script src='https://ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js'></script>"
    "<form method='POST' action='#' enctype='multipart/form-data' id='upload_form'>"
    "<input type='file' name='update' id='file' onchange='sub(this)' style=display:none>"
    "<label id='file-input' for='file'>   Choose file...</label>"
    "<input type='submit' class=btn value='Update'>"
    "<br><br>"
    "<div id='prg'></div>"
    "<br><div id='prgbar'><div id='bar'></div></div><br></form>"
    "<script>"
    "function sub(obj){"
    "var fileName = obj.value.split('\\\\');"
    "document.getElementById('file-input').innerHTML = '   '+ fileName[fileName.length-1];"
    "};"
    "$('form').submit(function(e){"
    "e.preventDefault();"
    "var form = $('#upload_form')[0];"
    "var data = new FormData(form);"
    "$.ajax({"
    "url: '/update',"
    "type: 'POST',"
    "data: data,"
    "contentType: false,"
    "processData:false,"
    "xhr: function() {"
    "var xhr = new window.XMLHttpRequest();"
    "xhr.upload.addEventListener('progress', function(evt) {"
    "if (evt.lengthComputable) {"
    "var per = evt.loaded / evt.total;"
    "$('#prg').html('progress: ' + Math.round(per*100) + '%');"
    "$('#bar').css('width',Math.round(per*100) + '%');"
    "}"
    "}, false);"
    "return xhr;"
    "},"
    "success:function(d, s) {"
    "console.log('success!') "
    "},"
    "error: function (a, b, c) {"
    "}"
    "});"
    "});"
    "</script>" +
    style;

/* setup function */
void setup(void)
{
  Serial.begin(115200);

  // Connect to WiFi network
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  /*use mdns for host name resolution*/
  if (!MDNS.begin(host))
  { // http://esp32.local
    Serial.println("Error setting up MDNS responder!");
    while (1)
    {
      delay(1000);
    }
  }
  Serial.println("mDNS responder started");
  /*return index page which is stored in serverIndex */
  server.on("/", HTTP_GET, []()
            {
    server.sendHeader("Connection", "close");
    server.send(200, "text/html", loginIndex); });
  server.on("/serverIndex", HTTP_GET, []()
            {
    server.sendHeader("Connection", "close");
    server.send(200, "text/html", serverIndex); });
  /*handling uploading firmware file */
  server.on(
      "/update", HTTP_POST, []()
      {
    server.sendHeader("Connection", "close");
    server.send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
    ESP.restart(); },
      []()
      {
        HTTPUpload &upload = server.upload();
        if (upload.status == UPLOAD_FILE_START)
        {
          Serial.printf("Update: %s\n", upload.filename.c_str());
          if (!Update.begin(UPDATE_SIZE_UNKNOWN))
          { // start with max available size
            Update.printError(Serial);
          }
        }
        else if (upload.status == UPLOAD_FILE_WRITE)
        {
          /* flashing firmware to ESP*/
          if (Update.write(upload.buf, upload.currentSize) != upload.currentSize)
          {
            Update.printError(Serial);
          }
        }
        else if (upload.status == UPLOAD_FILE_END)
        {
          if (Update.end(true))
          { // true to set the size to the current progress
            Serial.printf("Update Success: %u\nRebooting...\n", upload.totalSize);
          }
          else
          {
            Update.printError(Serial);
          }
        }
      });
  server.begin();

  Serial.print("connecting to host...");
  client.setInsecure(); // skip verification
  while (!client.connect("dfd20863ccd54f47b369ae83d9b324eb.s1.eu.hivemq.cloud", 8883))
  {
    Serial.print(".");
    delay(1000);
  }
  Serial.println(" connected!");

  // initialize mqtt client
  mqtt.begin(client);

  Serial.print("connecting to mqtt broker...");
  while (!mqtt.connect(clientID.c_str(), clientUser.c_str(), clientPass.c_str()))
  {
    Serial.print(".");

    delay(1000);
  }
  Serial.println(" connected!");

  mqtt.subscribe("kegglyMeter1", [](const String &payload, const size_t size)
                 {
                  //Serial.println("");
                Serial.println((String) "Incoming payload is: ["+payload+"]");
                LEDStatus[0] = payload.substring(0,1).toInt();
                LEDStatus[1] = payload.substring(2,3).toInt();
                //Serial.println(""); 
                });

  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
}

///////////////////////////////////////
// Simple loop. Simply calls the custom function for every LED in the length of the LED.
///////////////////////////////////////
void loop()
{
  delay(50);
  mqtt.update(); // should be called
  server.handleClient();

  for (int i = 0; i < NUM_LEDS; i++)
  {
    updateMeter(i, LEDStatus[i]);
  }

};