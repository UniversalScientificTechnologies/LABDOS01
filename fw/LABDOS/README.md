# Loading compiled binaries to MCU


## Avrdude

In case of using the avrdude and mightyCore bootloader we should use following command:

```
avrdude -v -patmega1284p -carduino -P/dev/ttyUSB0 -b115200 -D -Uflash:w:/tmp/arduino_build_743311/LABDOS.ino.hex:i
```
