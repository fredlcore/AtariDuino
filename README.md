# AtariDuino
Atariduino Version 2.0

# Kurzbeschreibung

Atariduino ist ein Interface für Erweiterungen aus der Arduino-Welt, einem Einplatinenrechner, der wie unser Atari ebenfalls mit einer 8-Bit CPU läuft. Der große Vorteil des Arduino ist es, dass er sehr weit verbreitet ist, es eine Vielzahl an Erweiterungen gibt und ihn eine große Usergemeinde mit Software versorgt. Über Atariduino lässt sich so ein Zugang zu dieser Welt herstellen und dabei nicht nur Sensoren, LEDs und Relais ansteuern, sondern auch Erweiterungen wie das Ethernet-Shield am Atari verwenden, um sich darüber mit dem Internet zu verbinden.
 
Versuche, unseren kleinen Atari ins Internet zu bringen, gab es zwar ein paar, aber diese sind jedoch wenn überhaupt nur schwer erhältlich und dann meist teurer in der Anschaffung. Ein Ansatz ist dabei, die gesamte Arbeit (den sogenannten TCP/IP Stack) auf dem Atari umzusetzen. Das ist zwar spannend, aber nicht unbedingt der Ansatz, den Atari selber in seinen Geräten verwendet hat. So bringt die Floppy 1050 ja auch eine Menge Eigenintelligenz mit und der Atari muss sich nicht darum kümmern, wie die Diskette formatiert wird, sondern schickt einfach nur den Befehl zum Formatieren der Diskette an das Laufwerk.

Diesen Ansatz verfolgt auch die Internetanbindung über Atariduino: Für den Arduino gibt es bereits eine Ansteuerung des Ethernet-Shields, und diese Software würde theoretisch (weil es beides 8-Bit Architekturen sind) auch auf dem Atari laufen, aber warum diese Arbeit dann nicht dem Arduino überlassen? So muss sich der Atari nicht um den TCP/IP-Stack etc. kümmern, sondern kann seinen Speicher für die Programme nutzen, die dann auf das Internet zugreifen wollen. Darüber hinaus ist auch nicht unbedingt nötig, das umfangreiche TCP/IP-Protokoll als Ganzes umzusetzen, wenn man letztlich nur ganz bestimmte Funktionen auf dem Atari nutzen kann oder will und man dafür Speicherplatz und Herstellungskosten einsparen kann. Diesen Weg geht Atariduino.

Als konkrete Anwendung habe ich daher einen kleinen WWW-Viewer implementiert. Dieser besteht aus sehr überschaubarem Code auf dem Atari und auf dem Arduino (jeweils vielleicht 2-3 Bildschirmseiten) und man kann damit unverschlüsselte, textbasierte Webseiten auf dem Atari anschauen. Da der ATARI-Teil in BASIC geschrieben ist, muss die Übertragungsrate auf 1200 Baud begrenzt werden, was ein bisschen an frühe Akkustikkoppler-Zeiten erinnert. Für weitere Entwicklungen, z.B. in Maschinensprache, kann dann auch eine höhere Geschwindigkeit verwendet werden, ebenso wie eine weitergehende HTML-Unterstützung, damit dann auch Links erkannt werden und „richtiges“ Browsen möglich ist.

Mit wenig mehr Aufwand ließe sich auch eine bidirektionale Kommunikation in Echtzeit zwischen zwei oder mehreren Ataris realisieren, was man z.B. für Multiplayer-Spiele über das Netzwerk nutzen könnte.

Weiterhin existiert die Möglichkeit des Anschlusses über den Joystickport. Damit lassen sich dann auch ganz ohne einen Arduino-Motherboard die Erweiterungskarten (sog. Shields) verwenden und über POKE-Befehle ansteuern. Ein Beispiel findet sich auf der Demo-Diskette.

Eine Demonstration von Atariduino und dem WWW Viewer findet Ihr in einem Video auf meinem YouTube-Kanal unter
https://youtu.be/N1rMdwv8_nE


# Teileliste

-	Atariduino Platine
-	2 x 8polige Buchsenleiste mit mind. 20mm langen Pins
-	2 x 6polige Buchsenleiste mit mind. 20mm langen Pins
-	1 x 2x6polige Pinleiste
-	1 x 9polige Pinleiste
-	1 x DB9 Buchse, 2,54mm Rastermaß
-	2 x BC547 Transistor
-	1 x 100µH Drosselspule
-	1 x 4,7kOhm Widerstand
-	1 x 27kOhm Widerstand
-	2 x 1N4148 Diode
-	1 x 1N5817 Diode
-	2 x 47pF Kondensator
-	1 x 100pF Kondensator
-	1 x Ein/Ausschalter, 3polig, 2,54mm Rastermaß
-	Optional (bei Verwendung an Computern ohne entsprechende Eingangswiderstände am Joystickport): 5 x 220 Ohm Widerstand

