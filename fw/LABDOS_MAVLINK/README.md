## Avrdude
Programování pomocí avrdude a mightyCore bootloaderu:
```
avrdude -v -patmega1284p -carduino -P/dev/ttyUSB0 -b115200 -D -Uflash:w:/tmp/arduino_build_743311/LABDOS.ino.hex:i
```
