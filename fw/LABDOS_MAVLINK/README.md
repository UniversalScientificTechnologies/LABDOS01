## Avrdude
Programování pomocí avrdude a mightyCore bootloaderu:
```
avrdude -v -patmega1284p -carduino -P/dev/ttyUSB0 -b115200 -D -Uflash:w:LABDOS/build/MightyCore.avr.1284/LABDOS.ino.hex:i
```
