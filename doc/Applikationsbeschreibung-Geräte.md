<!-- DOC HelpContext="Smarthome-Bridge" -->
# Geräte

Über die Gerätedefinition wird festgelegt, über welche Gruppenadressen die Ansteuerung von KNX-Geräten erfolgt.

<!-- DOC -->
### Verfügbare Geräte

Einstellung, wie viele Geräte verfügbar sind.

<!-- DOC HelpContext="MorePage" -->
### (mehr)

Über diese Seite können weitere Geräte hinzugefügt werden.

<!-- DOC -->
### Gerät deaktivieren (Konfiguration bleibt erhalten)

Über diese Einstellung kann ein Gerät deaktiviert werden, ohne die Einstellungen zu verlieren.
Damit kann das Gerät zu Diagnosezwecken deaktiviert werden, sodass keine Telegramme auf den Bus geschickt werden.

<!-- DOC HelpContext="Schalten" -->
## Ein-/Ausschaltbares Gerät

Dieser Gerätetyp wird für Geräte verwendet, die in der Auswahl der Gerätetypen nicht zur Verfügung stehen und die über eine einfache Ein- und Ausschaltfunktion verfügen.

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
  Farblampen, deren Farbe und Helligkeit gesteuert werden können.

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
  Das heißt, es erfolgt keine Drehung der Lamellen nach der Fahrbewegung.
                                                             
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

Dieser Gerätetyp wird zur Steuerung von Heizungsaktoren verwendet.

<!-- DOC HelpContext="ThermostatUnit" --> 
### Einheit

Einheit für die Temperatur.

Zur Auswahl steht:

- Celsius
- Fahrenheit

<!-- DOC HelpContext="ThermostatMode" -->
### Optionen

Auswahl, welche Betriebsart verwendet wird:

- Heizen und Kühlen Automatik
- Heizen oder Kühlen (manuelle Auswahl)
- Heizen
- Kühlen        

<!-- DOCEND -->
Abhängig von der Auswahl stehen für Heizen bzw. Kühlen folgende Einstellungen bereit.

<!-- DOC HelpContext="ThermostatKoType" -->
### Steuer-Objekt-Type

Object, welches zur Steuerung verwendet werden soll.

- Einschalt- oder Sperrobjekt (DPT 1.*)
  Mit dieser Einstellung, können Freigabe- oder Sperrobjekte verwendet werden.
- HVAC
  Es kann ein einzelner Datenpunkt für Heizen, Kühlen, Automatikbetrieb und Ausschalten verwendet werden

<!-- DOC HelpContext="ThermostatKoFeedback" -->
### Rückmeldungs-Objekt-Type

Object, welches als Feedback vom Aktor verwendet wird.

- Einschalt- oder Sperrobjekt (DPT 1.*)
  Mit dieser Einstellung, können Freigabe- oder Sperrobjekte verwendet werden.
- HVAC Kontrollmodus (DPT 20.105)
  Es kann ein einzelner Datenpunkt für Heizen, Kühlen, Automatikbetrieb und Ausschalten gemeldet werden.

<!-- DOC HelpContext="ThermostatKo" --> 
### Betriebsart

Kommunikationsobjekttyp, der für die Aktivierung der Betriebsart verwendet wird:

- Einschaltobjekt (1 bei EIN)
- Sperrobjekt (0 bei EIN)

<!-- DOC HelpContext="ThermostatKoFeedback" --> 
### Betriebsart Rückmeldung

Kommunikationsobjekttyp, der für die Rückmeldung der Betriebsart verwendet wird.
Über dieses Objekt muss gemeldet werden, ob die Betriebsart aktiv ist, unabhängig davon, ob gerade tatsächlich geheizt bzw. gekühlt wird.

- Ein Rückmeldung
- Sperren Aktiv

<!-- DOC HelpContext="ThermostatKoActive" -->
### Heizen/Kühlen aktiv Rückmeldung

Kommunikationsobjekttyp, der für die Rückmeldung verwendet wird, ob Heizen bzw. Kühlen gerade aktiv ist:

- 1 Bit Aktiv
- 1 Byte Stellwert (%)
- Keine (Intern errechnen)
  Der aktuelle Kühl- bzw. Heizstatus wird aus dem Vergleich von Soll- und Isttemperatur errechnet.

<!-- DOC -->
## Szene

Dieser Gerätetyp wird zur Aktivierung von KNX-Szenen verwendet.

<!-- DOC -->
### Szenen-Nummer

Szenen-Nummer, die bei einem EIN-Befehl gesendet wird.

<!-- DOC -->
## Media

