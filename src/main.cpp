#include "pitches.h"
#include "Arduino.h"

const int ledPins[4] = {2,3,4,5}; // broche pour les LEDs, rouge vert bleu jaune
const int buttonPins[5] = {6,7,8,9}; // broche pour les boutons, rouge vert bleu jaune
const int startButtonPin = 10; // Bouton de démarrage/action
const int segmentPins[7] = {A3,A2,12,A1,A0,A4,A5}; // Segments A à G
const int buzzerPin = 11;
int Tabrep[16];                            // Séquence maximale (16 étapes)
int currentLevel = 0;                        // Niveau actuel
int gameState = 0;               // 0=attente, 1=sequence, 2=reponse, 3=gameover
unsigned long lastDebounceTime = 0;
const int debounceDelay = 50;
bool sequence_joue = false;

void setup() {
  Serial.begin(9600);
  pinMode(segmentPins[0],OUTPUT);
  pinMode(segmentPins[1], OUTPUT);
  pinMode(segmentPins[2], OUTPUT);
  pinMode(segmentPins[3], OUTPUT);
  pinMode(segmentPins[4], OUTPUT);
  pinMode(segmentPins[5], OUTPUT);
  pinMode(segmentPins[6], OUTPUT);
  pinMode(ledPins[0], OUTPUT);
  pinMode(ledPins[1], OUTPUT);
  pinMode(ledPins[2], OUTPUT);
  pinMode(ledPins[3], OUTPUT);
  pinMode(buttonPins[0], INPUT);
  pinMode(buttonPins[1], INPUT);
  pinMode(buttonPins[2], INPUT);
  pinMode(buttonPins[3], INPUT);
  pinMode(startButtonPin, INPUT);
}

void loop() {
  switch (gameState) {
    case 0: // État attente - écran d'accueil
      if (checkButtonPress(startButtonPin)) {
        startGame();
        gameState = 1;
      }
      break;
      
    case 1: // Joue la séquence
      play_sequence();
      gameState = 2;
      break;

    case 2: // Attend la réponse du joueur
      if (checkPlayerInput()) {
        currentLevel++;
        if (currentLevel > 15) {
          victoryAnimation(); // fonction de Léo à rajouter
          gameState = 0;
        } else {
          score(currentLevel);
          delay(1000);
          gameState = 1;
        }
      } else {
        gameOver();
        gameState = 3;
      }
      break;

    case 3: // Game Over - attend redémarrage
      if (checkButtonPress(startButtonPin)) {
        gameState = 0;
        showStartScreen();
      }
      break;
  }
}

void startGame() {
  currentLevel = 1;
  score(currentLevel);
  // Génère une nouvelle séquence aléatoire
  //Met le code pour crée Tabrep aleatoirement
  for (int i = 0; i<16; i++){
    Tabrep[i]=random(0,4);
    Serial.print(Tabrep[i]);
    Serial.write('\n');
  }
}

bool checkButtonPress(int buttonPin) {
  if (digitalRead(buttonPin) == HIGH) { 
    delay(debounceDelay);                 // Attend le délai d'anti-rebond
    if (digitalRead(buttonPin) == HIGH) { // Vérifie si le bouton est toujours pressé
      while (digitalRead(buttonPin) == HIGH); // Attend le relâchement
      return true;
    }
  }
  return false;
}

void showStartScreen() {
  // Affiche "S" sur le 7 segments
  // Segments: A,B,C,D,E,F,G
  digitalWrite(segmentPins[0],HIGH);
  digitalWrite(segmentPins[1],LOW);
  digitalWrite(segmentPins[2],HIGH);
  digitalWrite(segmentPins[3],HIGH);
  digitalWrite(segmentPins[4],LOW);
  digitalWrite(segmentPins[5],HIGH);
  digitalWrite(segmentPins[6],HIGH);
  // Animation LEDs
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 4; j++) {
      digitalWrite(ledPins[j], HIGH);
    }
    delay(300);
    for (int j = 0; j < 4; j++) {
      digitalWrite(ledPins[j], LOW);
    }
    delay(300);
  }
}

