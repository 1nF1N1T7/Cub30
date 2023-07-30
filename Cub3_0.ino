#include <dht.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//CLASSE
dht DHT;
Adafruit_SSD1306 display(128, 32, &Wire, -1);


void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // Address 0x3D for 128x64
  //U.S
  pinMode(4, OUTPUT); //5v
  pinMode(5, OUTPUT); //GND
  pinMode(6, OUTPUT); //Trig
  pinMode(7, INPUT);  //Echo

 //Laser
  pinMode(11, OUTPUT); //5v

  //Button
  pinMode(12, OUTPUT); //GND

  //potentiometer

  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);

  //BUZZER
  pinMode(A3, OUTPUT); //5v
  pinMode(A4, OUTPUT); //GND

}


void SHOW(int size, int clear, int x, int y, String msg) {
  if (clear) {
    display.clearDisplay();
  }
  display.setTextSize(size);
  display.setTextColor(WHITE);
  display.setCursor(x, y);
  // Display static text
  display.println(msg);
  display.display();
}


void temperature() {

  // READ DATA
  int chk = DHT.read11(8);

  // DISPLAY DATA
  String state = "Humidity: " + String(DHT.humidity) + " %\n\n"+ "Temperature: " + String(DHT.temperature) + " C"; 
  SHOW(1, 1, 0, 0, state);
}


void distance() {

  digitalWrite(4, HIGH); //Enable 5v
  digitalWrite(5, LOW); //Enable GND

  digitalWrite(11, HIGH); //Enable 5v

  digitalWrite(6, LOW);

  delayMicroseconds(2);

  digitalWrite(6, HIGH);

  delayMicroseconds(10);

  digitalWrite(6, LOW);

  float t = pulseIn(7, HIGH);
  float d = t * 0.034 / 2;
  String distance = String(d) + "cm";

  SHOW(2, 1, 0, 15, String(distance));
  digitalWrite(11, LOW); //disable 5v Laser
}


void alarm() {
  SHOW(2, 1, 0, 15, "ALARM!");
  delay(2000);
  int e = 1;
  while (e) {
    int button = analogRead(A5);
    int m = analogRead(A1) + 1;
    String t = String(m) + "min";
    SHOW(2, 1, 0, 15, t);
    if (button == 0) {
      while (1) {
        int cm = millis() / 1000;
        int et = m * 60 - cm;
        if (et <= 0) {
          digitalWrite(A3, HIGH);
          digitalWrite(A4, LOW);
          SHOW(2, 1, 0, 15, "Time's UP!!!");
          delay(3000);
          digitalWrite(A3, LOW);
          e = 0;
          break;
        } else {
          SHOW(2, 1, 0, 15, String(et) + "s");
        }
      }
    }

  }
}


void loop() {
  //potentiometer
  digitalWrite(9, LOW);
  digitalWrite(10, HIGH);
  int potentiometerStatus = analogRead(A1);


  //  digitalWrite(12, LOW); //Enable GND

  //IsPotentiometer turned?
  if (potentiometerStatus < 341) {
    temperature();
  } else if (potentiometerStatus > 341 && potentiometerStatus < 682) {
    distance();
  } else if (potentiometerStatus > 682 && potentiometerStatus < 1023) {
    alarm();
  }


}
