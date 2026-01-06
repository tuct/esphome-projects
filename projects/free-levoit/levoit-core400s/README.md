
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


### Header - 10 bytes long, 0 after 4 bytes!

- Byte 1 A5 start byte of packet
- Byte 2 22 send message or 12 ack message (52 might be error response)
- Byte 3 1-byte sequence number, increments by one each packet
- Byte 4 1-byte size of payload (after checksum byte)
- Byte 5 Always 0
- Byte 6 1-byte checksum. Computed as 255 - ( (sum all of bytes except checksum) % 256 )
- Byte 7 Always Payload Type /Version
- Byte 8 Always Payload Type /Version
- Byte 9 Always Payload Type /Version
- Byte 10 Always 0

bytes 11 + payload (size includes bytes 7-10 => min value0x04)


Bytes 7-9 payload type/ cmd

- case 0x013040:  //Status response core300S
- case 0x01B040:  //status response core400S
- case 0x020055:  //Status response Vital 200s/pro

- case 0x013140:  //status request core300s
```
enum class LevoitDeviceModel : uint8_t { NONE, CORE_200S, CORE_300S, CORE_400S };
enum class LevoitPacketType : uint8_t { SEND_MESSAGE = 0x22, ACK_MESSAGE = 0x12, ERROR = 0x52 };
enum class LevoitPayloadType : uint32_t {
  STATUS_REQUEST =        0x01 31 40,
  STATUS_RESPONSE =       0x01 30 40,
  AUTO_STATUS =           0x01 60 40, // I only know this value for 200S, so might be wrong  
  SET_FAN_AUTO_MODE =     0x01 E6 A5,
  SET_FAN_MANUAL =        0x01 60 A2,
  SET_FAN_MODE =          0x01 E0 A5,
  SET_DISPLAY_LOCK =      0x01 00 D1,
  SET_WIFI_STATUS_LED =   0x01 29 A1,
  SET_POWER_STATE =       0x01 00 A0,
  SET_SCREEN_BRIGHTNESS = 0x01 05 A1,
  SET_FILTER_LED =        0x01 E2 A5,
  SET_RESET_FILTER =      0x01 E4 A5,
  TIMER_STATUS =          0x01 65 A2,
  SET_TIMER_TIME =        0x01 64 A2,
  TIMER_START_OR_CLEAR =  0x01 66 A2,
  SET_NIGHTLIGHT =        0x01 03 A0
};

enum class LevoitState : uint32_t {
  POWER               = 1 << 0,
  FAN_MANUAL          = 1 << 1,
  FAN_AUTO            = 1 << 2,
  FAN_SLEEP           = 1 << 3,
  DISPLAY             = 1 << 4,
  DISPLAY_LOCK        = 1 << 5,
  FAN_SPEED1          = 1 << 6,
  FAN_SPEED2          = 1 << 7,
  FAN_SPEED3          = 1 << 8,
  FAN_SPEED4          = 1 << 9,
  NIGHTLIGHT_OFF      = 1 << 10,
  NIGHTLIGHT_LOW      = 1 << 11,
  NIGHTLIGHT_HIGH     = 1 << 12,
  AUTO_DEFAULT        = 1 << 13,
  AUTO_QUIET          = 1 << 14,
  AUTO_EFFICIENT      = 1 << 15,
  AIR_QUALITY_CHANGE  = 1 << 16,
  PM25_NAN            = 1 << 17,
  PM25_CHANGE         = 1 << 18,
  WIFI_CONNECTED      = 1 << 19,
  HA_CONNECTED        = 1 << 20,
  FILTER_RESET        = 1 << 21,
  WIFI_LIGHT_SOLID    = 1 << 22,
  WIFI_LIGHT_FLASH    = 1 << 23,
  WIFI_LIGHT_OFF      = 1 << 24
};
```





### Payload New:

Byte 11 HW Version Minor

Byte 12 HW Version Major

Byte 13 Power:

* 00 Off
* 01 On

Byte 14 Fan mode:

* 00 Manual
* 01 Sleep
* 02 Auto



Byte 15 Current Fan Speed => diff from core300s

* 00 Sleep
* 01 Low
* 02 Med
* 03 High
* 255 Power Off

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


## Commands

### 01 E6 A5 - Configure Fan Auto Mode (ESP to MCU)

Byte 4 Always 0

Byte 5 Fan Auto Mode

- 00 Default, speed based on air quality
- 01 Quiet, air quality but no max speed
- 02 Efficient, based on room size
- Byte 6 & 7 00 00 or Efficient Area

### 01 60 A2 - Set Fan Manual (ESP to MCU)

Byte 4 Always 0

Byte 5 Always 0

Byte 6 Always 1

Byte 7 Fan Speed:

- 01 Low
- 02 Med
- 03 High
- 04 Highest 

### 01 E0 A5 - Set Fan Mode (ESP to MCU)

Byte 4 Always 0

Byte 5 Fan Mode

- 00 Manual (App always uses 01 60 A2 with speed to change to manual mode)
- 01 Sleep
- 02 Auto

### 1 0 D1 - Display Lock (ESP to MCU)

Byte 4 Always 0

Byte 5 Display Lock:

- 00 Unlocked
- 01 Locked
- 01 29 A1 - Wifi LED state (ESP to MCU)

### Wifi LED toggled at startup and when network connection changes

1	29	A1	0	0	F4	1	F4	1	0 Off

1	29	A1	0	1	7D	0	7D	0	0 On

1	29	A1  0   2   F4  1   F4  1   0 Blink

### 01 31 40 - Request Status (ESP to MCU)

Similar to status packet, occurs when Wifi led state is changed.

### 01 00 A0 - Set Power State (ESP to MCU)

Byte 4 Always 0

Byte 5 Fan Speed:

- 00 Off
- 01 On

### 01 05 A1 Set Brightness (ESP to MCU)

Byte 4 Always 0

Byte 5 Screen Brightness

00 Screen Off
64 Screen Full

### 01 E2 A5 - Set Filter LED (ESP to MCU)

Byte 4

- 00 Off
- 01 On

Byte 5 0

Wasn't in original captures before firmware update

### 01 E4 A5 - Reset Filter (ESP to MCU and MCU to ESP)
Byte 4 0

Byte 5 0

MCU sends to ESP when sleep button held for 3 seconds

ESP sends to MCU when reset on app.

### 01 65 A2 - Timer Status (MCU to ESP)

MCU sends a packet when timer is running with remaining time

A5 12 27 0C 00 DA 01 65 A2 00 08 0D 00 00 10 0E 00 00

Remaining time and initial time.

0x0D08 remaining seconds

0x0E10 initial seconds

### 1 64 A2 Set Timer Time (ESP to MCU)

Byte 4 Always 0

Byte 5 & 6 Time

Byte 7 & 8 Always 0

### 1 66 A2 Timer Start or Clear (MCU to ESP)

Byte 4 to 12 All 0 to Clear

Byte 5 & 6 and Byte 9 &10 set to same initial timer value


