#define BLYNK_TEMPLATE_ID "TMPL36T_WWVP8"
#define BLYNK_TEMPLATE_NAME "ALERT"
#define BLYNK_AUTH_TOKEN "aooxa5GD14JRN0qjLzapzU5s5eO8kHF5"




#define BLYNK_PRINT Serial
#include <ThingSpeak.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = BLYNK_AUTH_TOKEN;

char ssid[] = "Kirithik247";
char pass[] = "11111111";
int gasValue = 0;
unsigned long myChannelNumber =  2486495;
const char* myWriteAPIKey = "NYBAGSNTKQ28D08R";
const int GAS_PIN = A0;
const int motorPin1 = D6; // Connect to L293D input pin 1
const int motorPin2 = D7; // Connect to L293D input pin 2
const int enablePin = D5; // Connect to L293D enable pin
const  int greenled = D3;
const  int redled = D2;
const int powerled=D1;
WiFiClient client;

void setup() {
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(enablePin, OUTPUT);
  pinMode(greenled, OUTPUT);
  pinMode(redled, OUTPUT);
  pinMode(powerled, OUTPUT);
WiFi.mode(WIFI_STA);
  ThingSpeak.begin(client);
  WiFi.begin(ssid, pass);
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");  
  digitalWrite(redled, HIGH);
  digitalWrite(greenled, LOW);
}

void loop() {
  poweron();
  gasValue = analogRead(GAS_PIN);
  ThingSpeak.setField(1, gasValue);
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  Serial.print("The Current GAS value is: ");
  Serial.println(gasValue);
  Blynk.run();
  
  if (gasValue > 300) {
    Serial.println("!! EMERGENCY !!");
    Serial.println("HIGH LEVEL OF GAS LEAKAGE DETECTED !!");
    turnon();
  }

  if (gasValue < 50) {
    turnoff();
  }
  
}

void turnon() {
  Serial.println("!! VENTILATION SYSTEM IS ON !!");
  Blynk.virtualWrite(V1, gasValue);
  Blynk.logEvent("alert", "GAS LEAKAGE DETECTED");
  digitalWrite(motorPin1, HIGH);
  digitalWrite(motorPin2, LOW);
  digitalWrite(enablePin, HIGH);
  delay(1000);
  digitalWrite(redled, HIGH);
  digitalWrite(greenled, LOW);// Turn on the motor
}

void turnoff() {
  Serial.println("!! VENTILATION SYSTEM IS OFF !!");
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);
  digitalWrite(enablePin, LOW); // Turn off the motor
  delay(1000);
  digitalWrite(greenled, HIGH);
  digitalWrite(redled, LOW);
}



void poweron() {
  digitalWrite(powerled, HIGH);
  delay(1000);
    digitalWrite(powerled, LOW);

  
}
