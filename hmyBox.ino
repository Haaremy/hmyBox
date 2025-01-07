#include "Arduino.h"
#include "SoftwareSerial.h"
#include <DFRobotDFPlayerMini.h>
#include <SPI.h>
#include <MFRC522.h>

#define SDA_PIN 53  // SDA ist der CS/SS-Pin
#define RST_PIN 5   // Reset-Pin
#define LED 13
#define playstop 2 // Taste für Play und Pause
#define louder 4   // Taste für Lauter
#define quiter 3   // Taste für Leiser

int isPause = 0;
int vol = 5;
const unsigned long debounceDelay = 50; // 50ms Entprellzeit
bool isTagPresent = false; // Speichert, ob ein Tag erkannt ist
unsigned long lastSeenTagTime = 0;  // Zeitstempel der letzten Tag-Erkennung

// Zustandsvariablen für Tasten
int lastPlayStopState = LOW;
int lastLouderState = LOW;
int lastQuieterState = LOW;
long lastTagCode = 0;               // Speichert den Code des letzten Tags
int lastTrackPosition = 0;          // Speichert die letzte Position des Tracks
unsigned long playbackStartTime = 0; // Zeitpunkt, an dem die Wiedergabe gestartet wurde
unsigned long pausedTime = 0;       // Gespeicherte Wiedergabezeit (geschätzt)



unsigned long lastDebounceTimePlayStop = 0;
unsigned long lastDebounceTimeLouder = 0;
unsigned long lastDebounceTimeQuieter = 0;
bool tagRemoved = false;             // Verfolgt, ob das Tag entfernt wurde
unsigned long tagRemoveStartTime = 0; // Zeitstempel, wann das Tag entfernt wurde
const unsigned long tagTimeout = 2000; // 2 Sekunden Timeout für Entfernen


SoftwareSerial mySoftwareSerial(10, 11); // RX, TX
DFRobotDFPlayerMini myDFPlayer;

MFRC522 mfrc522(SDA_PIN, RST_PIN); // RFID

void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();

  Serial.println(F("Initializing Mp3 ... (May take 3~5 seconds)"));
  mySoftwareSerial.begin(9600);
  if (!myDFPlayer.begin(mySoftwareSerial)) {
    Serial.println(F("Error: Mp3 Fehler."));
    while (true);
  } else 
  myDFPlayer.playMp3Folder(1); // Spielt StartUp "0001.mp3" aus dem Ordner "ADVERT" ab
  Serial.println(F("Mp3 online."));
  myDFPlayer.volume(vol);
  

  pinMode(playstop, INPUT_PULLUP); // Interner Pull-Up-Widerstand
  pinMode(louder, INPUT_PULLUP);
  pinMode(quiter, INPUT_PULLUP);
  
}

