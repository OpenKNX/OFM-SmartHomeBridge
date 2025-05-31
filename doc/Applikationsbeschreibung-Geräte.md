<!-- DOC HelpContext="Smarthome-Bridge" -->
# Applikationsbeschreibung Geräte

Über die Gerätedefinition wird definitiert, über welche Gruppenadressen die Ansteuerung von KNX-Geräten erfolgt.

<!-- DOC -->
### Verfügbare Geräte

Einstellung wie viele Geräte verfügbar sind. 

<!-- DOC HelpContext="MorePage" -->
### (mehr)

Über diese Seite können weiter Geräte hinzugefügt werden.

<!-- DOC HelpContext="Kanalauswahl" -->
# Gerätetypen

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
- Szene

<!-- DOC HelpContext="Schalten" -->
## Ein-/Ausschaltbares Gerät 

Diese Gerätetype wird verwendete für Geräte die in der Auswahl der Gerätetypen nicht zur Verfügung steht, und die über eine einfaches Ein- und Ausschaltfunktion verfügen.

<!-- DOC -->
## Steckdose

Diese Gerätetype entspricht einer schaltbaren Steckdosen. Der Unterschied zu einem "Ein-/Ausschaltbarem Gerät" besteht im Symbolbild in der Visualisierung.

<!-- DOC -->
## Lampe

Die Type Lampe unterstützt verschiedene Lampenarten.

<!-- DOC -->
### Lampenart

- Ein/Aus
  Lampen, die ein- und ausgeschalten werden können
- Dimmer
  Lampen, dessen Helligkeit über einen Prozentwert gesteuert wird
- Farbe (RGB)
  Farblampe, dessen Farbe und Helligkeit gesteuert werden kann.

<!-- DOC HelpContext="SwitchOnBehaviorDimmer" -->
### Bei EIN Befehl (Dimmer)

Diese Einstellung steht nur für Dimmer zur Auswahl.
Hier wird konfiguriertiert, welche Helligkeit die Lampe beim Einschalten annehmen soll.

Zur Auswahl steht:

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
### Bei EIN Befehl wenn aktuelle Helligkeit > 0% (Dimmer)

Über diese Einstellung wird ermöglich, über einen EIN Befehl bei bereits eingeschaltener Lampe, die Helligkeit zu ändern.

Zur Auswahl steht:

- Letzte Helligkeit kleiner 100%
  Diese Einstellung wird meist dann verwendet, wenn mit dem ersten EIN Befehlt die Lampe auf volle Helligkeit gestellt wird.
  So ist es möglich, mit einem weiteren EIN Befehl die Lampe abzudimmen.
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
### Bei EIN Befehl (Farblampe)

Diese Einstellung steht nur für Farblampen (RGB) zur Auswahl.

- Ein Senden         
- Letzte Farbe  
  Die letzte Farbe wird in der Anwendung gespeichert. 
  Dazu muss nach einem Busspannungsausfall die Lampe einmalig manuell auf die gewünschte Farbe eingestellt werden.                     
- Letzte Farbe, ungleich weiß
  Diese Einstellung wird meist dann verwendet, wenn mit dem zweiten EIN Befehlt die Lampe auf weiß geschalten wird.
- Weiß                    
- Dunkel                    
- Rot                       
- Grün                      
- Blau                      
- Gelb                      
- Magenta                   
- Cyan   

<!-- DOC HelpContext="SwitchOnBehavior2RGB" -->
### Bei EIN wenn aktuelle Helligkeit > 0%

- Letzte Farbe, ungleich weiß
  Diese Einstellung wird meist dann verwendet, wenn mit dem ersten EIN Befehlt die Lampe auf weiß gestellt wird.
  So ist es möglich, mit einem weiteren EIN Befehl die Lampe auf eine Farb zu stellen.
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
  Zum Ausschalten der Lampe wird der RGB Wert für Schwarz (0) gesendet.
- Ja
  Zum Ein- und Ausschalten der Lampe dir ein eigenes Gruppenobjekt verwendet. 
 

<!-- DOC -->
## Jalousie

Diese Gerätetype wird zur Steuerung von Jalousie mit Lamellen verwendet.

<!-- DOC -->
### Auf/Ab Objekt verwenden

Normalerweise kann über den Prozent Ausgang die Steuerung erfolgen. Jedoch wird bei manchen Aktoren die Prozentsteuerung anderes behandelt, als Auf/Ab. Aus diesem Grund, kann über diese Gruppenobjekt die Ansteuerung von Auf und Ab erfolgen. Diese wird aber nur verwendet, wenn eine vollständige Öffnung oder Schließung erwünscht ist (0%, 100%). In allen anderen Fällen, wird der Prozent Ausgang verwendet.

