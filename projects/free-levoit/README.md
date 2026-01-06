# Project - Free Levoit Air Purifiers

I like the Levoit Air Purifiers! I have a Core 300s, Core 400s, two LV PUR 131s (got one under warranty), plus a Levoit Mini!
I hated the fact that i had to use the cloud hosted by levoit and wanted fully Home Assistant integration without the cloud.

Here is a list of all 4 models, running with esphome, Core300/400s do not need a hardware hack, only desaaembly to flash new firmware, the Mini and the LV PUR 131s need some extra hardware and hacking.

* [levoit-core300s](./projects/levoit-core300s), (esphome) - Custom Firmware
* [levoit-core400s](./projects/levoit-core400s), (esphome) - Custom Firmware
* [levoit-lv-pur131s](./projects/levoit-lv131s/), (esphome, pcb hack, extra hardware) - Custom Firmware + better MCU and Sensor, plays Doom!
* [levoit-mini](./projects/levoit-mini), (esphome, 3d parts, custom pcb, extra hardware) - Smartify the Levoit Mini, Custom Hardware and Firmware

Not my projects, but worth checking out:
* [levoit-vital-200s + levoit-vital-200s pro, levoit-vital-100s?](https://github.com/targor/levoit_vital/?tab=readme-ov-file)
* https://github.com/mulcmu/esphome-levoit-core300s
* https://github.com/acvigue/esphome-levoit-air-purifier



# Levoit MCU TO ESP core 200/300/400, Vital 100/200

UART between MCU and ESP

Generic format shared between versions, with some differences. Core and Vital versions use same base format but are 2 different main versions.

Data collected from all the links and repos mentioned above ands combined here.

Thanks to

- https://github.com/mulcmu/
- https://github.com/acvigue/
- https://github.com/targor/


Here is the main excel sheet with the [data online](https://docs.google.com/spreadsheets/d/1vSKuPWYplPWVsKXIuISHjPhHEBLqe_wB/edit?usp=sharing&ouid=109862782260450893370&rtpof=true&sd=true)
and a [snap to download](./Levoit%20Uart%20Communication.xlsx) 

## Header - 10 to 12 bytes long, 0 after 4 bytes!

- Byte 0 A5 start byte of packet
- Byte 1 22 send message or 12 ack message (52 might be error response)
- Byte 2 1-byte sequence number, increments by one each packet
- Byte 3 1-byte size of payload (after checksum byte)
- Byte 4 Always 0
- Byte 5 1-byte checksum. Computed as 255 - ( (sum all of bytes except checksum) % 256 )
- Byte 6 Command Size: 1 => 2 bytes, 2 => 4 bytes
- Byte 7 Always Payload Type 
- Byte 8 Always Payload Type 
- Byte 9 Always 0
- Byte 10 Payload Type - Used if Byte 6 = 0x02 as cmd id - Vital versions
- Byte 11 Payload Type - Used if Byte 6 = 0x02 as cmd id

### Bytes 0 - 5 Header
| B.0 | B.1 | B.2 | B.3 | B.4 | B.5 |
|:-:|:-:|:-:|:-:|:-:|:-:|
|Start| Message Type  |  Sequence | Payload Size  |  Zero | Checksum  |
| 0xA5 | 0x22  | 0xXX  | 0x05  | 0x00  | 0xXX  |


### Bytes 6 - 9/11 Command/Payload Type

Byte 6 = The lengh of the command, 

- 1 = 2 bytes
- 2 = 4 bytes

Size = 1
| B.6 | B.7 | B.8 | B.9 |
|:-:|:-:|:-:|:-:|
|CMD Size | CMD  |  CMD  |  Zero|
| 0x01 | 0xXX | 0xXX  | 0x00 |

Size = 2, used by vital versions
| B.6 | B.7 | B.8 | B.9 | B.10 | B.11 |
|:-:|:-:|:-:|:-:|:-:|:-:|
|CMD Size | CMD  |  CMD | Zero  | CMD  |  CMD |  
| 0x02 | 0xXX | 0xXX  | 0x000 | 0xXX  | 0xXX |

## Commands

All details can bve found [here](./Levoit%20Uart%20Communication.xlsx)

Core and Vital have different cmd structure, both use headers from above!

### Core Series

![core commands](./commands_core.png)

### Vital Series

![vital commands](./commands_vital.png)
![vital sleep config commands](./commands_vital_sleep_conf.png)

## Responses

|Devices|Type|Start| Message Type  |  Sequence | Payload Size  |  Zero | Checksum  | CMD Size | CMD  |  CMD | Zero ||||||||||
|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|
||| B.0 | B.1 | B.2 | B.3 | B.4 | B.5 |B.6 | B.7 | B.8 |B.9| B.10 |B.11 |B.12 |B.13 | B.14 | B.15 |B.16| B.17 ||
|Core200/300s| Timer running| 0xA5 | 0x22  | 0xXX  | 0x0C | 0x00  | 0xXX  |  **0x01**	|**0x65**	|**0xA2**	|0x00|**0x08**	|**0x0D**	|0x00	|0x00	|**0x10**	|**0x0E**	|0x00|	0x00|	B10/11 = Remaining time & B14/15 >initial timer value int 16|
|Core200/300s| Filter reset pressed| 0xA5 | 0x22  | 0xXX  | 0x05 | 0x00  | 0xXX  |**0x01**|	**0xE4**|**0xA5**|	0x00| During Timer running|



### Status Responses



|Devices|Start| Message Type  |  Sequence | Payload Size  |  Zero | Checksum  | CMD Size | CMD  |  CMD | Zero  | CMD<br>Payload |  CMD<br>Payload|  
|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|
|| B.0 | B.1 | B.2 | B.3 | B.4 | B.5 |B.6 | B.7 | B.8 |B.9| B.10 |B.11 |
|Core200/300s| 0xA5 | 0x22  | 0xXX  | 0x16 | 0x00  | 0xXX  |  **0x01** | **0x1B** | **0x40** |0x00|||
|Core400s| 0xA5 | 0x22  | 0xXX  | 0x16  | 0x00  | 0xXX  | **0x01** | **0x30**	| **0x40** |0x00|||
|Vital 100/200s| 0xA5 | 0x22  | 0xXX  | 0x6C | 0x00  | 0xXX  |  **0x02** | **0x00**	| **0x55** | 0x00 | **0x00** | **0x01** |




### Response format for Core versions

Response payload for core versions has a max lenght of 28 bytes total, 10 header, 18 max payload.

Response starts at byte 10, zero on bytes 19 and 27. 

|Byte|Core 200/300s<br>0x01 0x1B 0x40|Description|Core 400s - Difference<br>0x01 0x30 0x40|Core 400s - values|
|:---|:--------|:----------------|:-------------------|:----------------|
|B.10|HW Version patch||| |
|B.11|HW Version minor||| |
|B.12|HW Version major||| |
|B.13|Power|00 Off<br>01 On|| |
|B.14|Fan Mode|00 Manual<br>01 Sleep<br>02 Auto|| |
|B.15|Manual Fan Speed Selected|00<br>01 Low<br>02 Med<br>03 High|Current Fan Speed|00 Sleep<br>01 Low<br>02 Med<br>03 High<br>04 Highest<br>255 Power Off|
|B.16|Screen Brightness|00 Screen Off<br>64 Screen Full|Manual Fan Speed Selected|00<br>01 Low<br>02 Med<br>03 High<br>04 Highest|
|B.17|Screen|00 Off<br>01 On|Screen Brightness|00 Screen Off<br>64 Screen Full|
|B.18|Current Fan Speed|00 Sleep<br>01 Low<br>02 Med<br>03 High<br>255 Power Off|Screen|00 Off<br>01 On|
|B.19|Always Zero||| |
|B.20|PM2.5 AQI|01 - 04|| |
|B.21 B.22|PM2.5|int16 pm2.5 value|| |
|B.23|Display Lock|00 Unlocked<br>01 Locked|| |
|B.24|Fan Auto Mode|00 Default<br>01 Quiet<br>02 Efficient|| |
|B.25 B.26|Efficient Area|3B 01 100 sq ft (App Minimum) 0x013B is 315<br>EC 04 400 sq ft (App Maximum) 0x04EC is 1,260|| |
|B.27|Always Zero||| |




### Response format for Vital versions

Response payload for vital versions has a max lenght of 114 bytes total, 12 header, 102 max payload.

Response starts at byte 13,
Each part of the response has an ID, 2 bytes => 0x04 0x01, first byte is the id, 2nd the length of data 0x01 = 1 byte and 0x02 = 2 bytes.

|Data ID| Data Size  | Data Payload |Data ID| Data Size  |  Data Payload | Data Payload |  ... |
|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|
| B.10 | B.11  | B.12  | B.13| B.14  | B.15 |B.16 | B.XX |
| **0x00**| **0x01** | 0xXX  | **0x01**| **0x02** | 0xXX |0xXX | ... |




Here is a table with all know data from the vital 200s

| Address       | Type   | Parameter | Description |
|---------------|--------|-----------|------------|
| B.12          | int 8  | always 1 | |
| B.15          | int 8  | MCU Version patch |0x05 |
| B.16          | int 8  | MCU Version minor |0x00 |
| B.17          | int 8  | MCU Version major |0x01 => 1.0.5 ? |
| B.20          | int 8  | Power | 0,1 |
| B.23          | int 8  | FAN MODE | 0 Manual?, 1 auto, 2 sleep, 5 pet? |
| B.26          | int 8  | FAN lvl | 0=minimum,1,2,3,4(circulation),5(auto), 0XFF=NULL? |
| B.29          | int 8  | Fan Speed? | 1 - 4 |
| B.32          | int 8  | display illuminated state | 0,1 |
| B.35          | int 8  | display state | 0,1 |
| B.38          | int 8  | ?? | |
| B.41          | int 8  | Airquality lvl | 1 - 4 |
| B.44          | int 8  | airquality score | |
| B.47 B.48     | int 16 | paricledichte(pm2.5) | |
| B.51          | int 8  | display lock | 0,1 |
| B.54          | int 8  | automode | 0,1 |
| B.57 B.58     | int 16 | efficent | Efficiency max (1800) 2340 = 0x0924, B15/B16 are value, min(100) 130 = 0x0082 |
| B.61 B.62     | int 16 | ? | |
| B.65          | int 8  | ? auto mode?!? | 0=default,1=quiet,2=efficient? |
| B.68          | int 8  | LIGHT DETECT | 0,1 |
| B.71          |        | ? | |
| B.74          |        | ? | |
| B.77          | int 8  | sleepmode | default=0, custom=1 |
| B.80          |        | ? | |
| B.83 B.84     | int 16 | quick clean minutes | |
| B.87          | int 8  | quick clean fan lvl | 1-4 |
| B.90          |        | ? | |
| B.93 B.94     | int 16 | white noise minutes | |
| B.97          | int 8  | white noise fan lvl | 1-4 |
| B.100 B.101   | int 16 | sleep mode minutes | |
| B.104         |        | sleep mode fan mode/fan lvl | 1-4 |
| B.107         | int 8  | daytime turn device on/off | 0,1 |
| B.110         | int 8  | daytime fan mode | 0=use fan lvls, 2=auto, 5=PET mode |
| B.113         |        | daytime fan lvl | 1-4 |





