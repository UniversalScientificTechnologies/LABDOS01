# LABDOS01 - Experimental silicon based ionising radiation spectrometer

LABDOS01 is an open-source spectrometer-dosimeter based on silicone PIN diode and is intended for scientific and research purposes. Power and communication is secured by USB-C port or JST-GH connector. The device can be used statically (located in a specific place e.g. laboratory or base) or in mobile applications (such as cars or UAV). The spectrometer is housed in a 3D printed box that mechanical resistance and allows future development of user enclosures and integrations. 

The aim of LABDOS01 is to make the open-source, accesible, high quality, reliable and simple measuring device - radiation energy spectrometer for sientific comunity. 

LABDOS01 to function on its own. It requires a computer to visualize recorded measured data. At least it requires an external powering unit. The device is not intended for outdoor use (is not waterproof). 
Intead of that it is intended to be used as experimentel device with the exactly the same internals as its application specific variants like: 

 * [GEODOS01](https://github.com/UniversalScientificTechnologies/GEODOS01) - Outdoor and stand-alone onizing radiation detector
 * [AIRDOS02](https://github.com/UniversalScientificTechnologies/AIRDOS02) - Airborne cosmic radiation dosimeter with GPS
 * [SPACEDOS02](https://github.com/UniversalScientificTechnologies/SPACEDOS02) - PIN diode dosimeter for manned spacecraft 
 * [SPACEDOS01](https://github.com/UniversalScientificTechnologies/SPACEDOS01) - cubesat cosmic radiation dosimeter and spectrometer

LABDOS01 is therefore all-in-one solution for generic semiconductor based ionising radiation measurement. It should be used as universal testing device prior using an application specific design. 

## Parameters

 * Silicon PIN diode detector with 12.5 mm³ detection volume
 * Number of energy channels 250, but configurable by firmware in range
 * Deposited energy range from 200 keV to 12 MeV
 * Energy measurement resolution up  to 50 keV/channel but exact value depends on firmware and analog front-end setup.
 * Power supply 5V (trought USB port)
 * Integration time depends on firmware setup
 * Interface - USB 2.0, USB-C connector
 * Dimensions - 
 * Weight - 


The device is designed as open-source hardware and software and is released under the GPLv3 license. The device is originaly developed and maintained by [UST (Universal Scientific Technologies s.r.o.)](www.ust.cz) company, which sells it commercially and offers technical support.

## PC Connection

Simply connect the device to your computer USB port using a USB A-C cable. The device should appear as a virtual serial line on your computer. On a Linux computer, the device should connect itself. For computers with windows, you will need to install a driver for [FTDI USB](https://ftdichip.com/drivers/) converter.

This mode is especially suitable for interactive testing of silicon-based detector setup. E.g. experiments on accelerators, short-term flights or balloon or UAV experiments where external logging unit is available. 

### Output data format
The data output format is defined in the following table. 

### Data readout
We have prepared a simple python script for interactive reading and logging data from LABDOS01 spectrograph during an experiment.  

## Stand-alone use
Device contains SDcard, which could be used do data logging in stand-alone use. In that case the LABDOS01 needs an external power supply. 
That mode could be used for short-term demonstrating of SPACEDOS, AIRDOS or GEODOS variants of that device.  

## Technical details
Device uses [USTSIPIN02](https://github.com/ust-modules/USTSIPIN02) which is core of range of UST dosimeters specialized to specific applicaion like AIRDOS or SPACEDOS. 
