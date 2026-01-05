
# Levoit Core400s - Custom Firmware (esphome)

Based On Core300s - but needed some changes to the component as small parts of the communcations changed compared to the core300s

* Works with Core400s, Board Version 1.2 - MCU Firmware: 3.0.0
* Improved state handling, only send state if changed, lewss noisy logs,...

## Features

* Fan component with modes (Manual, Auto, Sleep)
* Display current and avg CFM value
* Filter life time 
  * Tracking based on current CFM value
  * Configurable via Home Assistant (1-12 Months)
  * Reset via Home Assistant
* Display run time in Home Assistant

## Disassembly

Very similar to Core 300s but i had to unplug the pcb from the carry holes to get to it after i removed the filter and filter hosing.
* Place upside down and remove base cover and filter to expose 8 screws (4 have washers)
* Remove all 8 screws be careful, as these are made out of a soft metal
* Using a pry tool slide in between tabs
* Separate base and top sleeve
* Unplug logic board - done with screwdriver/kitchen knife from the side
* Remove Fan unit to get to the logic board

J1 - dubug header pinout
1 - TX
2 - RX
3 - GND
4 - 33.3V
5 - IO0
6 - EN

I was not able to dump the original FW, it seems i am stupid or it is somehow prevented (watchdog?), maybe try when pcb is powererd up?

## Flash

* Solder wires to pins TXD0, RXD0, IO0, +3V3, and GND near the ESP32 on the logic board, and connect these to a USB-UART converter. On some boards, if these are through holes, soldering may not be necessary.
* Connect IO0 to ground during power before connecting USB-UART to boot to bootloader. On some boards, IO0 may not have it's own debug pin and the ESP32 GPIO0 pin on the esp can be used.

### Backup Existing Firmware
```
esptool read_flash 0 ALL levoit.bin
```

This did not work for me, always ended in an error, so i yoloed it and continued without a backup of the original FW

### Update name and set secrets

Rename `secrets-example.yaml` to `secrets.yaml` and set your wifi and encryption key, ...

Adopt device name if needed in `core400s.yaml` (multiple units!)

### Compile and Install New Firmware

```
esphome run core400s.yaml
```
  
& reassemble, enjoy :)

#### In case you want to flash back original FW

```
esptool erase_flash
esptool write_flash 0x00 levoit.bin
```





## HW Details

Different FW! 3.0.0

Main changes 
* Message identifier for status changed from 01 30 40 to 01 B0 40
* Position of fan speed and display on changed compared to 300s other message ids!

01 B0 40 - Status (MCU to ESP)

Header new 

A5 22 F7 16 00 10 **01 B0 40** 00 00 00 03 

Header Old

A5 22 1D 16 00 E4 **01 30 40** 00 07 00 02 #

Payload New

01 00 01 01 64 01 00 01 FF FF 00 02 BD 00 01

Payload OLD

01 00 01 64 01 00 00 01 03 00 00 00 3B 01 00


22 byte long (0x16) status packet payload:


Header
Byte 1 A5 start byte of packet
Byte 2 22 send message or 12 ack message (52 might be error response)
Byte 3 1-byte sequence number, increments by one each packet
Byte 4 1-byte size of payload (after checksum byte)
Byte 5 Always 0
Byte 6 1-byte checksum. Computed as 255 - ( (sum all of bytes except checksum) % 256 )
Byte 7 Always Payload Type /Version
Byte 8 Always Payload Type /Version
Byte 9 Always Payload Type /Version

Byte 10 MCU FW SubMinor

Byte 11 MCU FW Minor

Byte 12 MCU FW Major

Payload New:


Byte 13 Power:

* 00 Off
* 01 On

Byte 14 Fan mode:

* 00 Manual
* 01 Sleep
* 02 Auto



Byte 15 Current Fan Speed => diff from core300s

00 Sleep
01 Low
02 Med
03 High
255 Power Off

Byte 16 Manual Fan Speed Selected => diff from core300s

* 00 (Occurs at startup for 1 packet)
* 01 Low
* 02 Med
* 03 High

Byte 17 Screen Brightness => diff from core300s

* 00 Screen Off
* 64 Screen Full (Screen illuminates briefly when another button is tapped while screen is off)

Byte 18 Screen

* 00 Off
* 01 On


Byte 19 Always 0

Byte 20 PM2.5 (Normally 1 increased to 4 during filter testing, color ring LEDs)

Byte 21 & 22 PM2.5 0xFFFF when off, samples every 15 minutes when off

Byte 23 Display Lock:

* 00 Unlocked
* 01 Locked

Byte 24 Fan Auto Mode: (Only configurable by the app)

* 00 Default, speed based on air quality
* 01 Quiet, air quality but no max speed
* 02 Efficient, based on room size

Byte 25 & 26 Efficient Area:

3B 01 100 sq ft (App Minimum) 0x013B is 315

EC 04 400 sq ft (App Maximum) 0x04EC is 1,260

Linear scale, not sure what the units are.

Byte 27 Always 0



