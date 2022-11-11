# LABDOS01 - Scientific, semiconductor based ionising radiation spectrometer

![LABDOS01 back panel](/doc/LABDOS01.jpg "LABDOS01A on table")

LABDOS01 is an open-source spectrometer-dosimeter based on silicon PIN diode and is intended for scientific research and experimental purposes. Power and communication is secured by USB-C port or JST-GH connector. The device can be used statically (located in a specific place e.g. laboratory or base) or in mobile applications (such as cars or UAV). The spectrometer is housed in a 3D printed box, which brings basic mechanical resistance and allows future development of user enclosures and integrations.

The aim of LABDOS01 is to make the open-source, accesible, high quality, reliable and simple measuring device - radiation energy spectrometer for sientific comunity.

LABDOS01 to function on its own. It requires a computer to visualize recorded measured data. At least it requires an external powering unit. The device is not intended for outdoor use (it is not waterproof).
Intead of that it is intended to be used as experimental device with the exactly the same internals as its application specific variants like:

 * [GEODOS01](https://github.com/UniversalScientificTechnologies/GEODOS01) - Outdoor and stand-alone onizing radiation detector
 * [AIRDOS02](https://github.com/UniversalScientificTechnologies/AIRDOS02) - Airborne cosmic radiation dosimeter with GPS
 * [SPACEDOS02](https://github.com/UniversalScientificTechnologies/SPACEDOS02) - PIN diode dosimeter for manned spacecraft
 * [SPACEDOS01](https://github.com/UniversalScientificTechnologies/SPACEDOS01) - cubesat cosmic radiation dosimeter and spectrometer

LABDOS01 is therefore all-in-one solution for generic semiconductor based ionising radiation measurement. It should be used as universal testing device prior using an application specific design.

## Where to get it?

LABDOS01 is commercially available from [Universal Scientific Technologies s.r.o.](https://www.ust.cz/), write an email to sale@ust.cz or shop at [Tindie store](https://www.tindie.com/products/ust/labdos01-open-source-laboratory-dosimeter/).
The device is designed as open-source hardware and software and is released under the GPLv3 license. The device is originaly developed and maintained by [UST (Universal Scientific Technologies s.r.o.)](https://www.ust.cz) company, which sells it commercially and offers technical support.

## Parameters

 * Silicon PIN diode detector with 12.5 mm³ detection volume
 * Number of energy channels 250, but configurable by firmware in range
 * Deposited energy range from 200 keV to 12 MeV
 * Energy measurement resolution up  to 50 keV/channel but exact value depends on firmware and analog front-end setup.
 * Power supply 5V (by using USB port or JST-GH connectors)
 * Integration time depends on firmware setup
 * Interface - USB 2.0, USB-C connector or 3.3V UART link on JST-GH connector ([Pixhawk telemetry port](https://github.com/pixhawk/Pixhawk-Standards/blob/master/DS-009%20Pixhawk%20Connector%20Standard.pdf)).
 * Dimensions - 96 x 56 x 19 mm
 * Weight -

Device uses [USTSIPIN02](https://github.com/ust-modules/USTSIPIN02) which is core of range of UST dosimeters specialized to specific applicaion like AIRDOS or SPACEDOS.

## Connection

Simply plug-in the LABDOS01A to your computer or tablet USB port using a USB-C cable. The device should appear as a virtual serial line. On a Linux computer, the device should connect without any external drivers. For computers with windows, you will need to install a driver for [FTDI USB](https://ftdichip.com/drivers/) converter.


![LABDOS01 smartphone connection](/doc/LABDOS01A_smartphone-tablet_connection.jpg
 "LABDOS01A connected to a smartphone")

This usage case is especially suitable for interactive testing of silicon-based detector setup. E.g. experiments on accelerators, short-term flights or balloon or UAV experiments where external logging unit is available.

### Output data format

Te output message types are described in following paragraphs. The exact format of each message depends on application-specific firmware. The firmware could be tunned to specific usage case by modifying the [arduino code](https://github.com/UniversalScientificTechnologies/LABDOS01/tree/LABDOS01A/fw). The firmware itself could be updated using the bootloader.

#### Initial message

```
#Hmmm...
```
The initial message is sent as soon as the processor is restarted, and reading it correctly indicates that the connection is well set up.

#### About message

As a second message, when turned on, a string is sent that uniquely identifies the device.

```
$DOS,LABDOS01A,L02,256,379276a,1290c00806a200914056a000a0000086
```
* `$DOS` - is first character of message string
* `LABDOS01A` - Name of device. In this case it is LABDOS
* `L02`- FW version
* `256` - ADC offset
* `379276a` - Hash of commit with of this firmware
* `1290c00806a200914056a000a0000086` - Unique serial namber of LABDOS dosemeters

#### Data message

```
$HIST,47,521,46077,0,256,0,45922,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
```

* `$HIST` - Marking a message with spectrums
* `count` - Message number since restart or power-up
* `time` - Time in seconds from power-up
* `suppress` -
* `dose` - Number of detected particles
* `offset` - ADC offset. Specifies the zero channel position.
* `energetic channels` - All remaining values indicate a certain energy channel. From the smallest to the largest. Third value is 1st channel. 

### Data logging

The data is sent according to a very simple protocol, which is sent in text form. Other option is logging to integrated SDcard see "Stand-alone use" section.

#### Linux/Android

The device should appear as a virtual serial line without any external drivers. There are many serial logging tools which could be used to log the LABDOS output, as example picocom or minicom. Look to [LABDOS01A/sw](https://github.com/UniversalScientificTechnologies/LABDOS01/tree/LABDOS01A/sw) for details. 

#### Windows

For computers with windows, you will need to install a driver for [FTDI USB](https://ftdichip.com/drivers/) converter. Then you could use the [putty](https://www.putty.org/) which is a program can be used for logging. One of the tutorials on how to set up data logging using putty is [here](https://my.kualo.com/knowledgebase/?kbcat=0&article=888) for example.

#### Stand-alone use

Device contains SDcard, which could be used do data logging in stand-alone use. In that case the LABDOS01 needs an external power supply.
That mode could be used for short-term demonstrating of SPACEDOS, AIRDOS or GEODOS variants of that device.  
