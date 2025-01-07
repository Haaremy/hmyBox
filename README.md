# hmyBox
TonieBox ist outdated und zu teuer.

## Arduino DIY

### Bauteile Prototyp I
- Arduino (Funduino) Mega 2560
- Breadboard
- Mini MP3 Player DFPlayer Master Module
- RFID-Modul RC522
- 1 - 2 x Lautsprecher 8ohm 3W
- 3 x 2 Pin Drückschalter (Taste)
- 3 x 1k Ohm Widerstand

### Schaltplan Prototyp
![screenshot](Schaltplan_Prototyp.png)
* Im Schaltplan wurde kein Steckboard eingezeichnet. Dieses wurde bspw für die Taster genutzt oder aus Gründen der Kabel-Länge für den RFID-Leser sowie für die Widerstände.

### Code
#### Abhängigkeiten
- DFRobotDFPlayerMini
- MFRC522

### Ordnerstruktur Mp3-Modul
```bash
├── mp3
|   ├── 0001_StartUpSound.mp3
├── 01
|   ├── 001_Story1_Infos.mp3
|   ├── 002_Story2Title.mp3   
├── 02
...
```