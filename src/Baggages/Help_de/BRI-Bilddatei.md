### Bilddatei

Bilddateien im PNG Format müssen zuerst in das Gerät übertragen werden.
Dies kann am Einfachsten über den USB Bildupdload erfolgen.

- das Gerät mit dem USB-Port des Computers verbinden
- Doppelklick auf die PROG Taste
- -> Nun sollte ein Laufwerk sichtbar werden und die PROG Led leuchten
- In das Verzeichnis "Inbox" die Bilddatein kopieren
- Das Laufwerk auswerfen
- -> Die PROG LED beginnt zu flackern als Signal, dass die Datein übertragen werden

Wenn ein Terminal mit dem Gerät verbunden ist, kann man über das Kommando "fs" den Inhalt des Dateisystems prüfen.
Hinweis: Das Laden der Bilddatei ignoriert die Groß- Kleinschreibung.

Alternativer Upload über VS-Code:
- Das Projekt https://github.com/OpenKNX/OAM-TouchRound.git ausgechecken
- In Visual Studio Code mit installierter Extension "PlatformIO" den Ordner öffnen
- Die Konfiguration "env:release_RP2350" in der Fußzeile auswählen.
- In den Order "data" die gewünschten Datein kopieren.
- Das Plugin PlatformIO auf der linken Seiten auswählen
- Datein über "Upload File System" hochladen
!PlatformIO File Upload

Eine sehr gute Auswahl an monochromen Bildern die für Statuseinfärbungen verwendet werden können sind die Material Design Icons
https://pictogrammers.com/library/mdi/.
Beim Export den Advanced Export verwendet und die in der ETS bei der Bilddatei angegebene Pixelanzahl verwenden.

