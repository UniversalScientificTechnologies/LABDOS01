# SPACEDOS03

SPACEDOS03A is an open-source spectrometer-dosimeter based on silicone PIN diode and is intended for scientific and research purposes. Power and communication is secured (optionally) by USB port. The device can be used statically (located in a specific place) or in mobile applications (such as cars). The spectrometer is housed in a 3D printed box that secures mechanical resistance. 

The aim of SPACEDOS03 is to make the accesible, high quality, reliable and simple measuring device - radiation energy spectrometer for sientific comunity. 

SPACEDOS03A is not able to function on its own. It requires a computer to log/visualize the measured data. At least some powering unit. The device is not intended for outdoor use (not waterproof). For these needs, look at [GEODOS01](https://github.com/UniversalScientificTechnologies/GEODOS01).

## Parameters

 * Silicon PIN diode detector with 12.5 mm³ detection volume
 * Number of energy channels 250
 * Deposited energy range from 200 keV to 12 MeV
 * Energy measurement resolution < 50 keV/channel
 * Power supply 5V (trought USB port)
 * Integration time 15 s
 * Interface - USB 2.0, USB-B connector
 * Dimensions - 
 * Weight - 


The device is designed as open-source hardware and software and is released under the GPLv3 license. The device is originlly developed and maintained by [UST (Universal Scientific Technologies)](www.ust.cz) company, which sells it commercially and offers technical support.

If you need a fully automatic device (with internal logging, backup power supply, ..) or device designed for outdoor use without the possibility of connecting power and network you can look at [GEODOS01](https://github.com/UniversalScientificTechnologies/GEODOS01) or contact us by email [support@ust.cz](mailto:support@ust.cz). We can agree on the development of equipment according to your requirements.


## Connection
Connecting the device is very simple. Simply connect the device to your computers USB-2.0 port using a USB A-B cable. The device should appear as a virtual serial line on your computer. On a Linux computer, the device should connect itself. For computers with windows, you will need to install a driver for [FTDI USB]() converter.

## Output protocol
The data output format is defined in the following table. 

### Data readout
We have prepared a simple python script for reading and logging data from SPACEDOS03A spectrograph. 


## Technical details
Device is composed from modules where each modul do the individual function. And they are interconnected with wires. 
 * [USTSIPIN01](modules/USTSIPIN01/)
 * [USBRS232](https://github.com/mlab-modules/USB232R02)


### Interconnection
TODO
