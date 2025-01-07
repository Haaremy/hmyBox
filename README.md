# hmyBox
TonieBox ist outdated und zu teuer.

## Arduino DIY Prototyp
### Funktionsweise
Jeder NFC-Tag hat eine Unique ID. Diese ID liest das RFID Gerät aus und anhand der ID wird in Zeile ~146  die Entsprechende Audio abgespielt. Der Code ab Zeile ~184 ist derzeit ungenutzt und soll in späteren Versuchen die NFC Blöcke lesen (Inhalt des Chips). In einem Probelauf konnte ich die mitgelieferten NFC Tags sowie einen Nintendo Amiibo zur Steuerung der Musik nutzen. Der Amiibo konnte entsprechend im unteren Part nicht ausgelesen werden, da er verschlüsselt ist. Ziel ist es dann einen Tag zu haben, diesen auszulesen und anhand der Nummer/n auf dem NFC die entsprechende Datei zu spielen. Exemplarisch Order 2 Lied 2 (siehe Mp3 Ordnerstruktur).

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