#include <Arduino.h>

const int ledPins[4] = {2,3,4,5}; // broche pour les LEDs, rouge vert bleu jaune
const int buttonPins[4] = {6,7,8,9}; // broche pour les boutons, rouge vert bleu jaune
const int A = A3;
const int B = A2;
const int C = 12;
const int D = A1;
const int E = A0;
const int F = A4;
const int G = A5;

void setup() {

Serial.begin(9600);
pinMode(A,OUTPUT);
pinMode(B, OUTPUT);
pinMode(C, OUTPUT);
pinMode(D, OUTPUT);
pinMode(E, OUTPUT);
pinMode(F, OUTPUT);
pinMode(G, OUTPUT);
int Tabrep[16];
  for (int i = 0; i<16; i++){
    Tabrep[i]=random(1,5);
    Serial.print(Tabrep[i]);}

}



    

void loop() {
  digitalWrite(A,HIGH); //Pour écrire 1 à priori
  digitalWrite(B,LOW);
  digitalWrite(C,LOW);
  digitalWrite(D,HIGH);
  digitalWrite(E,HIGH);
  digitalWrite(F,HIGH);
  digitalWrite(G,HIGH);
  for (int i = 0; i < 4; i++) {
    digitalWrite(ledPins[i], HIGH); // Allume
    delay(500);
    digitalWrite(ledPins[i], LOW);  // Éteint
    delay(200);}
}