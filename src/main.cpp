#include "pitches.h"
#include "Arduino.h"

const int ledPins[4] = {2,3,4,5}; // broche pour les LEDs, rouge vert bleu jaune
const int buttonPins[4] = {6,7,8,9}; // broche pour les boutons, rouge vert bleu jaune
const int startButtonPin = 10; // Bouton de démarrage/action
const int segmentPins[7] = {A3,A2,12,A1,A0,A4,A5}; // Segments A à G
const int buzzerPin = 11;

int Tabrep[16];                             // Tableau vide  qui contiendra la séquence généré aleatoirement dans la fonction startGame (16 etapes max)
int currentLevel = 0;                       // Niveau actuel du jeu (actuelement 0/16)
int gameState = 0;                          // 0 = attente, 1 = joue la sequence, 2 = reponse du joueur, 3 = gameover      
const int debounceDelay = 50;               // Durée pour eviter les rebonds sur les boutons           

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
  pinMode(buzzerPin, OUTPUT);
}

void loop() {
  switch (gameState) {
    case 0:           // État attente - écran d'accueil
      if (checkButtonPress(startButtonPin)) {
        startGame();  // Lance une nouvelle partie wouhou
        gameState = 1;
      } else {
        //animation d'attente
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
      break;
      
    case 1: // Joue la séquence
      play_sequence();
      gameState = 2;
      break;

    case 2: // Attend la réponse du joueur
      if (checkPlayerInput()) { // si la reponse du joueur est vraie
        currentLevel++;
        if (currentLevel > 4) { // si la niveau maximum atteint
          score(currentLevel);  // afficher le score
          darude_sandstorm();   // musique de victoire
          gameState = 0;        // retour au menu
        } else {                // sinon on passe au niveau suivant
          score(currentLevel);  
          delay(1000);
          gameState = 1;
        }
      } else { // le joueur c'est donc trompé (il s'est TRUMPé !!!!)
        GameOver(); // fonction qui gère l'animation du game over
        gameState = 3;
      }
      break;

    case 3: // Game Over - attend redémarrage
      if (checkButtonPress(startButtonPin)) { // attand le joueur presse le bouton reset pour recommancer le jeu
        gameState = 0;
        showStartScreen(); // animation d'attante du restart
      }
      break;
  }
}

void startGame() {
  currentLevel = 0;             // réinitialise le niveau
  score(currentLevel);          
  for (int i = 0; i<16; i++){   // boucle pour génerer une séquence aléatoire
    Tabrep[i]=random(0,4);
    Serial.print(Tabrep[i]);    // pour verifier la séquence dans la console
    Serial.write('\n');
  }
}

void play_sequence(){
  // joue la séquence jusqu'au niveau actuel
  for (int i = 0;i<=currentLevel;i++){
    digitalWrite(ledPins[Tabrep[i]], HIGH); // allume la LED
    son(Tabrep[i]);                         // joue le son corespondant
    delay(500);
    digitalWrite(ledPins[Tabrep[i]], LOW);  // éteint la LED
    delay(200);
  }
}

bool checkPlayerInput() {
  int buttonPressed;  //stocke quel bouton a été pressé
  int currentStep = 0; //étape actuelle dans la séquence (ex : 2/8 étape 2 sur une séquence de 8 (niveau 8))
  
  while(currentStep <= currentLevel) {   // tant que l'etape est inferieure ou égale a la taille de la séquence actuelle
    // Vérification des boutons
    for(int i=0; i<4; i++) {
      if(digitalRead(buttonPins[i]) == HIGH) {
        // Un bouton a été pressé
        buttonPressed = i;    // Enregistre quel bouton est pressé
        digitalWrite(ledPins[i], HIGH); // Allume la LED corespondante
        son(i);                         // son corespondant
        delay(200);
        digitalWrite(ledPins[i], LOW);  // Eteint la LED

        while (digitalRead(buttonPins[i]) == HIGH); // Attend relâchement
        if (buttonPressed != Tabrep[currentStep]) { // Vérifie si le bouton est dans la séquence
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

bool checkButtonPress(int buttonPin) {    
  if (digitalRead(buttonPin) == HIGH) {   // si le bouton est presse
    delay(debounceDelay);                 // Attend le délai d'anti-rebond
    if (digitalRead(buttonPin) == HIGH) { // Vérifie si le bouton est toujours pressé
      // while (digitalRead(buttonPin) == HIGH); // Attend le relâchement
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

void son(int num) {
  switch(num){
    case 0: num = 500; break; // son pour le rouge
    case 1: num = 600; break; // vert
    case 2: num = 700; break; // bleu
    case 3: num = 800; break; // jaune
  }
  tone(buzzerPin,num,100);    // joue le son
  delay(10);
  noTone(11);                 // arrête le son
}

void GameOver(){
  int NOTE_SUSTAIN = 40;
  for(uint8_t nLoop = 0;nLoop < 2;nLoop ++){
    tone(11,NOTE_A5,100);
    delay(NOTE_SUSTAIN);
    tone(11,NOTE_B5,100);
    delay(NOTE_SUSTAIN);
    tone(11,NOTE_C5,100);
    delay(NOTE_SUSTAIN);
    tone(11,NOTE_B5,100);
    delay(NOTE_SUSTAIN);
    tone(11,NOTE_C5,100);
    delay(NOTE_SUSTAIN);
    tone(11,NOTE_D5,100);
    delay(NOTE_SUSTAIN);
    tone(11,NOTE_C5,100);
    delay(NOTE_SUSTAIN);
    tone(11,NOTE_D5,100);
    delay(NOTE_SUSTAIN);
    tone(11,NOTE_E5,100);
    delay(NOTE_SUSTAIN);
    tone(11,NOTE_D5,100);
    delay(NOTE_SUSTAIN);
    tone(11,NOTE_E5,100);
    delay(NOTE_SUSTAIN);
    tone(11,NOTE_E5,100);
    delay(NOTE_SUSTAIN);
  }
}

void score(int n){   // affiche le chiffre n sur l'afficheur 7 segments
  switch(n){
    case 0:
    digitalWrite(segmentPins[0],HIGH);
    digitalWrite(segmentPins[1],HIGH);
    digitalWrite(segmentPins[2],HIGH);
    digitalWrite(segmentPins[3],HIGH);
    digitalWrite(segmentPins[4],HIGH);
    digitalWrite(segmentPins[5],HIGH);
    digitalWrite(segmentPins[6],LOW);
    break;

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