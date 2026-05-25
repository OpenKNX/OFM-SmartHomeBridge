# OAM-SmartHomeBridge Architektur (AI-tauglich)

## Ziel dieser Doku
Diese Datei beschreibt die Architektur so, dass ein AI-Agent schnell und korrekt Änderungen umsetzen kann.

Wichtigster Grundsatz:
- HomeKitBridge, HueBridge und WebVisuBridge sind Factory- und Orchestrierungs-Schichten.
- Gerätespezifische Logik liegt in den gerätespezifischen Klassen in den Ordnern wie Switch, Dimmer, RGB, Scene, Rolladen, Jalousie, Thermostat, Display, Alarm, Fan, DoorWindow, Media, Lock.

## Architektur in einem Satz
SmartHomeBridgeModule erstellt Kanäle, registriert Bridges und lässt jede Bridge pro Kanal ein passendes Bridge-Gerät erzeugen; die eigentliche Gerätefunktion lebt in den jeweiligen Device-Klassen.

## Schichtenmodell

1. Modul-Schicht
- Datei: lib/OFM-SmartHomeBridge/src/SmartHomeBridgeModule.cpp
- Verantwortlich für Setup, Bridge-Registrierung, Kanal-Erzeugung, Start/Lifecycle.

2. Factory-Bridge-Schicht
- Dateien: HomeKitBridge, HueBridge, WebVisuBridge
- Verantwortlich für createXxx-Methoden und bridge-spezifische Infrastruktur.
- Nicht verantwortlich für gerätespezifische Kernlogik.

3. Geräte-/Channel-Schicht
- Ordner: Switch, Dimmer, RGB, Scene, Rolladen, Jalousie, Thermostat, Display, Alarm, Fan, DoorWindow, Media, Lock
- Hier liegt die eigentliche Gerätefunktion, Statusverarbeitung und Protokoll-Umsetzung.

## Rollen und Verantwortungen

### SmartHomeBridgeModule
- Liest Konfiguration.
- Aktiviert Bridges (HomeKit, Hue, WebVisu).
- Erstellt pro Kanal den passenden KnxChannelXxx.
- Ruft pro Bridge channel->createBridgeDevice(bridge) auf.
- Steuert Lifecycle: initialize -> createChannels -> registerWebPages -> start -> loop.

### HomeKitBridge, HueBridge, WebVisuBridge
- Implementieren createSwitch, createDimmer, usw. als Factory-Einstieg.
- Erzeugen die passenden bridge-spezifischen Device-Objekte (z. B. WebVisuSwitch).
- Registrieren device-nahe Objekte am Channel.
- Halten ggf. bridge-weite Infrastruktur (z. B. Webserver-Routen, HomeKit-/Hue-Stack).
- Keine zentrale fachliche Geräte-Logik duplizieren.

### Geräteklassen in Switch, Dimmer, usw.
- Enthalten gerätespezifische Logik.
- Reagieren auf Kanal-Events.
- Erzeugen bridge-spezifische Payloads/Statusabbilder.
- Kapseln Geräteverhalten je Typ.

## Aktueller WebVisu-Stand

- WebVisuBridge ist für Transport/Websocket/Command-Routing zuständig.
- WebVisuBridge speichert keinen DeviceState und kein Geräte-JSON dauerhaft.
- Snapshot wird on-demand aus aktuellen Kanälen erstellt.
- WebVisuSwitch und WebVisuDimmer halten lokale Originalwerte (z. B. Name, Power, Brightness) und bauen ihr JSON selbst.

## Daten- und Aufruffluss

### Start
1. SmartHomeBridgeModule initialisiert Bridges.
2. Modul erzeugt KnxChannelXxx pro konfiguriertem Kanal.
3. Für jede Bridge wird pro Kanal createXxx aufgerufen.
4. Bridge erzeugt gerätespezifisches Bridge-Objekt und registriert es am Channel.

### Laufzeit
1. Kanalzustand ändert sich durch KNX oder Command.
2. Channel ruft die registrierten Bridge-Geräte auf (z. B. setPower, setBrightness).
3. Bridge-Gerät erzeugt bridge-spezifische Darstellung (z. B. WebVisu JSON).
4. Bridge transportiert das Ergebnis (z. B. broadcastUpdate).

## Regeln für Änderungen (wichtig für AI)

1. Neue Gerätefunktion
- In den passenden Geräteordner implementieren.
- Nicht in HomeKitBridge/HueBridge/WebVisuBridge als Fachlogik implementieren.

2. Bridge-Erweiterung
- In Bridge-Klasse nur Factory, Transport, Lifecycle, Infrastruktur.
- Geräteverhalten in Switch/Dimmer/etc. halten.

3. Duplicate-Logic vermeiden
- Gemeinsame Hilfsfunktionen in Basisklassen oder Shared-Utilities verschieben.
- Beispiel: jsonEscape in WebVisuWidgetBase.

4. Signaturen mit _channelIndex
- Parametername _channelIndex beibehalten, wenn Macro-abhängig.

5. Guards respektieren
- SMARTHOMEBRIDGE_DEVICESONLY
- OPENKNX_WEBSERVER

## Wo welche Änderungen hingehören

- HomeKit-spezifische Infrastruktur: HomeKitBridge.
- Hue/Espalexa-spezifische Infrastruktur: HueBridge.
- Web UI/Websocket/Command Parsing: WebVisuBridge.
- Switch-Verhalten: Switch/*
- Dimmer-Verhalten: Dimmer/*
- Weitere Gerätetypen: jeweiliger Geräteordner.

## Schnellcheck vor Merge

1. Liegt Gerätefachlogik im richtigen Geräteordner?
2. Enthält die Bridge nur Factory/Orchestrierung/Infrastruktur?
3. Kompiliert develop_Adafruit_Feather_ESP32_V2_USB?
4. Sind Guard-Defines eingehalten?
5. Wurden bestehende Channel/Bridge-Patterns beibehalten?
