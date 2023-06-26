# LABDOS01 user manual

## Stand-alone use

Check that LABDOS01 contains an properly formatted SD card, that is mandatory to data logging in stand-alone use. 
In that case, the LABDOS01 needs an external power supply e.g. power bank or smartphone. The LABDOS could be connected by any sufficiently reliable USB-C to USB-C/USB-A cable. In the case of powerbank it is important to check that powerbank does not switch-off after a while. It is common issue with many powerbanks. 

Note: Ony industrial SLC SD cards with properly implemented SPI interface are supported.

### How to check the correct logging

Red Power LED is light on. The blue and green leds flash every ten seconds. 

## Data logging to external device 

Simply plug the LABDOS01 into your computer or tablet USB port using a USB-C cable. The device should appear as a virtua serial line. On a Linux computer, the device should connect without any external drivers. For computers with Windows, you will need to install a driver for [FTDI USB](https://ftdichip.com/drivers/) converter.

![LABDOS01 smartphone connection](LABDOS01A_smartphone-tablet_connection.jpg
 "LABDOS01A connected to a smartphone")

#### Linux

The device should appear as a virtual serial line without any external drivers. Many serial logging tools could be used to log the LABDOS output, for example, `picocom` or `minicom`. Look to [LABDOS01A/sw](https://github.com/UniversalScientificTechnologies/LABDOS01/tree/LABDOS01A/sw) for details. 

#### Android

The device should appear as a virtual serial line without any external drivers. Many serial logging tools could be used to log the LABDOS output, for example, [Serial USB terminal](https://play.google.com/store/apps/details?id=de.kai_morich.serial_usb_terminal).

#### Windows

For computers with Windows, you will need to install a driver for [FTDI USB](https://ftdichip.com/drivers/) converter. Then you could use the [putty](https://www.putty.org/) which is a program that can be used for logging. One of the tutorials on how to set up data logging using Putty is [here](https://my.kualo.com/knowledgebase/?kbcat=0&article=888) for example.

