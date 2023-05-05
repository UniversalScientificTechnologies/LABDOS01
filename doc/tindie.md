## What LABDOS01 is? 

LABDOS01A is an ionizing radiation dosimeter and spectrometer based on a [c-Si](https://en.wikipedia.org/wiki/Crystalline_silicon) semiconductor detector. LABDOS01 is designed for use in laboratory and experimental conditions.

### The main features

  * Spectrometry measurement simultaneously with accurate dosimetric data
  * High sensitivity to ionizing radiation particles
  * Data logging to Smartphone or SDcard
  * Extremely low power consumption
  * Open-source firmware customizable in Arduino IDE

[LABDOS01 is an open-source](https://certification.oshwa.org/cz000008.html) dosimeter and ionizing radiation spectroscope developed for scientific use. LABDOS01A is a part of our successful open-source [UST dosimeters](https://www.ust.cz/UST-dosimeters/) series. The series involves the [SPACEDOS](https://www.ust.cz/UST-dosimeters/SPACEDOS/), [AIRDOS](https://www.ust.cz/UST-dosimeters/AIRDOS/), and [GEODOS](https://www.ust.cz/UST-dosimeters/GEODOS/) dosimeters. These devices have specific applications and the LABDOS01A complements the UST dosimeters series with a portable device designed for preliminary or laboratory experiments.

![Cosmic Rays radiation measured by SPACEDOS02 onboard ISS](https://raw.githubusercontent.com/UniversalScientificTechnologies/SPACEDOS02/SPACEDOS02A/doc/src/img/ISS_radiation_map.png "Cosmic Rays radiation measured by SPACEDOS02 onboard ISS")

LABDOS01 is an ionizing radiation detector, dosimeter-spectrometer, which works on the principle of collecting ions created by deposited energy in a semiconductor PIN diode. LABDOS01 can be classified as a [Liulin-type](https://en.wikipedia.org/wiki/Liulin_type_instruments) detector. Semiconductor based detection principle ensures sensitivity to almost all types of ionizing radiation forms except high-energy gamma and neutrons.

### What could it be used for?

* AeroSpace or Terrestrial Radiation Monitoring
* Aircraft Onboard Radiation Monitoring
* Scientific High Altitude Balloons, e.g. [Pfotzer Maximum measurement](https://en.wikipedia.org/wiki/Georg_Pfotzer)
* Educational Toolkit, [cosmic ray monitoring](https://en.wikipedia.org/wiki/Cosmic_ray) 
* Radiation Mapping in 3D together with GNSS and UAV
* Space Weather Monitoring e.g on high-altitude observatories
* [Open science](https://en.wikipedia.org/wiki/Open_science) 
* [Citizen science](https://en.wikipedia.org/wiki/Citizen_science)

The device uses [USTSIPIN02](https://github.com/ust-modules/USTSIPIN02) which is the core of a range of UST dosimeters specialized to the specific application like [AIRDOS](https://www.ust.cz/UST-dosimeters/AIRDOS/) or [SPACEDOS](https://www.ust.cz/UST-dosimeters/SPACEDOS/) or [GEODOS](https://www.ust.cz/UST-dosimeters/GEODOS/).

Since the semiconductor detector-based spectrometer and dosimeter combined in one device, is a unique instrument we have created a few examples of their use cases. Please see the [LABDOS documentation](https://github.com/UniversalScientificTechnologies/LABDOS01) for additional examples or more details. 

#### Aircraft fly-crew cosmic ray dosimeter

Secondary cosmic rays at higher altitudes in the atmosphere are a typical example of a mixed radiation field. A silicon semiconductor detector is an almost ideal device for measuring the parameters of such ionizing radiation. In addition, LABDOS can be easily connected to a smartphone, allowing data recording and visualization during the entire flight.

![Doserate measured on-board of an aircraft](https://raw.githubusercontent.com/UniversalScientificTechnologies/AIRDOS02/AIRDOS02A/doc/src/img/airdos_flight_doserate.png "Doserate measured on-board of an aircraft")

#### Radiation spectra measurement

Because LABDOS is also a spectrometer, it enables different radioisotopes to be distinguished by measuring the deposited energy in silicon. Here is an example of spectra [Am-241](https://en.wikipedia.org/wiki/Americium-241) and [Pu-239](https://en.wikipedia.org/wiki/Plutonium-239). 

![Am-241 and Pu-239 spetra measured by LABDOS](https://raw.githubusercontent.com/UniversalScientificTechnologies/LABDOS01/LABDOS01A/doc/img/radiation_spectra.png "Am-241 and Pu-239 spetra measured by LABDOS")

## Parameters

The LABDOS01A is standardly packed in a user-modifiable 3D-printed plastic box that provides mechanical durability and compactness for the device. At the same time, it can be mounted into the [MLAB](https://mlab.cz/) base plate (ALBASEx) after a screw replacement.

The detector is equipped with indication LEDs that show the status of the device. LABDOS contains 2 LEDs indicating USB communication, 1 indicating the presence of a power supply, and 3 user LEDs, which can have different meanings according to the used firmware.

| Parameter | Value |
|---|----|
| Detection element | Si PIN diode, 12.5 mm³ detection volume |
| Number of energy channels | 500 energy channels; depends on firmware |
| Deposited energy range | 160 keV to 8 MeV, can be changed by modification in analog front-end.  |
| Energy measurement resolution | up to 50 keV/channel, but the exact value depends on firmware and analog front-end setup.|
| Power supply |  5V, 3.3V; by using USB-C port or Pixhawk compatible JST-GH connectors |
| Power |  A few mA RMS. The consumption current is dominated by LED blinking and SDcard writing. |
| Integration time | 10s, depends on firmware setup |
| Dead-time | depends on firmware setup, 10ms with shipped firmware|
| Interface | USB-C, 3.3V PWR, UART |
| Dimensions | 96 x 56 x 19 mm; External dimensions of enclosure |
| Weight | 74 grams; including the enclosure |

The default energy range is selected to best match the alpha particle radiation of [radon progenies](https://en.wikipedia.org/wiki/Radon). 

## Connection
LABDOS01A can be connected to a logging or display device in several ways. Firstly, it is possible via the USB-C interface, which can fully power the device and download measured data. Firmware can also be updated via the USB-C interface. 

### UART

Another data output in the form of a UART is in a separate connector. Data output is the same as in the case of the USB interface. Connecting via this interface is recommended for long-term measurements. The connector belongs to the JST-GH series. UART connector is compatible with the [Dronecode DS-009](https://github.com/pixhawk/Pixhawk-Standards/blob/master/DS-009%20Pixhawk%20Connector%20Standard.pdf) standard.

### 3v3

The 3v3 power supply is reserved for use in very specific situations, especially in consumption-sensitive applications. Under normal circumstances, it is not needed. Check the schematic for possible uses.

### Auxiliary input

The auxiliary input is used to connect support devices that can provide additional information for measurement, for example, GPS position, PPS timestamps, or other data. 


## Firmware

LABDOS is based on Arduino and therefore can be equipped with many firmware versions that provide different types of data outputs. The firmware can be easily customized by using the Arduino IDE.  If your experiment requires a specific type of data, please feel free to contact us. 

### Generic firmware

The LABDOS01A is standardly shipped with generic firmware. On the output serial interface, a string is sent every 10s. This ASCII line contains a spectrum of the detected particles integrated over the last 10s. The output is simple, human-readable, and ready to be machine-processed.


## Shipment content:
  * LABDOS01A - Laboratory semiconductor dosimeter
     * Firmware variant: Generic firmware with CSV, UART/USB-C output
  * High-quality USB-C cable, 0.5m