<!-- DOC -->
### Stopp Objekt verwenden

Wird diese Einstellung verwendet, wird ein Stopp Objekt eingeblendet. Über dieses kann eine aktuelle Fahrbewegung gestoppt werden um eine vorherigen Fahrbefehl vorzeitig zu stoppen.

<!-- DOC -->
### Lamellenposition bei Fahrt

Über diese Einstellung, kann definiert werden, in welche Richtung die Lamellen stehen sollen, wenn ein Fahrt erfolgt.

- nicht ändern
  Die Lamellen behalten die letzte Vorgabe bei. 
  Das bedeutet, nach einer Fahrbewegung werden die Lamellen wieder in die vorherige Stellung gedreht.
- 100% (schließen) wenn Fahrt von 0% Höhe startet
  In dieser Eisntellung werden bei einer Fahrt von 0% auf 100% die Lamellen am Ende in der geschlossen Stellung beibehalten.
- 100% (schließen) bei Fahrt nach unten, 0% (öffnen) bei Fahrt nach oben
  Diese Einstellung entspricht dem natürlichen Verhalten der Lamellen bei Fahrbewegungen.  
  D.H. es erfolgt keine Drehung der Lamellen nach der Fahrbewegung.
                                                             
<!-- DOC -->
## Rolladen

Diese Gerätetype wird zur Steuerung von Rolladen (Ohne Lamellen) verwendet.

<!-- DOCEND -->
### Auf/Ab Objekt verwenden

Normalerweise kann über den Prozent Ausgang die Steuerung erfolgen. Jedoch wird bei manchen Aktoren die Prozentsteuerung anderes behandelt, als Auf/Ab. Aus diesem Grund, kann über diese Gruppenobjekt die Ansteuerung von Auf und Ab erfolgen. Diese wird aber nur verwendet, wenn eine vollständige Öffnung oder Schließung erwünscht ist (0%, 100%). In allen anderen Fällen, wird der Prozent Ausgang verwendet.

<!-- DOCEND -->
### Stopp Objekt verwenden

Wird diese Einstellung verwendet, wird ein Stopp Objekt eingeblendet. Über dieses kann eine aktuelle Fahrbewegung gestoppt werden um eine vorherigen Fahrbefehl vorzeitig zu stoppen.

<!-- DOC -->
## Markise

Diese Gerätetype wird zur Steuerung von Markisen oder Pergulas verwendet.

<!-- DOCEND -->
### Auf/Ab Objekt verwenden

Normalerweise kann über den Prozent Ausgang die Steuerung erfolgen. Jedoch wird bei manchen Aktoren die Prozentsteuerung anderes behandelt, als Auf/Ab. Aus diesem Grund, kann über diese Gruppenobjekt die Ansteuerung von Auf und Ab erfolgen. Diese wird aber nur verwendet, wenn eine vollständige Öffnung oder Schließung erwünscht ist (0%, 100%). In allen anderen Fällen, wird der Prozent Ausgang verwendet.

<!-- DOCEND -->
### Stopp Objekt verwenden

Wird diese Einstellung verwendet, wird ein Stopp Objekt eingeblendet. Über dieses kann eine aktuelle Fahrbewegung gestoppt werden um eine vorherigen Fahrbefehl vorzeitig zu stoppen.

<!-- DOC -->
## Thermostat

<!-- DOC -->
## Szene

Diese Gerätetype wird zur aktivierung von KNX-Szenen verwendet.

<!-- DOC -->
### Szene Nummer

Szene Nummer die bei einem EIN Befehl gesendet wird.

<!-- DOC -->
## Media

Diese Gerätetype wird auf OpenKNX Displays zur Steuerung eines Media Players verwendet. 
Aktuelle wird diese Gerätetype in der Smart Home Bridge nicht unterstützt. 

<!-- DOC -->
### Objekt fuer Mediatitel

Blendet ein Gruppenobjekt ein, dass den Titel- oder Quellennamen empfangen kann, der am Display angezeigt werden soll.

<!-- DOC -->
## Anzeige

<!-- DOC -->
## Kontakt

<!-- DOC -->
## Bewegungsmelder

<!-- DOC -->
## Präsensmelder

<!-- DOC -->
## Leckmelder

<!-- DOC -->
## Rauchmelder

<!-- DOC -->
## Kohlendioxidmelder (CO2)

<!-- DOC -->
## Kohlenmonoxidmelder (CO)

<!-- DOC -->
## Lüfter

<!-- DOC -->
## Fenster

<!-- DOC -->
## Tür

<!-- DOC -->
## Garagentor


