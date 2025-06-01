<!-- DOC HelpContext="Smarthome-Bridge" -->
# Applikationsbeschreibung Geräte

Über die Gerätedefinition wird definiert, über welche Gruppenadressen die Ansteuerung von KNX-Geräten erfolgt.

<!-- DOC -->
### Verfügbare Geräte

Einstellung, wie viele Geräte verfügbar sind. 

<!-- DOC HelpContext="MorePage" -->
### (mehr)

Über diese Seite können weitere Geräte hinzugefügt werden.

<!-- DOC HelpContext="Schalten" -->
## Ein-/Ausschaltbares Gerät 

Dieser Gerätetyp wird verwendet für Geräte, die in der Auswahl der Gerätetypen nicht zur Verfügung stehen und die über eine einfache Ein- und Ausschaltfunktion verfügen.

<!-- DOC -->
## Steckdose

Dieser Gerätetyp entspricht einer schaltbaren Steckdose. Der Unterschied zu einem "Ein-/Ausschaltbaren Gerät" besteht im Symbolbild in der Visualisierung.

<!-- DOC -->
## Lampe

Der Typ Lampe unterstützt verschiedene Lampenarten.

<!-- DOC -->
### Lampenart

- Ein/Aus  
  Lampen, die ein- und ausgeschaltet werden können.
- Dimmer  
  Lampen, deren Helligkeit über einen Prozentwert gesteuert wird.
- Farbe (RGB)  
  Farblampen, deren Farbe und Helligkeit gesteuert werden kann.

<!-- DOC HelpContext="SwitchOnBehaviorDimmer" -->
### Bei EIN-Befehl (Dimmer)

Diese Einstellung steht nur für Dimmer zur Auswahl.
Hier wird konfiguriert, welche Helligkeit die Lampe beim Einschalten annehmen soll.

Zur Auswahl stehen:

- Letzte Helligkeit  
  Die letzte Helligkeit wird in der Anwendung gespeichert. 
  Dazu muss nach einem Busspannungsausfall die Lampe einmalig manuell auf die gewünschte Helligkeit eingestellt werden.          
- Letzte Helligkeit kleiner 100%  
  Diese Einstellung ist dann sinnvoll, wenn über einen zweiten Einschaltbefehl die Lampe auf 100% gestellt werden soll.
- 100%                         
- 90%                          
- 80%                          
- 70%                          
- 60%                          
- 50%                          
- 40%                          
- 30%                          
- 20%                          
- 10%                          

<!-- DOC HelpContext="SwitchOnBehavior2Dimmer" -->
### Bei EIN-Befehl, wenn aktuelle Helligkeit > 0% (Dimmer)

Über diese Einstellung wird ermöglicht, über einen EIN-Befehl bei bereits eingeschalteter Lampe die Helligkeit zu ändern.

Zur Auswahl stehen:

- Letzte Helligkeit kleiner 100%  
  Diese Einstellung wird meist dann verwendet, wenn mit dem ersten EIN-Befehl die Lampe auf volle Helligkeit gestellt wird.
  So ist es möglich, mit einem weiteren EIN-Befehl die Lampe abzudimmen.
- 100%                         
- 90%                          
- 80%                          
- 70%                          
- 60%                          
- 50%                          
- 40%                          
- 30%                          
- 20%                          
- 10%               

<!-- DOC HelpContext="SwitchOnBehaviorRGB" -->
### Bei EIN-Befehl (Farblampe)

Diese Einstellung steht nur für Farblampen (RGB) zur Auswahl.

- Ein Senden         
- Letzte Farbe  
  Die letzte Farbe wird in der Anwendung gespeichert. 
  Dazu muss nach einem Busspannungsausfall die Lampe einmalig manuell auf die gewünschte Farbe eingestellt werden.                     
- Letzte Farbe, ungleich weiß  
  Diese Einstellung wird meist dann verwendet, wenn mit dem zweiten EIN-Befehl die Lampe auf weiß geschaltet wird.
- Weiß                    
- Dunkel                    
- Rot                       
- Grün                      
- Blau                      
- Gelb                      
- Magenta                   
- Cyan   

<!-- DOC HelpContext="SwitchOnBehavior2RGB" -->
### Bei EIN, wenn aktuelle Helligkeit > 0%

- Letzte Farbe, ungleich weiß  
  Diese Einstellung wird meist dann verwendet, wenn mit dem ersten EIN-Befehl die Lampe auf weiß gestellt wird.
  So ist es möglich, mit einem weiteren EIN-Befehl die Lampe auf eine Farbe zu stellen.
- Weiß                      
- Dunkel                    
- Rot                       
- Grün                      
- Blau                      
- Gelb                      
- Magenta                   
- Cyan  

<!-- DOC -->
### KO für Schalten verwenden

