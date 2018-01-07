
#include <FirebaseArduino.h>
#include <ESP8266WiFi.h>

// Set these to your WIFI settings
#define WIFI_SSID "wifi-name"
#define WIFI_PASSWORD "wifi-password"

//Set these to the Firebase project settings
#define FIREBASE_URL "your-project-name.firebaseio.com"
#define FIREBASE_DB_SECRET "firebase-db-secret"
#define PATH "/count"

//Set an ID of the device - different for each
#define DEVICE_ID "office_1"

//set the pin where the buzzer is connected
#define BUZZER D2

int last_read = 0;
int diff = 0;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(4, OUTPUT);
  Serial.begin(9600);

  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());

  //begin Firebase
  Firebase.begin(FIREBASE_URL, FIREBASE_DB_SECRET);
  
  //get the last updated value for the device
  last_read = Firebase.getInt(DEVICE_ID);
  
  //start streaming the data for the updating value
  Firebase.stream(PATH);
}


void loop() {
  blink();
  //check if streaming failed and try to restart
  if (Firebase.failed()) {
    Serial.println("streaming error");
    Serial.println(Firebase.error());
    delay(1000);
    Firebase.stream(PATH);
    return;
  }
  
  if (Firebase.available()) {
    //get the event
    FirebaseObject event = Firebase.readEvent();
    int data = event.getInt("data");
    diff = data - last_read;
    if (diff > 0) {
      Serial.print("diff: ");
      Serial.println(diff);
      //do the beep of difference
      for(int i = 0; i < diff; i++) {
        beep();
        delay(100);
      }
      //update the new last value
      last_read = data;
      Firebase.setInt(DEVICE_ID, last_read);
      // handle error
      if (Firebase.failed()) {
         Serial.print("setting /number failed:");
         Serial.println(Firebase.error());
      }

      Serial.print("count: ");
      Serial.println(data);

      //set streaming again
      Firebase.stream(PATH);
    }
  }   
}

void blink() {
  digitalWrite(LED_BUILTIN, HIGH);
  delay(500);
  digitalWrite(LED_BUILTIN, LOW);
  delay(500);
}

void beep() {
  tone(BUZZER, 2000, 300);
  delay(300);
  noTone(BUZZER);
}