void play_sequence(){
  for (int i = 0;i<=currentLevel;i++){
    digitalWrite(ledPins[Tabrep[i]], HIGH);
    delay(500);
    digitalWrite(ledPins[Tabrep[i]], LOW);
    delay(200);
  }
  sequence_joue = true;
}

bool checkPlayerInput() {
  int buttonPressed;  //stocke quel bouton a été pressé
  int currentStep = 0; //suit la position dans la séquence à reproduire
  
  while(currentStep < currentLevel) {
    // Vérification des boutons
    for(int i=0; i<4; i++) {
      if(digitalRead(buttonPins[i]) == HIGH) {
        // Un bouton a été pressé
        buttonPressed = i;    // Enregistre quel bouton est préessé
        digitalWrite(ledPins[i], HIGH); // Allume la LED comrespondandte
        delay(200);
        digitalWrite(ledPins[i], LOW);  // Eteint la LED
        while (digitalRead(buttonPins[i]) == HIGH); // Attend relâchement
        if (buttonPressed != Tabrep[currentStep]) {
          return false; // Mauvaise réponse
        }
        currentStep++; // Bonne réponse, passe à l'étape suivante
      }
    }
    // Vérification bouton start (abandon)
    if(digitalRead(startButtonPin) == HIGH) {
      return false;
    }
    delay(10);
  }
  return true;
}

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
  tone(buzzerPin,num,1);
  delay(10000);
}

