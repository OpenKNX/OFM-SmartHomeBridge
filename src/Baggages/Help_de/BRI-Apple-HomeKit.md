### Apple HomeKit

Über Apple HomeKit können die KNX Geräte gesteuert werden. Es werden bis zu 149 Geräte unterstützt, jedoch derzeit nur maximal 60 empfohlen. Das Limit ergibt sich aus eine Beschränkung der Anzahl der Accessories bei einem HomeKit Gerät mit maximal 150. Eines davon repräsentiert das Bridge Gerät.

Nachdem die Bridge über ETS programmiert wurde, kann sie mit Apple Homekit verbunden.

- "Gerät hinzufügen" wählen
- "Weitere Optionen..." wählen
- Nun sollte die Bridge sichtbar sein. Den Kopplungscode der in ETS eingestellt wurde (Standardwert 46637726) eingeben und die Meldung das es sich um ein nicht zertifiziertes Gerät handelt bestätigen.
- Danach den Setup-Wizard für alle Geräte durchführen.

Sollte nachträglich eine Gerätetype sich ändern, z.B. eine Lampe wird zu einer Jalousie, oder eine Untertype ändert sich z.B. ein CO2 Sensor wird zu einem Kontakt, muss zuerst das Gerät deaktiviert werden. Danach die in der Home App warten bis das Gerät verschwunden ist und danach kann das Gerät wieder aktiviert werden.

