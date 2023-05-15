#include "DHT.h" // DHT sensor library by Adafruit
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ESP8266HTTPClient.h>
//#include <Supabase.h>

#define DHTPIN 5 // D1
#define DHTTYPE DHT11 

// Replace with your network credentials
const char* ssid     = "WE_E11030";
const char* password = "35bGateway@pOWer";

// supabase credentials
String API_URL = "https://eawamgpjbkirebwesnoc.supabase.co";
String API_KEY = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJzdXBhYmFzZSIsInJlZiI6ImVhd2FtZ3BqYmtpcmVid2Vzbm9jIiwicm9sZSI6ImFub24iLCJpYXQiOjE2ODM5NTYyNzQsImV4cCI6MTk5OTUzMjI3NH0.8qs-lPmnrNug0Gp_7vk2rFZ3oT8BskvTllqBU1JNgrE";
String TableName = "natural factors";
const int httpsPort = 5432;

// Sending interval of the packets in seconds
int sendinginterval = 60; // 20 minutes
//int sendinginterval = 120; // 2 minutes

HTTPClient https;
WiFiClientSecure client;
DHT dht(DHTPIN, DHTTYPE);

float h;
float t;
int m;

void setup() {
  // builtIn led is used to indicate when a message is being sent
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH); // the builtin LED is wired backwards HIGH turns it off

  // HTTPS is used without checking credentials 
  client.setInsecure();

  // Connect to the WIFI 
  Serial.begin(115200);
  dht.begin();
  
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  // Print local IP address
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {

   // If connected to the internet turn the Builtin led On and attempt to send a message to the database 
  if (WiFi.status() == WL_CONNECTED) {
    digitalWrite(LED_BUILTIN, LOW); // LOW turns ON

    // Read all sensors
    h = dht.readHumidity();
    t = dht.readTemperature();
    m = analogRead(A0);

    // Send the a post request to the server
    https.begin(client,API_URL+"/rest/v1/"+TableName);
    https.addHeader("Content-Type", "application/json");
    https.addHeader("Prefer", "return=representation");
    https.addHeader("apikey", API_KEY);
    https.addHeader("Authorization", "Bearer " + API_KEY);
    int httpCode = https.POST("{\"temperature\":" + String(t)+ ",\"humidity\":"+ String(h)+",\"moisture\":" + String(1024 - m)+"}" );   //Send the request
    String payload = https.getString(); 
    Serial.println(httpCode);   //Print HTTP return code
    Serial.println(payload);    //Print request response payload
    https.end();

    
    digitalWrite(LED_BUILTIN, HIGH); // HIGH turns off
  }else{
    Serial.println("Error in WiFi connection");
  }
  delay(1000*sendinginterval);  //wait to send the next request
  
}
