#include <DHT.h>  // Including library for dht
#include <ESP8266WiFi.h>
 
String apiKey = "5KVA1N26AWN81ENA";     //  Enter your Write API key from ThingSpeak
 
const char *ssid =  "BELANG";     // replace with your wifi ssid and wpa2 key
const char *pass =  "123456789";
const char* server = "api.thingspeak.com";
const char* host = "dht11.susila.miktool.my.id";

int suhu = 0; 
int nilai_suhu = 0;

int kelembaban = 0; 
int nilai_kelembaban = 0;
 
#define DHTPIN 0          //pin where the dht11 is connected
DHT dht(DHTPIN, DHT11);
WiFiClient client;
void setup() 
{
       Serial.begin(115200);
       delay(10);
       dht.begin();
       Serial.println("Connecting to ");
       Serial.println(ssid);
       WiFi.begin(ssid, pass);
      while (WiFi.status() != WL_CONNECTED) 
     {
            delay(500);
            Serial.print(".");
     }
      Serial.println("");
      Serial.println("WiFi connected");
}
void loop() 
{
  
      float h = dht.readHumidity();
      float t = dht.readTemperature();
      
              if (isnan(h) || isnan(t)) 
                 {
                     Serial.println("Failed to read from DHT sensor!");
                      return;
                 }
 
                         if (client.connect(server,80))   //   "184.106.153.149" or api.thingspeak.com
                      {  
                            
                             String postStr = apiKey;
                             postStr +="&field1=";
                             postStr += String(t);
                             postStr +="&field2=";
                             postStr += String(h);
                             postStr += "\r\n\r\n";
 
                             client.print("POST /update HTTP/1.1\n");
                             client.print("Host: api.thingspeak.com\n");
                             client.print("Connection: close\n");
                             client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
                             client.print("Content-Type: application/x-www-form-urlencoded\n");
                             client.print("Content-Length: ");
                             client.print(postStr.length());
                             client.print("\n\n");
                             client.print(postStr);
 
                             Serial.print("Temperature: ");
                             Serial.print(t);
                             Serial.print(" degrees Celcius, Humidity: ");
                             Serial.print(h);
                             Serial.println("%. Send to Thingspeak.");
                             delay(1000);

                             suhu = dht.readTemperature();
                             nilai_suhu = suhu;

                             kelembaban = dht.readHumidity();
                             nilai_kelembaban = kelembaban;

                             WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
 
  // We now create a URI for the request
  String url = "/add.php?";
  url += "nilai_sensor=";
  url += nilai_suhu;
 
  Serial.print("Requesting URL: ");
  Serial.println(url);
 
  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");

  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }

    String url2 = "/add2.php?";
  url2 += "nilai_sensor=";
  url2 += nilai_kelembaban;
 
  Serial.print("Requesting URL: ");
  Serial.println(url2);
 
  // This will send the request to the server
  client.print(String("GET ") + url2 + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");
 
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }                   
                        }
          client.stop();
          Serial.println("Waiting...");
  delay(1000);
}