# Bauanleitung

Für den Fall, dass man das Board selber zu Hause drucken will, ist bei Verwendung der reinen Eagle-Dateien der Befehl „ratsnest“ einzugeben, um die Ground Plane zu füllen. Bei einem Export in Gerber findet dies automatisch statt.

Die Bauteile entsprechend der Beschriftung auf dem Board einstecken und verlöten. Wenn nur die SIO-Komponente verwendet werden soll, kann die DB9-Buchse, die 9polige Pinleiste und die beim Atari sowieso nicht notwendigen 220 Ohm Widerstände weggelassen werden.
Bei Rechnern, die 220 Ohm an den Joystick Ports aufweisen, sind stattdessen die Lötbrücken SJ1 bis SJ5 zu überbrücken.

Für die SIO-Anbindung ist ein SIO-Kabel aufzuschneiden und die Pins folgendermaßen zu verbinden:
SIO-Port		Atariduino-SIO IN-Pinleiste
10 (+5V)		10>5V
8 (Motor)		8>8
7 (Command)		7>7
5 (Data Out)		5>RX
4 (Ground)		4>GND
3 (Data In)		3>TX

Achtung: Nicht mit der SIO OUT Pinleiste verbinden! Diese stellt die durch den Schaltkreis entsprechend aufbereiteten Signale zur Verbindung mit anderen als den oben genannten Arduino Pins zur Verfügung.

# Ansteuerung

Atariduino kann über zwei Varianten an den Atari angeschlossen und angesteuert werden: Über den SIO-Port oder den Joystick-Port. Da die Internetanbindung als primäre Anwendung nur über den SIO-Port funktioniert, soll darauf das Hauptaugenmerk liegen.

# SIO-Port

Die Verbindung über den SIO-Port erfolgt über den R:Verter Treiber von Bob Puff. Dieser kann mittels XIO-Befehlen konfiguriert werden. Die Konfiguration benötigt lediglich drei Befehle:
OPEN #1, 13, 0, „R:“ – Öffnen von R: auf Kanal 1 im Schreib-/Lesemodus
XIO 40, #1, 0, 0, „R:“ – Aktivierung des „Concurrent Mode“, der gleichzeitig Lesen und Schreiben auf dem Kanal ermöglicht
XIO 36, #1, 0, 0, „R:“ – Einstellen der Übertragungsgeschwindigkeit auf 1200 Baud.
Danach können über PRINT #1 und INPUT #1 Daten über die R:-Schnittstelle und Atariduino mit dem Arduino ausgetauscht werden.
Bei der Anpassung der Firmware ist darauf zu achten, dass eine Datenausgabe an den Atari immer mit einem 0x0D Byte beendet wird.
Um eine abzufragende Webseite über Atariduino abzufragen, muss dazu ein String gesendet werden der mit CTRL+B begonnen wird und mit CTRL+C beendet wird; das sind im Atari jeweils Grafikzeichen, stehen aber im ASCII Code für „Start Text“ und „End Text“ bei einer Datenübertragung und dienen der eindeutigen Abgrenzung des dazwischenliegenden Befehls. Der Befehl für die Abfrage einer Webseite lautet „ATARIDUINO_REQ_WWW=...“, so dass der Befehl für eine Abfrage der Webseite des Abbuc (die sich leider noch nicht im reinen Textmodus sinnvoll anzeigen lässt), folgendermaßen aussehen würde:
„{CTRL+B}ATARIDUINO_REQ_WWW=www.abbuc.de{CTRL+C}“
Zu beachten ist, dass kein Protokoll am Anfang angegeben werden darf, da standarmäßig http verwendet wird. Das verschlüsstelte https-Protokoll ist auf Grund des begrenzten Speichers des Arduino nicht verwendbar.

Weitere Befehle zur Ansteuerung von Sensoren und anderen Erweiterungen können mit wenigen Zeilen in der Firmware hinzugefügt werden, zwei Beispiele sind im Sourcecode bereits enthalten.

Der Anschluss an das Netzwerk ist über DHCP vorgesehen. Nach dem Einschalten des Arduinos über den Schalter an der Atariduino-Platine sollte man etwas warten, damit der Arduino genug Zeit hat, sich über DHCP eine IP-Adresse zu besorgen. Erst dann beginnt er, auf Anfragen auf dem SIO-Port zu lauschen.

# Joystick-Port