Diese Einstellung steht nur für Farblampen (RGB) zur Auswahl.

- Nein  
  Zum Ausschalten der Lampe wird der RGB-Wert für Schwarz (0) gesendet.
- Ja  
  Zum Ein- und Ausschalten der Lampe wird ein eigenes Gruppenobjekt verwendet. 
 

<!-- DOC -->
## Jalousie

Dieser Gerätetyp wird zur Steuerung von Jalousien mit Lamellen verwendet.

<!-- DOC -->
### Auf/Ab-Objekt verwenden

Normalerweise kann über den Prozent-Ausgang die Steuerung erfolgen. Jedoch wird bei manchen Aktoren die Prozentsteuerung anders behandelt als Auf/Ab. Aus diesem Grund kann über dieses Gruppenobjekt die Ansteuerung von Auf und Ab erfolgen. Diese wird aber nur verwendet, wenn eine vollständige Öffnung oder Schließung erwünscht ist (0%, 100%). In allen anderen Fällen wird der Prozent-Ausgang verwendet.

<!-- DOC -->
### Stopp-Objekt verwenden

Wird diese Einstellung verwendet, wird ein Stopp-Objekt eingeblendet. Über dieses kann eine aktuelle Fahrbewegung gestoppt werden, um einen vorherigen Fahrbefehl vorzeitig zu stoppen.

<!-- DOC -->
### Lamellenposition bei Fahrt

Über diese Einstellung kann definiert werden, in welche Richtung die Lamellen stehen sollen, wenn eine Fahrt erfolgt.

- nicht ändern  
  Die Lamellen behalten die letzte Vorgabe bei. 
  Das bedeutet, nach einer Fahrbewegung werden die Lamellen wieder in die vorherige Stellung gedreht.
- 100% (schließen), wenn Fahrt von 0% Höhe startet  
  In dieser Einstellung werden bei einer Fahrt von 0% auf 100% die Lamellen am Ende in der geschlossenen Stellung beibehalten.
- 100% (schließen) bei Fahrt nach unten, 0% (öffnen) bei Fahrt nach oben  
  Diese Einstellung entspricht dem natürlichen Verhalten der Lamellen bei Fahrbewegungen.  
  D.h. es erfolgt keine Drehung der Lamellen nach der Fahrbewegung.
                                                             
<!-- DOC -->
## Rollladen

Dieser Gerätetyp wird zur Steuerung von Rollläden (ohne Lamellen) verwendet.

<!-- DOCEND -->
### Auf/Ab-Objekt verwenden

Normalerweise kann über den Prozent-Ausgang die Steuerung erfolgen. Jedoch wird bei manchen Aktoren die Prozentsteuerung anders behandelt als Auf/Ab. Aus diesem Grund kann über dieses Gruppenobjekt die Ansteuerung von Auf und Ab erfolgen. Diese wird aber nur verwendet, wenn eine vollständige Öffnung oder Schließung erwünscht ist (0%, 100%). In allen anderen Fällen wird der Prozent-Ausgang verwendet.

<!-- DOCEND -->
### Stopp-Objekt verwenden

Wird diese Einstellung verwendet, wird ein Stopp-Objekt eingeblendet. Über dieses kann eine aktuelle Fahrbewegung gestoppt werden, um einen vorherigen Fahrbefehl vorzeitig zu stoppen.

<!-- DOC -->
## Markise

Dieser Gerätetyp wird zur Steuerung von Markisen oder Pergolas verwendet.

<!-- DOCEND -->
### Auf/Ab-Objekt verwenden

Normalerweise kann über den Prozent-Ausgang die Steuerung erfolgen. Jedoch wird bei manchen Aktoren die Prozentsteuerung anders behandelt als Auf/Ab. Aus diesem Grund kann über dieses Gruppenobjekt die Ansteuerung von Auf und Ab erfolgen. Diese wird aber nur verwendet, wenn eine vollständige Öffnung oder Schließung erwünscht ist (0%, 100%). In allen anderen Fällen wird der Prozent-Ausgang verwendet.

<!-- DOCEND -->
### Stopp-Objekt verwenden

Wird diese Einstellung verwendet, wird ein Stopp-Objekt eingeblendet. Über dieses kann eine aktuelle Fahrbewegung gestoppt werden, um einen vorherigen Fahrbefehl vorzeitig zu stoppen.

<!-- DOC -->
## Thermostat

<!-- DOC -->
## Szene

Dieser Gerätetyp wird zur Aktivierung von KNX-Szenen verwendet.

<!-- DOC -->
### Szenen-Nummer

Szenen-Nummer, die bei einem EIN-Befehl gesendet wird.

<!-- DOC -->
## Media

