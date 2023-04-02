<a href="https://certification.oshwa.org/cz000008.html" title="Open Source Hardware Association Certificate"><img align="right" src="https://raw.githubusercontent.com/oshwa/certification-mark-generator/gh-pages/premade/certification-template.svg" alt="Open Source Hardware Association Certificate"></a>

# LABDOS01 - Scientific, semiconductor-based ionizing radiation spectrometer

[![Build firmware](https://github.com/UniversalScientificTechnologies/LABDOS01/actions/workflows/build_fw.yml/badge.svg)](https://github.com/UniversalScientificTechnologies/LABDOS01/actions/workflows/build_fw.yml)


![LABDOS01 back panel](/doc/LABDOS01.jpg "LABDOS01A on table")

LABDOS01 is an open-source spectrometer-dosimeter based on a silicon PIN diode and is intended for scientific research and experimental purposes. Power and communication are secured by a USB-C port or JST-GH connector. The device can be used statically (located in a specific place e.g. laboratory or base) or in mobile applications (such as cars or UAVs). The spectrometer is housed in a 3D printed box, which brings basic mechanical resistance and allows future development of user enclosures and integrations.

LABDOS01 aims to make an open-source, accessible, high-quality, reliable, and simple measuring device - a radiation energy spectrometer for the scientific community.

LABDOS01 to function on its own. It requires a computer to visualize recorded measured data. At least it requires an external powering unit. The device is not intended for outdoor use (it is not waterproof).
Instead of that, it is intended to be used as an experimental device with the same internals as its application-specific variants like:

 * [GEODOS01](https://github.com/UniversalScientificTechnologies/GEODOS01) - Outdoor and stand-alone ionizing radiation detector
 * [AIRDOS02](https://github.com/UniversalScientificTechnologies/AIRDOS02) - Airborne cosmic radiation dosimeter with GPS
 * [SPACEDOS02](https://github.com/UniversalScientificTechnologies/SPACEDOS02) - PIN diode dosimeter for manned spacecraft
 * [SPACEDOS01](https://github.com/UniversalScientificTechnologies/SPACEDOS01) - CubeSat cosmic radiation dosimeter and spectrometer

LABDOS01 is therefore the all-in-one solution for generic semiconductor-based ionizing radiation measurement. It should be used as a universal testing device before using an application-specific design.

## Where to get it?

LABDOS01 is commercially available from [Universal Scientific Technologies s.r.o.](https://www.ust.cz/), write an email to sale@ust.cz or shop at [Tindie store](https://www.tindie.com/products/ust/labdos01-open-source-laboratory-dosimeter/).
The device is designed as open-source hardware and software and is released under the GPLv3 license. The device is originally developed and maintained by [UST (Universal Scientific Technologies s.r.o.)](https://www.ust.cz) company, which sells it commercially and offers technical support.

## Parameters

 * Silicon PIN diode detector with 12.5 mm³ detection volume
 * Number of energy channels 250, but configurable by firmware in range
 * Deposited energy range from 200 keV to 12 MeV
 * Energy measurement resolution up to 50 keV/channel but the exact value depends on firmware and analog front-end setup.
 * Power supply 5V (by using the USB port or JST-GH connectors)
 * Integration time depends on firmware setup
 * Interface - USB 2.0, USB-C connector or 3.3V UART link on JST-GH connector ([Pixhawk telemetry port](https://github.com/pixhawk/Pixhawk-Standards/blob/master/DS-009%20Pixhawk%20Connector%20Standard.pdf)).
 * Dimensions - 96 x 56 x 19 mm
 * Weight - 74 grams

## Applications

* AeroSpace or Terrestrial Radiation Monitoring
* Aircraft Onboard Radiation Monitoring
* Scientific High Altitude Balloons, e.g. [Pfotzer Maximum measurement](https://en.wikipedia.org/wiki/Georg_Pfotzer)
* Educational Toolkit, [cosmic ray monitoring](https://en.wikipedia.org/wiki/Cosmic_ray) 
* Radiation Mapping in 3D together with GNSS and UAV
* Space Weather Monitoring e.g on high altitude observatories 
* [Citizen science](https://en.wikipedia.org/wiki/Citizen_science)

The device uses [USTSIPIN02](https://github.com/ust-modules/USTSIPIN02) which is the core of a range of UST dosimeters specialized to the specific application like [AIRDOS](https://www.ust.cz/UST-dosimeters/AIRDOS/) or [SPACEDOS](https://www.ust.cz/UST-dosimeters/SPACEDOS/) or [GEODOS](https://www.ust.cz/UST-dosimeters/GEODOS/).

## Usage examples

### Dosemeter for frequent-flyers

The LABDOS could be easily used as a cosmic radiation dosemeter on board aircraft. In this use case, the LABDOS measures the secondary cosmic radiation generated in the atmosphere or the aircraft fuselage. There is a calculation of the dose rate in silicon for the example flight.

![LABDOS01 smartphone connection](/doc/img/altitude_doserate_LABDOS01.png)

The measured data could be compared with a CARI numerical model, as could be seen in the following graph. 

![LABDOS01 smartphone connection](/doc/img/LABDOS01_CARI.png)

The sum is photons+electrons+protons+positrons. The difference between the sum value and the total value is mostly caused by muons.
If we consider some shielding (the LABDOS sensor is covered by 35 um of copper foil) the agreement is clear. It is also visible that the measured data contains some more information on the fluctuations, which is missing in the numerical model.

## Connection

Simply plug the LABDOS01A into your computer or tablet USB port using a USB-C cable. The device should appear as a virtual serial line. On a Linux computer, the device should connect without any external drivers. For computers with windows, you will need to install a driver for [FTDI USB](https://ftdichip.com/drivers/) converter.


![LABDOS01 smartphone connection](/doc/LABDOS01A_smartphone-tablet_connection.jpg
 "LABDOS01A connected to a smartphone")

This usage case is especially suitable for interactive testing of silicon-based detector setups. E.g. experiments on accelerators, short-term flights, or balloon or UAV experiments where an external logging unit is available.

### Output data format

The output message types are described in the following paragraphs. The exact format of each message depends on application-specific firmware. The firmware could be tuned to specific usage cases by modifying the [arduino code](https://github.com/UniversalScientificTechnologies/LABDOS01/tree/LABDOS01A/fw). The firmware itself could be updated using the bootloader.
The baudrate used by USB port by default is 115200. 

#### Initial messages

```
#Cvak...
#Hmmm...
#Filesize,2173540
#Filename,3.txt
```
The initial messages is sent as soon as the processor is restarted, and reading it correctly indicates that the connection is well set up.
Filesize and Filename is information about a file in case a proper SD card is inserted.

#### About message

As a next message, when turned on, a string is sent that uniquely identifies the device.

```
$DOS,LABDOS01A,L02,256,379276a,1290c00806a200914056a000a0000086
```
* `$DOS` - is the first character of the message string
* `LABDOS01A` - Name of the device. In this case, it is LABDOS
* `L02`- FW version
* `256` - ADC DC offset
* `379276a` - Hash of commit with this firmware
* `1290c00806a200914056a000a0000086` - Unique serial number of LABDOS dosemeters

#### Data message

```
$HIST,0,7.94,517,1,13297,48790,2914,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0```
```

* `$HIST` - Marking a message with spectrums (histograms)
* `count` - Meassuring number since restart or power-up; Restart can be andled by toggling of DTR signal on an asynchronnous interface
* `time` - Time in seconds from power-up
* `suppress` - Number of filtered (omitted) ionizing radiation events, prevention of double detections caused by asynchronous sampling 
* `flux` - Number of detected particles per meassuring interval (6.88 s)
* `energetic channels` - All remaining values indicate a certain energy channel from the smallest to the largest deposited energy; The 4th value is 1st channel without noise 

### Data logging

The data is sent according to a very simple protocol, which is sent in text form. Another option is logging into the integrated SDcard, see the "Stand-alone use" section.

#### Linux/Android

The device should appear as a virtual serial line without any external drivers. Many serial logging tools could be used to log the LABDOS output, for example, `picocom` or `minicom`. Look to [LABDOS01A/sw](https://github.com/UniversalScientificTechnologies/LABDOS01/tree/LABDOS01A/sw) for details. 

#### Windows

For computers with windows, you will need to install a driver for [FTDI USB](https://ftdichip.com/drivers/) converter. Then you could use the [putty](https://www.putty.org/) which is a program that can be used for logging. One of the tutorials on how to set up data logging using putty is [here](https://my.kualo.com/knowledgebase/?kbcat=0&article=888) for example.

#### Stand-alone use

The device contains an SD card, which could be used do data logging in stand-alone use. In that case, the LABDOS01 needs an external power supply.
That mode could be used for short-term demonstrating of SPACEDOS, AIRDOS, or GEODOS variants of that device.

*Note: Ony industrial SLC SD cards with properly implemented SPI interface are supported.* 