Dieser Gerätetyp wird auf OpenKNX-Displays zur Steuerung eines Media Players verwendet. 
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
  Diese kann beispielsweise von dem OpenKNX-Modul [OFM-InternetWeatherModule](https://github.com/OpenKNX/OFM-InternetWeatherModule), das in den [OAM-InternetServices](https://github.com/OpenKNX/OAM-InternetServices) enthalten ist, bezogen werden.
  Dieser Anzeigewert wird nur von OpenKNX-Displays unterstützt und steht in der SmartHomeBridge nicht zur Verfügung.
- Schneeprognose  
  Anzeige einer Schneeprognose. 
  Diese kann beispielsweise von dem OpenKNX-Modul [OFM-InternetWeatherModule](https://github.com/OpenKNX/OFM-InternetWeatherModule), das in den [OAM-InternetServices](https://github.com/OpenKNX/OAM-InternetServices) enthalten ist, bezogen werden.
  Dieser Anzeigewert wird nur von OpenKNX-Displays unterstützt und steht in der SmartHomeBridge nicht zur Verfügung.
- Wind     
  Anzeige eines Wind-Messwertes in km/h.      
  Dieser Anzeigewert wird nur von OpenKNX-Displays unterstützt und steht in der SmartHomeBridge nicht zur Verfügung.
- Prozent      
  Anzeige eines Prozentwertes.
  Dieser Anzeigewert wird nur von OpenKNX-Displays unterstützt und steht in der SmartHomeBridge nicht zur Verfügung.
- Individuell  
  Dieser Typ erlaubt die Auswahl des Eingangs-Kommunikationsobjekts und die Einstellung einer Einheit für den Messwert.
  Dieser Anzeigewert wird nur von OpenKNX-Displays unterstützt und steht in der SmartHomeBridge nicht zur Verfügung.
- Text    
  Zeigt den Text an, der über ein Text-Kommunikationsobjekt empfangen wird.       
  Dieser Anzeigewert wird nur von OpenKNX-Displays unterstützt und steht in der SmartHomeBridge nicht zur Verfügung.

<!-- DOCEND -->
#### Zusätzliche Einstellungen bei 'Individuell'

<!-- DOC HelpContext="DisplayInputType" -->
##### Eingang

Typ des Kommunikationsobjekts:

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

Dieser Gerätetyp wird für Lüfter verwendet.

<!-- DOC -->
### Lüfter hat Automatikmodus

Gibt an, ob der Lüfter einen Automatikmodus unterstützt.

<!-- DOC -->
### Objekt für Automatik

Über diese Einstellung wird konfiguriert, welcher Objekttyp für die Aktivierung des Automatikmodus verwendet werden soll.

Zur Auswahl stehen:

- 1 Bit Automatik (Automatik = EIN)
- 1 Bit Handbetrieb (Automatik = AUS)

<!-- DOC -->
### Objekt für Rückmeldung

Über diese Einstellung wird konfiguriert, welcher Objekttyp für die Rückmeldung des aktuellen Status des Automatikmodus verwendet werden soll.

Zur Auswahl stehen:

- 1 Bit Automatik (Automatik = EIN)
- 1 Bit Handbetrieb (Automatik = AUS)

<!-- DOC -->
## Fenster

Dieser Gerätetyp wird für Fenster mit und ohne Motorantrieb verwendet.

<!-- DOC -->
## Motorantrieb

Mögliche Einstellungen:

- Ja  
  Wenn ein Motorantrieb verwendet werden soll.
- Nein  
  Wenn lediglich ein Meldekontakt vorhanden ist, der den Öffnungsstatus liefert.

<!-- DOC -->
### Steuerungs-Objekt 

Einstellung, ob das Gerät über einen Prozentwert oder lediglich über Öffnen/Schließen gesteuert werden kann.

Mögliche Einstellungen:

- Öffnen/Schließen  
  1 Bit (empfohlen DPT-1.009)
- Prozent  
  1 Byte (empfohlen DPT-5.001)

<!-- DOC -->
## Öffnen/Schließen-Objekt

Diese Einstellung steht nur zur Verfügung, wenn die Steuerung über Prozent erfolgt.
Normalerweise kann über den Prozent-Ausgang die Steuerung vollständig erfolgen. 
Jedoch wird bei manchen Aktoren die Prozentsteuerung anders behandelt als Öffnen/Schließen. 
Aus diesem Grund kann über dieses Gruppenobjekt die Ansteuerung von Öffnen und Schließen erfolgen.
Diese wird aber nur verwendet, wenn eine vollständige Öffnung oder Schließung erwünscht ist (0%, 100%). In allen anderen Fällen wird der Prozent-Ausgang verwendet.

Mögliche Optionen:

- Nicht verwenden
- Öffnen und Schließen
- Nur öffnen senden
- Nur schließen senden    

<!-- DOC -->
### Stopp-Objekt

Stellt ein Kommunikationsobjekt zum Stoppen bereit.
Ist diese Einstellung aktiviert, kann die Fahrbewegung vorzeitig gestoppt werden.

<!-- DOC -->
### Blockadenerkennung

Stellt ein Kommunikationsobjekt bereit, über das eine Blockade des Antriebs gemeldet werden kann.

<!-- DOC -->
### Rückmeldung

Gibt an, welcher Kommunikationsobjekttyp für die Rückmeldung der aktuellen Öffnungsposition verwendet wird.
Mögliche Einstellungen:

- Absolute Position  
  Meldung erfolgt über ein Prozent (DPT 5.001) Kommunikationsobjekt.
- Geöffnet  
  Meldung erfolgt über ein Bit-Objekt. EIN entspricht geöffnet.
- Geschlossen  
  Meldung erfolgt über ein Bit-Objekt. EIN entspricht geschlossen.

<!-- DOC -->
### Öffnungsrichtung

Gibt an, in welche Richtung die Öffnung erfolgt.
Diese Einstellung wird nur in OpenKNX-Anzeigen verwendet, um das Öffnen und Schließen über Symbole richtig darzustellen. 
In der SmartHomeBridge steht die Einstellung nicht zur Verfügung.

<!-- DOC -->
## Tür

Dieser Gerätetyp wird für Türen mit und ohne Motorantrieb verwendet.

<!-- DOCEND -->

Die Tür unterstützt die gleichen Einstellungen wie das [Fenster](#fenster).

<!-- DOC -->
## Garagentor

Dieser Gerätetyp wird für Garagentore mit und ohne Motorantrieb verwendet.

<!-- DOCEND -->

Das Garagentor unterstützt die gleichen Einstellungen wie das [Fenster](#fenster).