Dieser Gerätetyp wird auf OpenKNX Displays zur Steuerung eines Media Players verwendet. 
Aktuell wird dieser Gerätetyp in der Smart Home Bridge nicht unterstützt. 

<!-- DOC -->
### Objekt für Mediatitel

Blendet ein Gruppenobjekt ein, das den Titel- oder Quellennamen empfangen kann, der am Display angezeigt werden soll.

<!-- DOC -->
## Anzeige

Dieser Gerätetyp dient zum Visualisieren von Messwerten oder Statusinformationen aus dem KNX-System.

<!-- DOC -->
### Anzeigewert

Folgende Anzeigetypen stehen zur Auswahl:

- Temperatur  
  Anzeige einer Temperatur.     
- Luftfeuchtigkeit  
  Anzeige der Luftfeuchtigkeit.
- Helligkeit    
  Anzeige der Helligkeit. 
- Regenprognose  
  Anzeige einer Regenprognose. 
  Diese kann beispielsweise von dem OpenKNX Modul [OFM-InternetWeatherModule](https://github.com/OpenKNX/OFM-InternetWeatherModule), das in der [OAM-InternetServices](https://github.com/OpenKNX/OAM-InternetServices) enthalten ist, bezogen werden.
  Dieser Anzeigewert wird nur von OpenKNX-Displays unterstützt und steht in der SmartHomeBridge nicht zur Verfügung.
- Schneeprognose  
  Anzeige einer Schneeprognose. 
  Diese kann beispielsweise von dem OpenKNX Modul [OFM-InternetWeatherModule](https://github.com/OpenKNX/OFM-InternetWeatherModule), das in der [OAM-InternetServices](https://github.com/OpenKNX/OAM-InternetServices) enthalten ist, bezogen werden.
  Dieser Anzeigewert wird nur von OpenKNX-Displays unterstützt und steht in der SmartHomeBridge nicht zur Verfügung.
- Wind     
  Anzeige eines Wind-Messwertes in km/h.      
  Dieser Anzeigewert wird nur von OpenKNX-Displays unterstützt und steht in der SmartHomeBridge nicht zur Verfügung.
- Prozent      
  Anzeige eines Prozentwertes.
  Dieser Anzeigewert wird nur von OpenKNX-Displays unterstützt und steht in der SmartHomeBridge nicht zur Verfügung.
- Individuell  
  Dieser Typ erlaubt die Auswahl des Eingangs-Kommunikationsobjektes und die Einstellung einer Einheit für den Messwert.
  Dieser Anzeigewert wird nur von OpenKNX-Displays unterstützt und steht in der SmartHomeBridge nicht zur Verfügung.
- Text    
  Zeigt den Text an, der über ein Text-Kommunikationsobjekt empfangen wird.       
  Dieser Anzeigewert wird nur von OpenKNX-Displays unterstützt und steht in der SmartHomeBridge nicht zur Verfügung.

<!-- DOCEND -->
#### Zusätzliche Einstellungen bei 'Individuell'

<!-- DOC HelpContext="DisplayInputType" -->
##### Eingang

Typ des Kommunikationsobjektes:

- DPT 1.xxx (Schalten)
- DPT 5.xxx (1-Byte-Wert) 
- DPT 5.001 (Prozentwert)
- DPT 6.xxx (1-Byte-Wert mit Vorzeichen)
- DPT 7.xxx (2-Byte-Wert)
- DPT 8.xxx (2-Byte-Wert mit Vorzeichen)
- DPT 9.xxx (2-Byte-Fließkomma)
- DPT 12.xxx (4-Byte-Wert)
- DPT 13.xxx (4-Byte-Wert mit Vorzeichen)
- DPT 14.xxx (4-Byte-Fließkomma)

<!-- DOC -->
##### Einheit

Einheit des Messwertes.
Diese wird zur Anzeige verwendet und kann vom Kommunikationstyp bei Bedarf abweichend sein.
Es erfolgt jedoch keine Umrechnung des Wertes, dies muss bei Bedarf über die Logik implementiert werden.

<!-- DOC -->
## Alarm

Es stehen die unter Typ aufgezählten Alarmtypen zur Verfügung:

<!-- DOC HelpContext="AlarmType" -->
### Typ

- Kontakt
- Bewegungsmelder
- Präsenzmelder
- Leckmelder
- Rauchmelder
- Kohlendioxidmelder (CO2)
- Kohlenmonoxidmelder (CO)

<!-- DOC -->
### Eingang invertieren

Standardmäßig bedeutet ein EIN-Signal am Eingang, dass der Alarm aktiv ist.
Über diese Einstellung kann der Eingangswert vor der Auswertung invertiert werden. 

<!-- DOC -->
## Lüfter

<!-- DOC -->
## Fenster

<!-- DOC -->
## Tür

<!-- DOC -->
## Garagentor


