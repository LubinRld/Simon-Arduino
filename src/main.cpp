#include "pitches.h"
#include "Arduino.h"

const int ledPins[4] = {2,3,4,5}; // broche pour les LEDs, rouge vert bleu jaune
const int buttonPins[4] = {6,7,8,9}; // broche pour les boutons, rouge vert bleu jaune
const int A = A3;
const int B = A2;
const int C = 12;
const int D = A1;
const int E = A0;
const int F = A4;
const int G = A5;
int compteursucces = 15;
int Tabrep[16]; // Tableau de séquence aléatoire
bool sequence_joue = false;

void son(int num) {
  switch(num){
    case 0:
    num = 523;
    break;
    case 1:
    num = 587;
    break;
    case 2:
    num = 659;
    break;
    case 3:
    num = 698;
    break;
  }
  tone(11,num,1);
  delay(10000);
}

void darude_sandstorm(){
  int shortTone = 80;
  int longTone = 200;
  int standardDelay = 50;
  int delayBetweenBars = 50;

  tone(11,  NOTE_B3, shortTone);
  delay(shortTone);
  noTone(11);
  delay(standardDelay);
  tone(11,  NOTE_B3, shortTone);
  delay(shortTone);
  noTone(11);
  delay(standardDelay);
  tone(11,  NOTE_B3, shortTone);
  delay(shortTone);
  noTone(11);
  delay(standardDelay);
  tone(11,  NOTE_B3, shortTone);
  delay(shortTone);
  noTone(11);
  delay(standardDelay);
  tone(11,  NOTE_B3, longTone); 
  delay(longTone);
  noTone(11);
  delay(delayBetweenBars);
  
  tone(11,  NOTE_B3, shortTone);
  delay(shortTone);
  noTone(11);
  delay(standardDelay); 
  tone(11,NOTE_B3, shortTone);
  delay(shortTone);
  noTone(11);
  delay(standardDelay);
  tone(11,  NOTE_B3, shortTone);
  delay(shortTone);
  noTone(11);
  delay(standardDelay);
  tone(11,  NOTE_B3, shortTone);
  delay(shortTone);
  noTone(11);
  delay(standardDelay);
  tone(11,  NOTE_B3, shortTone);
  delay(shortTone);
  noTone(11);
  delay(standardDelay);
  tone(11,  NOTE_B3, shortTone);
  delay(shortTone);
  noTone(11);
  delay(standardDelay);
  tone(11,  NOTE_B3, longTone); 
  delay(longTone);
  noTone(11);
  delay(standardDelay);
 
  tone(11,  NOTE_E4, shortTone);
  delay(shortTone);
  noTone(11);
  delay(standardDelay); 
  tone(11,NOTE_E4, shortTone);
  delay(shortTone);
  noTone(11);
  delay(standardDelay);
  tone(11,  NOTE_E4, shortTone);
  delay(shortTone);
  noTone(11);
  delay(standardDelay);
  tone(11,  NOTE_E4, shortTone);
  delay(shortTone);
  noTone(11);
  delay(standardDelay);
  tone(11,  NOTE_E4, shortTone);
  delay(shortTone);
  noTone(11);
  delay(standardDelay);
  tone(11,  NOTE_E4, shortTone);
  delay(shortTone);
  noTone(11);
  delay(standardDelay);
  tone(11,  NOTE_E4, longTone); 
  delay(longTone);
  noTone(11);
  delay(standardDelay);
 
 
  tone(11,  NOTE_D4, shortTone);
  delay(shortTone);
  noTone(11);
  delay(standardDelay); 
  tone(11,NOTE_D4, shortTone);
  delay(shortTone);
  noTone(11);
  delay(standardDelay);
  tone(11,  NOTE_D4, shortTone);
  delay(shortTone);
  noTone(11);
  delay(standardDelay);
  tone(11,  NOTE_D4, shortTone);
  noTone(11);
  delay(standardDelay);
  tone(11,  NOTE_D4, shortTone);
  delay(shortTone);
  noTone(11);
  delay(standardDelay);
  tone(11,  NOTE_D4, shortTone);
  delay(shortTone);
  noTone(11);
  delay(standardDelay);
  tone(11,  NOTE_D4, longTone); 
  delay(longTone);
  noTone(11);
  delay(standardDelay);
 
  tone(11, NOTE_A3, longTone);
  delay(longTone);
  noTone(11);
  delay(standardDelay);
 
  tone(11,  NOTE_B3, shortTone);
  delay(shortTone);
  noTone(11);
  delay(standardDelay);
  tone(11,  NOTE_B3, shortTone);
  delay(shortTone);
  noTone(11);
  delay(standardDelay);
  tone(11,  NOTE_B3, shortTone);
  delay(shortTone);
  noTone(11);
  delay(standardDelay);
  tone(11,  NOTE_B3, shortTone);
  delay(shortTone);
  noTone(11);
  delay(standardDelay);
  tone(11,  NOTE_B3, longTone); 
  delay(longTone);
  noTone(11);
  delay(delayBetweenBars);
 
  tone(11,  NOTE_B3, shortTone);
  delay(shortTone);
  noTone(11);
  delay(standardDelay); 
  tone(11,NOTE_B3, shortTone);
  delay(shortTone);
  noTone(11);
  delay(standardDelay);
  tone(11,  NOTE_B3, shortTone);
  delay(shortTone);
  noTone(11);
  delay(standardDelay);
  tone(11,  NOTE_B3, shortTone);
  delay(shortTone);
  noTone(11);
  delay(standardDelay);
  tone(11,  NOTE_B3, shortTone);
  delay(shortTone);
  noTone(11);
  delay(standardDelay);
  tone(11,  NOTE_B3, shortTone);
  delay(shortTone);
  noTone(11);
  delay(standardDelay);
  tone(11,  NOTE_B3, longTone); 
  delay(longTone);
  noTone(11);
  delay(standardDelay);
 
  tone(11, NOTE_E4, longTone);
  delay(longTone);
  noTone(11);
  delay(standardDelay);
 
  tone(11,  NOTE_B3, shortTone);
  delay(shortTone);
  noTone(11);
  delay(standardDelay);
  tone(11,  NOTE_B3, shortTone);
  delay(shortTone);
  noTone(11);
  delay(standardDelay);
  tone(11,  NOTE_B3, shortTone);
  delay(shortTone);
  noTone(11);
  delay(standardDelay);
  tone(11,  NOTE_B3, shortTone);
  delay(shortTone);
  noTone(11);
  delay(standardDelay);
  tone(11,  NOTE_B3, longTone); 
  delay(longTone);
  noTone(11);
  delay(delayBetweenBars);
 
  tone(11,  NOTE_B3, shortTone);
  delay(shortTone);
  noTone(11);
  delay(standardDelay); 
  tone(11,NOTE_B3, shortTone);
  delay(shortTone);
  noTone(11);
  delay(standardDelay);
  tone(11,  NOTE_B3, shortTone);
  delay(shortTone);
  noTone(11);
  delay(standardDelay);
  tone(11,  NOTE_B3, shortTone);
  delay(shortTone);
  noTone(11);
  delay(standardDelay);
  tone(11,  NOTE_B3, shortTone);
  delay(shortTone);
  noTone(11);
  delay(standardDelay);
  tone(11,  NOTE_B3, shortTone);
  delay(shortTone);
  noTone(11);
  delay(standardDelay);
  tone(11,  NOTE_B3, longTone); 
  delay(longTone);
  noTone(11);
  delay(standardDelay);
 
  tone(11, NOTE_E4, longTone); 
  delay(longTone);
  noTone(11);
  delay(standardDelay);
 




}

void setup() {

Serial.begin(9600);
pinMode(A,OUTPUT);
pinMode(B, OUTPUT);
pinMode(C, OUTPUT);
pinMode(D, OUTPUT);
pinMode(E, OUTPUT);
pinMode(F, OUTPUT);
pinMode(G, OUTPUT);
for (int i = 0; i<16; i++){
    Tabrep[i]=random(0,4);
    Serial.print(Tabrep[i]);
    Serial.write('\n');
  }
}

void play_sequence(){
for (int i = 0;i<=compteursucces;i++){
  digitalWrite(ledPins[Tabrep[i]], HIGH);
  delay(500);
  digitalWrite(ledPins[Tabrep[i]], LOW);
  delay(200);
}
sequence_joue = true;
}

void loop() {
  digitalWrite(A,HIGH); //Pour écrire 1 à priori
  digitalWrite(B,LOW);
  digitalWrite(C,LOW);
  digitalWrite(D,HIGH);
  digitalWrite(E,HIGH);
  digitalWrite(F,HIGH);
  digitalWrite(G,HIGH);
  if(sequence_joue==false)
  {
  play_sequence();
  }
}