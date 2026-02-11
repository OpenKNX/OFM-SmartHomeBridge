<!-- DOC HelpContext="Smarthome-Bridge" -->
# Applikationsbeschreibung Smart Home Bridge

Die Smart Home Bridge stellt KNX Geräte wie Lampen, Dimmer, Lüfter usw. als Geräte für andere Systeme zur Verfügung. 

ACHTUNG: Bei WLAN Geräten muss unter Netzwerk / WiFi-Assistent das WLAN richtig konfiguriert werden. 
Bei LAN Geräten ist nichts einzustellen.

## Basiseinstellungen

In den Basiseinstellungen wird die Auswahl der angebunden Smart Home System vorgenommen.

<!-- DOC -->
### Name

Name der Smart Home Bridge.
Bei manchen Smart Home Systemen wird dieser beim Koppeln angezeigt.


<!-- DOCEND -->
Folgende Smart Home Systeme werden unterstüzt:

<!-- DOC -->
#### Apple HomeKit

Über Apple HomeKit können die KNX Geräte gesteuert werden. Es werden bis zu 149 Geräte unterstützt, jedoch derzeit nur maximal 60 empfohlen. Das Limit ergibt sich aus eine Beschränkung der Anzahl der Accessories bei einem HomeKit Gerät mit maximal 150. Eines davon repräsentiert das Bridge Gerät.

Nachdem die Bridge über ETS programmiert wurde, kann sie mit Apple Homekit verbunden.

- "Gerät hinzufügen" wählen
- "Weitere Optionen..." wählen
- Nun sollte die Bridge sichtbar sein. Den Kopplungscode der in ETS eingestellt wurde (Standardwert 46637726) eingeben und die Meldung das es sich um ein nicht zertifiziertes Gerät handelt bestätigen.
- Danach den Setup-Wizard für alle Geräte durchführen.

Sollte nachträglich eine Gerätetype sich ändern, z.B. eine Lampe wird zu einer Jalousie, oder eine Untertype ändert sich z.B. ein CO2 Sensor wird zu einem Kontakt, muss zuerst das Gerät deaktiviert werden. Danach die in der Home App warten bis das Gerät verschwunden ist und danach kann das Gerät wieder aktiviert werden.

<!-- DOC -->
### Hue Emulation (Nur für Alexa)

Die Philips Hue Emulation funktiontioniert nur für Amazon Echo Geräte, d.h. in der Hue App können die Geräte nicht verwendet werden.
Philips Hue unterstützt nur Lampen, für einige Gerätetypen gibt es aber die Möglichkeit diese als Lampen in Alexa anzeigen zu lassen.

Achtung: Die Anbindung wird instabil wenn zu viele Geräte definiert sind. Es empfiehlt sich nicht mehr als etwa 30 Geräte für Philips Hue zu nutzen. Die Anzahl der Geräte die für Homekit verwendet werden spielt dabei keine Rolle.

<!-- DOCEND -->
#### Hue spezifische Eintellungen

Folgende Einstellungen stehen bei Geräten die in HUE verwendet werden zur Verfügung:

<!-- DOC -->
##### Gerät in Hue verwenden

Gibt an, ob das Gerät über die HUE Emulation gefunden und gesteuert werden soll.

<!-- DOC -->
#### In HUE als Lampe darstellen

Die Philips HUE Emulation unterstützt nur Lampen. Geräte mit dieser Einstellung können aber als Lampen in HUE emuliert werde, um trotzdem eine Steuerung über die Smart Home Bridge zu Ermöglichen.

<!-- DOC -->
##### In HUE als dimmbare Lampe darstellen

Die Philips HUE Emulation unterstützt nur Lampen. Geräte mit dieser Einstellung können aber als Dimmbare Lampe in HUE emuliert werde, um trotzdem eine Steuerung über die Smart Home Bridge zu Ermöglichen.

<!-- DOC -->
### Kopplungscode

Über Apple HomeKit können die KNX Geräte gesteuert werden. Es werden bis zu 149 Geräte unterstützt. Das Limit ergibt sich aus eine Beschränkung der Anzahl der Accessories bei einem HomeKit Gerät mit maximal 150. Eines davon repräsentiert das Bridge Gerät.

Nachdem die Bridge über ETS programmiert wurde, kann sie mit Apple Homekit verbunden:

- "Gerät hinzufügen" wählen
- "Weitere Optionen..." wählen
- Nun sollte die Bridge sichtbar sein. Den Kopplungscode der in ETS eingestellt wurde (Standardwert 46637726) eingeben und die Meldung das es sich um ein nicht zertifiziertes Gerät handelt bestätigen.
- Danach den Setup-Wizard für alle Geräte durchführen.

Sollte nachträglich eine Gerätetype sich ändern, z.B. eine Lampe wird zu einer Jalousie, oder eine Untertype ändert sich z.B. ein CO2 Sensor wird zu einem Kontakt, muss zuerst das Gerät deaktiviert werden. Danach die in der Home App warten bis das Gerät verschwunden ist und danach kann das Gerät wieder aktiviert werden.

<!-- DOC HelpContext="Kanalauswahl" -->
# Gerätetypen

Diese Einsellung legt fest, welche Gerätetype verwendet wird.

<!-- DOCEND -->
Zur Auswahl stehen:

- Ein- Ausschaltbares Gerät 
- Steckdose
- Lampe
- Jalousie
- Rolladen
- Markise
- Thermostat
- Anzeige
- Alam
- Lüfter
- Fenster
- Tür
- Garagentor
- Schloss
- Szene

Mehr Informationen über die Geräte die angesteuert werden können, sind unter [Applikationsbeschreibung-Geräte](Applikationsbeschreibung-Geräte) zu finden.