void loop() {
  unsigned long currentTime = millis();

  // Play/Pause-Taste
  int currentPlayStopState = digitalRead(playstop);
  if (currentPlayStopState != lastPlayStopState && currentPlayStopState == LOW) {
    if (currentTime - lastDebounceTimePlayStop > debounceDelay) {
      lastDebounceTimePlayStop = currentTime;
      isPause = !isPause;
      if (isPause) {
        myDFPlayer.pause();
        Serial.println(F("Pause."));
      } else {
        myDFPlayer.start();
        Serial.println(F("Play."));
      }
    }
  }
  lastPlayStopState = currentPlayStopState;

  // Lauter-Taste
  int currentLouderState = digitalRead(louder);
  if (currentLouderState != lastLouderState && currentLouderState == LOW) {
    if (currentTime - lastDebounceTimeLouder > debounceDelay) {
      lastDebounceTimeLouder = currentTime;
      vol = min(vol + 1, 30);
      myDFPlayer.volume(vol);
      Serial.print(F("Lauter: "));
      Serial.println(vol);
    }
  }
  lastLouderState = currentLouderState;

  // Leiser-Taste
  int currentQuieterState = digitalRead(quiter);
  if (currentQuieterState != lastQuieterState && currentQuieterState == LOW) {
    if (currentTime - lastDebounceTimeQuieter > debounceDelay) {
      lastDebounceTimeQuieter = currentTime;
      vol = max(vol - 1, 0);
      myDFPlayer.volume(vol);
      Serial.print(F("Leiser: "));
      Serial.println(vol);
    }
  }
  lastQuieterState = currentQuieterState;

// Prüfen, ob ein neues Tag in Reichweite ist
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {


    long code = 0;
    for (byte i = 0; i < mfrc522.uid.size; i++) { // Erkennt UID of NFC
      code = ((code + mfrc522.uid.uidByte[i]) * 10);
    }

        if (code == lastTagCode){
    lastSeenTagTime = currentTime; // Aktualisiere die Zeit, in der das Tag zuletzt erkannt wurde
    }

    if (!isTagPresent) {
      if(tagRemoved){
        Serial.print("Wartezeit gestoppt.");
        tagRemoved = false;
      }
      
      isTagPresent = true;
      Serial.print("Die Nummer des Tags ist: ");
      Serial.println(code);

      if (code == lastTagCode) {
        // Selbes Tag erkannt, Wiedergabe fortsetzen
        Serial.println("Selbes Tag erkannt, Wiedergabe wird fortgesetzt.");
        myDFPlayer.start(); // Wiedergabe an der letzten Position fortsetzen
      } else {
        // Neues Tag, Track abspielen
        lastTagCode = code;
            //readCard();

        Serial.println("Neues Tag erkannt, Track wird gestartet.");
         myDFPlayer.stop();
        switch(code){
          case 268187580:
            myDFPlayer.playFolder(1, 1); // Beispiel: Ordner 2, Track 1
            break;
          case 1609320:
            myDFPlayer.playFolder(1, 2); // Beispiel: Ordner 2, Track 2
            break;
          case 1662500:
            myDFPlayer.playFolder(1, 3); // Beispiel: Ordner 2, Track 3
            break;
          break;
        }
        
      }
    }
  }

  // Prüfen, ob das Tag entfernt wurde
  if (isTagPresent && currentTime - lastSeenTagTime > tagTimeout) {
    if (!tagRemoved) {
      tagRemoved = true;
      tagRemoveStartTime = currentTime;
      Serial.println("Tag entfernt, Wartezeit gestartet.");
    }

    // Wenn das Tag länger als 3 Sekunden entfernt ist, Wiedergabe pausieren
    if (tagRemoved && currentTime - tagRemoveStartTime > tagTimeout) {
      isTagPresent = false;
      tagRemoved = false;
      Serial.println("Tag nicht zurückgekehrt, Wiedergabe pausiert.");
      myDFPlayer.pause(); // Wiedergabe pausieren
    }
  }

  }

// zusätzliche Karten Informationen

  void readCard(){
    // UID der Karte ausgeben
  Serial.print("UID der Karte: ");
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    Serial.print(" ");
  }
  Serial.println();

  // Standard-Schlüssel für Authentifizierung
  MFRC522::MIFARE_Key key;
  for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;

  // Block 4 lesen (Beispiel)
  byte block = 4; // Blocknummer
  byte buffer[18];
  byte bufferSize = sizeof(buffer);

  // Authentifizierung für den Sektor des Blocks
  if (mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(mfrc522.uid)) == MFRC522::STATUS_OK) {
    // Block lesen
    if (mfrc522.MIFARE_Read(block, buffer, &bufferSize) == MFRC522::STATUS_OK) {
      Serial.print("Block ");
      Serial.print(block);
      Serial.print(": ");
      for (byte i = 0; i < 16; i++) { // 16 Bytes pro Block
        Serial.print(buffer[i], HEX);
        Serial.print(" ");
      }
      Serial.println();
    } else {
      Serial.println("Fehler beim Lesen des Blocks.");
    }
    mfrc522.PICC_HaltA(); // Karte pausieren
    mfrc522.PCD_StopCrypto1(); // Authentifizierung stoppen
  } else {
    Serial.println("Authentifizierung fehlgeschlagen.");
  }
  }





  

