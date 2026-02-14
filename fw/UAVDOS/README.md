# LABDOS01 firmware suitable for connection to UAV platform

The LABDOS01 loaded with that firmware could output [MAVLink messages](https://en.wikipedia.org/wiki/MAVLink) suitable for interfacing to the [TF-ATMON](https://www.thunderfly.cz/tf-atmon.html) system. This design is now replaced by dedicated hardware [AIRDOS03](https://docs.dos.ust.cz/airdos/AIRDOS03).

### Loading compiled binaries to MCU


#### Avrdude

In case of using the avrdude and mightyCore bootloader, we should usethe  following command:

```
avrdude -v -patmega1284p -carduino -P/dev/ttyUSB0 -b115200 -D -Uflash:w:/tmp/arduino_build_743311/LABDOS.ino.hex:i
```