void score(int n){
  switch(n){
    case 1:
    digitalWrite(segmentPins[0],LOW);
    digitalWrite(segmentPins[1],HIGH);
    digitalWrite(segmentPins[2],HIGH);
    digitalWrite(segmentPins[3],LOW);
    digitalWrite(segmentPins[4],LOW);
    digitalWrite(segmentPins[5],LOW);
    digitalWrite(segmentPins[6],LOW);
    break;
    case 2:
    digitalWrite(segmentPins[0],HIGH);
    digitalWrite(segmentPins[1],HIGH);
    digitalWrite(segmentPins[2],LOW);
    digitalWrite(segmentPins[3],HIGH);
    digitalWrite(segmentPins[4],HIGH);
    digitalWrite(segmentPins[5],LOW);
    digitalWrite(segmentPins[6],HIGH);
    break;
    case 3:
    digitalWrite(segmentPins[0],HIGH);
    digitalWrite(segmentPins[1],HIGH);
    digitalWrite(segmentPins[2],HIGH);
    digitalWrite(segmentPins[3],HIGH);
    digitalWrite(segmentPins[4],LOW);
    digitalWrite(segmentPins[5],LOW);
    digitalWrite(segmentPins[6],HIGH);
    break;
    case 4:
    digitalWrite(segmentPins[0],LOW);
    digitalWrite(segmentPins[1],HIGH);
    digitalWrite(segmentPins[2],HIGH);
    digitalWrite(segmentPins[3],LOW);
    digitalWrite(segmentPins[4],LOW);
    digitalWrite(segmentPins[5],HIGH);
    digitalWrite(segmentPins[6],HIGH);
    break;
    case 5:
    digitalWrite(segmentPins[0],HIGH);
    digitalWrite(segmentPins[1],LOW);
    digitalWrite(segmentPins[2],HIGH);
    digitalWrite(segmentPins[3],HIGH);
    digitalWrite(segmentPins[4],LOW);
    digitalWrite(segmentPins[5],HIGH);
    digitalWrite(segmentPins[6],HIGH);
    break;
    case 6: 
    digitalWrite(segmentPins[0],HIGH);
    digitalWrite(segmentPins[1],LOW);
    digitalWrite(segmentPins[2],HIGH);
    digitalWrite(segmentPins[3],HIGH);
    digitalWrite(segmentPins[4],HIGH);
    digitalWrite(segmentPins[5],HIGH);
    digitalWrite(segmentPins[6],HIGH);
    break;
    case 7:
    digitalWrite(segmentPins[0],HIGH);
    digitalWrite(segmentPins[1],HIGH);
    digitalWrite(segmentPins[2],HIGH);
    digitalWrite(segmentPins[3],LOW);
    digitalWrite(segmentPins[4],LOW);
    digitalWrite(segmentPins[5],LOW);
    digitalWrite(segmentPins[6],LOW);
    break;
    case 8:
    digitalWrite(segmentPins[0],HIGH);
    digitalWrite(segmentPins[1],HIGH);
    digitalWrite(segmentPins[2],HIGH);
    digitalWrite(segmentPins[3],HIGH);
    digitalWrite(segmentPins[4],HIGH);
    digitalWrite(segmentPins[5],HIGH);
    digitalWrite(segmentPins[6],HIGH);
    break;
    case 9:
    digitalWrite(segmentPins[0],HIGH);
    digitalWrite(segmentPins[1],HIGH);
    digitalWrite(segmentPins[2],HIGH);
    digitalWrite(segmentPins[3],HIGH);
    digitalWrite(segmentPins[4],LOW);
    digitalWrite(segmentPins[5],HIGH);
    digitalWrite(segmentPins[6],HIGH);
    break;
    case 10:
    digitalWrite(segmentPins[0],HIGH);
    digitalWrite(segmentPins[1],HIGH);
    digitalWrite(segmentPins[2],HIGH);
    digitalWrite(segmentPins[3],LOW);
    digitalWrite(segmentPins[4],HIGH);
    digitalWrite(segmentPins[5],HIGH);
    digitalWrite(segmentPins[6],HIGH);
    break;
    case 11:
    digitalWrite(segmentPins[0],LOW);
    digitalWrite(segmentPins[1],LOW);
    digitalWrite(segmentPins[2],HIGH);
    digitalWrite(segmentPins[3],HIGH);
    digitalWrite(segmentPins[4],HIGH);
    digitalWrite(segmentPins[5],HIGH);
    digitalWrite(segmentPins[6],HIGH);
    break;
    case 12:
    digitalWrite(segmentPins[0],HIGH);
    digitalWrite(segmentPins[1],LOW);
    digitalWrite(segmentPins[2],LOW);
    digitalWrite(segmentPins[3],HIGH);
    digitalWrite(segmentPins[4],HIGH);
    digitalWrite(segmentPins[5],HIGH);
    digitalWrite(segmentPins[6],LOW);
    break;
    case 13:
    digitalWrite(segmentPins[0],LOW);
    digitalWrite(segmentPins[1],HIGH);
    digitalWrite(segmentPins[2],HIGH);
    digitalWrite(segmentPins[3],HIGH);
    digitalWrite(segmentPins[4],HIGH);
    digitalWrite(segmentPins[5],LOW);
    digitalWrite(segmentPins[6],HIGH);
    break;
    case 14:
    digitalWrite(segmentPins[0],HIGH);
    digitalWrite(segmentPins[1],LOW);
    digitalWrite(segmentPins[2],LOW);
    digitalWrite(segmentPins[3],HIGH);
    digitalWrite(segmentPins[4],HIGH);
    digitalWrite(segmentPins[5],HIGH);
    digitalWrite(segmentPins[6],HIGH);
    break;
    case 15:
    digitalWrite(segmentPins[0],HIGH);
    digitalWrite(segmentPins[1],LOW);
    digitalWrite(segmentPins[2],LOW);
    digitalWrite(segmentPins[3],LOW);
    digitalWrite(segmentPins[4],HIGH);
    digitalWrite(segmentPins[5],HIGH);
    digitalWrite(segmentPins[6],HIGH);
    break;
  }
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