Der Vollständigkeit halber sei die Anbindung über den Joystick-Port kurz erklärt: Hierfür ist ein einfaches 1:1-Verbindungskabel nötig, um den Atari an den Arduino anzuschließen. Über diese Verbindung können die digitalen Pins 2-5 angesteuert werden, indem mit den folgenden POKE Befehlen der Joystickport 1 auf allen vier Ausgangspins auf „Ausgang“ geschaltet wird:
POKE 54018,56 : REM Vorbereiten der Ausgangskonfiguration
POKE 54016,15 : REM Alle vier Ausgangspins von Joystickport 1 auf Ausgang schalten
POKE 54018,60 : REM Ausgangskonfiguration beenden
POKE 54016,15 : REM Alle vier Ausgangspins auf „HIGH“
FOR X = 0 TO 200 : NEXT X : REM Kurz warten...
POKE 54016,0 : REM Alle vier Ausgangspins auf “LOW” 

# Troubleshooting / FAQ

-	Der Atari reagiert nicht mehr! Selbst BREAK funktioniert nicht!
o	Das Beispielprogramm in BASIC verwendet dem einfacheren Verständnis wegen die Befehle PRINT und INPUT. Der INPUT-Befehl kehrt im „Concurrent-Mode“, der das gleichzeitige Lesen und Schreiben auf einem Kanal ermöglicht, erst zur weiteren Bearbeitung zurück, wenn er von der Gegenseite das End-Of-Line-Zeichen (0x0D) empfangen hat, zurück. Dies ließe sich über das Einlesen mittels GET verhindern, allerdings ist diese Variante deutlich langsamer als das Empfangen via INPUT, weswegen im Beispielprogramm trotz dieser Einschränkung der Weg über INPUT gewählt wurde.

-	Ich bekomme keine Webseite angezeigt!
o	Es kann sein, dass der Arduino Probleme hat, per DHCP eine Internetadresse zu bekommen. Diese Anfrage nach einer IP-Adresse hat einen sehr langen Timeout (bis zu 2 Minuten), so dass es danach aussehen kann, dass nichts mehr reagiert. Im Zweifelsfall kann man den Arduino über den kleinen Reset-Knopf auf dem Ethernet-Shield oder auf dem Arduino selber resetten, er versucht dann erneut eine Adresse zu bekommen.

-	Es werden wirre Zeichen dargestellt
o	Manche Webseiten bestehen eigentlich aus Bildern oder auch aus Unicode-Zeichen, die dann beim Atari Grafik- oder Steuerzeichen sein können. Webseiten mit reinem 7-Bit ASCII sind daher für die Anzeige am besten geeignet.
-	Geht das auch mit WLAN?
o	Indirekt ja. Entweder schließt man das Ethernet-Shield an eine WLAN-Bridge an, die es von TP-Link für 10-20 Euro gibt, oder man verwendet statt des Ethernet-Shields ein Wifi-Shield. Dafür müsste das Arduino-Script entsprechend dieser Anleitung angepasst werden:
https://www.arduino.cc/en/Guide/ArduinoWiFiShield#toc7
 
# Pinbelegungen

# SIO-Port

Die Pins des SIO-Ports werden wie folgt mit den nachfolgend genannten Pins des Arduino verbunden:

SIO Port		Arduino Pin
10 (+5V)		5V
8 (Motor)		Digital Pin 8
7 (Command)		Digital Pin 7
5 (Data Out)		Digital Pin 0
4 (Ground)		GND
3 (Data In)		Digital Pin 1

Falls bereits existierende Software (wie z.B. SIO2Arduino) bestimmte Signale an anderen Pins erwartet, können diese über die SIO OUT Pinleiste auf den gewünschten Pin aufgeschaltet werden. Das Signal hat an SIO OUT den entsprechenden Schaltkreis bereits durchlaufen und es sind keine weiteren Bauteile nötig.

# Joystick-Port

Die Pins des Joystick Ports werden wie folgt mit den nachfolgend genannten Pins des Arduino verbunden:

Joystick Port		Arduino Pin
1 (Up)			Digitaler Pin 2
2 (Down)		Digitaler Pin 3
3 (Left)			Digitaler Pin 4
4 (Right)		Digitaler Pin 5
5 (Paddle B)		Nicht verbunden 
6 (Fire)			Nicht verbunden
7 (5V)			5V
8 (GND)		GND
9 (Paddle A)		Nicht verbunden

Es sind standardmäßig nur die als Ausgänge konfigurierbaren Pins des Joystickports mit den oben genannten Pins des Arduino verbunden. Die nicht verbundenen Pins, die nur als Eingänge am Atari verwendet werden (Fire, Paddles) können auf Wunsch über die Pinleiste mit beliebigen Pins des Arduino verbunden werden, um von dort aus Signale an den Atari zu senden.
