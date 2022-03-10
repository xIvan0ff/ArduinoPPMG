#include <WiFiNINA.h>
#include<WiFiSSLClient.h>
#include<WiFiClient.h>

const char* ssid = "Hristiyan's iPhone";
const char* password = "aaaaaaaa";
WiFiClient client;

int phototransistorPin = A0;
int temperaturePin = A1;
int moisturePin = A2;

const int AVERAGE_N = 30;

void setup() {
 Serial.begin(9600);
 while (!Serial);
 delay(200);
 Serial.print("Connecting Wifi: ");
 Serial.println(ssid);
 while (WiFi.begin(ssid, password) != WL_CONNECTED) {
   Serial.print(".");
   delay(500);
 }
 Serial.println("WiFi connected");
 Serial.println("");
}

void send_data(float temperature, int moisture, int light) {
  String _temperature = String(temperature);
  String _moisture = String(moisture);
  String _light = String(light);
   if (client.connect("wethebest.asuscomm.com", 7777)) {
   // Serial.println("Connected to host.");
   client.print("GET /api/log?temperature=" + _temperature + "&moisture=" + _moisture + "&light=" + _light + " HTTP/1.1\r\nHost: wethebest.asuscomm.com\r\nConnection: close\r\n\r\n");
   /* delay(1000);
   while (client.available()) {
     char c = client.read();
     Serial.write(c);
   } */
   client.stop();
 }
 else {
   Serial.println("Failed to connect to client.");
 }
}

void loop() {
 int moisture = get_average_moisture();
 float temperature = get_average_temperature();
 int light = get_average_light();
 
 Serial.print("Moisture: ");
 Serial.print(moisture);
 Serial.println("%");
 Serial.print("Temperature: ");
 Serial.println(temperature);
 Serial.print("Light: ");
 Serial.print(light);
 Serial.println("%");
 send_data(temperature, moisture, light);
 delay(500);
}

float get_temperature() {
 int reading = analogRead(temperaturePin);
 float voltage = reading * 3.3;
 voltage /= 1024.0;
 float temperatureC = (voltage - 0.5) * 100 ;
 float temperatureF = (temperatureC * 9.0 / 5.0) + 32.0;
 return temperatureC;
}

float get_average_temperature() {
 float temperature_value = 0;
 for (int a = 0; a < AVERAGE_N; a++) {
   temperature_value += get_temperature();
   delay(1);
 }
 return temperature_value / AVERAGE_N;
}

int get_average_moisture() {
 int tempValue = 0;
 for (int a = 0; a < AVERAGE_N; a++) {
   tempValue += analogRead(moisturePin);
   delay(1);
 }
 int moisture_value = tempValue / AVERAGE_N;
 return (float)moisture_value / 1024 * 100;
}

int get_average_light() {
 int light_value = analogRead(phototransistorPin);
 return (float)light_value / 1024 * 100;
}
