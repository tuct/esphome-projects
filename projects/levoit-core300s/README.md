# esphome-levoit-core300s

# Changes by Tobi



* Added Fan component, so that i can use it with my fan group ;)
  * Preset Modes are part of Fan 
* Added Filter Life time based on info from below
  * Filter Life Time configurable between 1-12 months
  * Based on CFM per Mode and Runtime, Max Hours = 141CFM*24*30* Filter Life time (months)
  * Filter Led works
  * Reset Filter implemented
* Removed BLE

![image info](./photos/UpdatedUiInHa.png)
![image info](./photos/fan.png)

I started using this [implenetation](https://github.com/acvigue/esphome-levoit-air-purifier), but it crashed with some freeRTos errors, so i siwtched to this one.

But there are some good infos, i collected here as well!


## Disassembly
For details on the teardown please read the [blog post](https://vigue.me/posts/levoit-air-purifier-esphome-conversion)

* Place upside down and remove base cover and filter to expose 8 screws (4 have washers)
* Remove all 8 screws be careful, as these are made out of a soft metal
* Using a pry tool slide in between tabs
* Separate base and top sleeve
* Unplug logic board

## Flash
* Copy the provided sample configuration for your model to a new ESPHome configuration, while keeping the generated passwords
* Compile and download the binary (Choose the modern format once compilation has completed)
* Solder wires to pins TXD0, RXD0, IO0, +3V3, and GND near the ESP32 on the logic board, and connect these to a USB-UART converter. On some boards, if these are through holes, soldering may not be necessary.
*Connect IO0 to ground during power before connecting USB-UART to boot to bootloader. On some boards, IO0 may not have it's own debug pin and the ESP32 GPIO0 pin on the esp can be used.

### Backup Existing Firmware
```
esptool read_flash 0 ALL levoit.bin
```

This did not work for mew, always ended in an error, so i yoloed it and continued without a backup of the original FW

### Update name and set secrets
Rename `secrets-example.yaml` to `secrets.yaml` and set your wifi and encryption key, ...

Adopt device name if needed in `core300s.yaml` (multiple units!)

### Compile and Install New Firmware
```
esphome run core300s.yaml
```
  
& reassemble, enjoy :)

#### In case you want to flash back original FW
```
esptool erase_flash
esptool write_flash 0x00 levoit.bin
```

## Original
Modifications to a Levoit Core 300s air purifier to use esphome for local control.  The stock PCB has an ESP32-SOLO-1C with headers broken out to program.  All of the user interface and control seems to be implemented on the U2 MCU.  There is another power PCB that supplies 5V from mains input and connections for the 2.5PM sensor and Motor PWM control.

U2 Pins 13/14 are serial RX/TX connection to the ESP32 GPIO16/17.  Level shifting mosfets on bottom of PCB.  TP10 and TP33 by the ESP32 are respective test points.

Added a wire harness out the top with connections to the J1 header for the ESP32 and to TP10 and TP33.  Push button added for boot mode.  esptool connected to the Core300s ESP okay.  Used an ESP32 devboard to capture both sides of the core300s traffic on TP10 and TP33 with a little Arduino [project](https://github.com/mulcmu/ESP32_dual_serial_log).  115200 8n1 seemed to capture traffic okay.

Uploaded packet captures and mostly decoded.  Looks like a 3 byte payload type or bitmapped structure.  Filter life appears to be traced by the ESP, not stored in the U2 MCU.  ESPHome will need to replicate some sort of filter usage metric.

Update stock firmware to 1.0.8 / 2.0.13 before flashing esphome.  2.0.7 on one of my units had issues with pm2.5 and fan speed.

Updated to use external component vs custom components that were removed.

https://github.com/acvigue/esphome-levoit-air-purifier provides an alternate that is bit more polished.

#### Packet Header:

`A5` start byte of packet

`22` send message or `12` ack message  (`52` might be error response)

`1-byte` sequence number, increments by one each packet

`1-byte` size of payload (after checksum byte)

`0` always zero

`1-byte` checksum.  Computed as 255 - ( (sum all of bytes except checksum) % 256 )

Payload Type:  3 bytes

The acknowledge packet `12` contains the same sequence number, with the `payload type` and `0`

#### Payload Types/Structure:

##### `01 30 40` - Status (MCU to ESP)

The MCU sends a status packet a few times a second to once every few seconds.  Frequency increases when controls are operated.  After firmware update the MCU was way less chatty.  Only sending updates on state changes or once every 60 seconds.

`A5 22 1D 16 00 E4 01 30 40 00 07 00 02 01 00 01 64 01 00 00 01 03 00 00 00 3B 01 00`

22 byte long (0x16) status packet payload:

Byte 4 Always `0`

Byte 5 MCU FW SubMinor

Byte 6 MCU FW Minor

Byte 7 MCU FW Major

Byte 8 Power:  

- `00` Off
- `01` On 

Byte 9 Fan mode:

- `00` Manual
- `01` Sleep
- `02` Auto 

Byte 10 Manual Fan Speed Selected

- `00` (Occurs at startup for 1 packet)
- `01` Low
- `02` Med 
- `03` High

Byte 11 Screen Brightness

- `00` Screen Off
- `64` Screen Full (Screen illuminates briefly when another button is tapped while screen is off)

Byte 12 Screen

- `00` Off
- `01` On 

Byte 13 Current Fan Speed

- `00` Sleep
- `01` Low
- `02` Med
- `03` High
- `255` Power Off

Byte 14 Always `0`

Byte 15 PM2.5  (Normally `1` increased to `4` during filter testing, color ring LEDs)

Byte 16 & 17 PM2.5  0xFFFF when off, samples every 15 minutes when off

Byte 18 Display Lock:

- `00` Unlocked
- `01` Locked

Byte 19 Fan Auto Mode: (Only configurable by the app)

- `00` Default, speed based on air quality
- `01` Quiet, air quality but no max speed
- `02` Efficient, based on room size

Byte 20 & 21 Efficient Area: 

- `3B 01` 100 sq ft (App Minimum)  0x013B is 315

- `EC 04` 400 sq ft (App Maximum) 0x04EC is 1,260

  Linear scale, not sure what the units are.

Byte 22 Always `0`

##### `01 E6 A5` - Configure Fan Auto Mode (ESP to MCU)

Byte 4 Always `0` 

Byte 5Fan Auto Mode

- `00` Default, speed based on air quality
- `01` Quiet, air quality but no max speed
- `02` Efficient, based on room size

Byte 6 & 7 `00 00` or Efficient Area

##### `01 60 A2` - Set Fan Manual (ESP to MCU)

Byte 4 Always `0`

Byte 5 Always `0`

Byte 6 Always `1`

Byte 7 Fan Speed:

- `01` Low
- `02` Med
- `03` High

##### `01 E0 A5` - Set Fan Mode (ESP to MCU)

Byte 4 Always `0`

Byte 5 Fan Mode

- `00` Manual (App always uses `01 60 A2` with speed to change to manual mode)
- `01` Sleep
- `02` Auto 

##### `1 0 D1` - Display Lock (ESP to MCU)

Byte 4 Always `0`

Byte 5Display Lock:

- `00` Unlocked
- `01` Locked

##### `01 29 A1` - Wifi LED state (ESP to MCU)

Wifi LED toggled at startup and when network connection changes

`1	29	A1	0	0	F4	1	F4	1	0 `Off

`1	29	A1	0	1	7D	0	7D	0	0` On

`1	29	A1  0   2   F4  1   F4  1   0` Blink

##### `01 31 40` - Request Status (ESP to MCU)

Similar to status packet, occurs when Wifi led state is changed.  

##### `01 00 A0` - Set Power State (ESP to MCU)

Byte 4 Always `0`

Byte 5 Fan Speed:

- `00` Off
- `01` On

##### `01 05 A1` Set Brightness (ESP to MCU)

Byte 4 Always `0`

Byte 5 Screen Brightness

- `00` Screen Off
- `64` Screen Full

##### `01 E2 A5` - Set Filter LED (ESP to MCU)

Byte 4

- `00` Off
- `01` On

Byte 5 `0`

Wasn't in original captures before firmware update

##### `01 E4 A5` - Reset Filter (ESP to MCU and MCU to ESP)

Byte 4 `0`

Byte 5 `0`

MCU sends to ESP when sleep button held for 3 seconds

ESP sends to MCU when reset on app.

##### `01 65 A2` - Timer Status (MCU to ESP)

MCU sends a packet when timer is running with remaining time

`A5 12 27 0C 00 DA 01 65 A2 00 08 0D 00 00 10 0E 00 00`

Remaining time and initial time.

0x0D08 remaining seconds

0x0E10 initial seconds

##### `1 64 A2` Set Timer Time (ESP to MCU)

Byte 4 Always `0`

Byte 5 & 6 Time

Byte 7 & 8 Always `0`

##### `1 66 A2` Timer Start or Clear (MCU to ESP)

Byte 4 to 12 All `0` to Clear

Byte 5 & 6 and Byte 9 &10 set to same initial timer value

#### ESPHome functions:

- Toggle Power
- Set fan Mode, Manual, Sleep, Auto
- Set fan speed for manual mode
- PM2.5 sensor feedback
- Filter life remaining.  
- No Timer/Schedule support (implement in HA instead of on device)

#### Filter Life:

Manufacturer lists the filter life at 6 to 8 months and provided clean air deliver rates (CADR) for the different fan modes.

| Fan Speed | CADR    |
| --------- | ------- |
| Sleep     | 37 CFM  |
| Low       | 60 CFM  |
| Med       | 80 CFM  |
| High      | 141 CFM |

Filter lifetime air volume will be estimated based on user provided number of months and 24h operation on High.  The actual air volume through the filter will be estimated based actual runtime and volume for each fan speed.

Filter state sensor will provide a remaining lifetime percent.  Filter service interval will also be selectable and tracked.  Filter service is basically just to vacuum pre-filter.

#### TODO:

- Investigate OTA of stock hardware without disassembly.
- Filter Life tracking
- Timer feedback if enabled on unit
- PM2.5 feedback seems very low.  Sensor unit is a Cubic PM2008MS.  Look into raw data.

#### Notes:

Vesync app has original FW version for ESP and MCU from the PCB sticker.  OTA firmware updated ESP and MCU firmware.

The external ESP32 data logger can be used as test ESPhome device if internal ESP32 is put into bootloader mode at powerup.

Internal ESP32 sends out some limited status information on the serial port during operation.

Internal ESP32 seems to use MQTT to communicate with Vesync servers.  Wireshark dissector shows packets as malformed.  They might be encrypted.

FW update downloads file from fw.vesync.com.  Looks to be initiated by MQTT. Just flashing the OTA binary or saved binary doesn't seem to update the U2 firmware automatically.  Looks like another MQTT command and file download for MCU update.

Ordered 2 more October 2022, still have 1.1.01 and 2.0.7 firmware from factory.
