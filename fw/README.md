## How to update firmware
### Prerequisites

Install avrdude.
```
sudo apt update
```

```
sudo apt -y install avrdude
```

### Programming LABDOS by avrdude

Please provide a correct path to .hex file. 

Also correct name of ttyUSB interface has to be provided.

Then run avrdude.

For 8MHz version
```
avrdude -v -patmega1284p -carduino -P/dev/ttyUSB0 -b57600 -D -Uflash:w:./LABDOS.hex:i
```

For 16MHz version
```
avrdude -v -patmega1284p -carduino -P/dev/ttyUSB0 -b115200 -D -Uflash:w:./LABDOS.hex:i
```

