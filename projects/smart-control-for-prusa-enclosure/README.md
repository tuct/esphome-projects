# smart control for the original Prusa enclosure
Esphome based smart control for the original prusa enclosure to control temperature and air quality. 

Based on the original prusa enclosure with light bar and air filter and 
The awesome mod which adds automated heater and air ventilation from [printables]( 
https://www.printables.com/model/561491-automated-heating-system-for-original-enclosure).

This adds air quality and auto control for the air filter and uses esphome instead of c directly. 

## UI / Front
![UI](/images/UI_a.jpg)
![UI](/images/UI_b.jpg)

## Home Assistant 
![HomeAssistant](/images/homeassistant.png)
![HomeAssistant](/images/homeassistant2.png)



## Warning!
This is for advanced users only! You should be familiar with esphome and have experience with bigger projects as this requires quite some soldiering and wires. 

This documentaion is also Work in Progress and might not be fully complete as of now.

## Features 

* UI with Control via Rotary Button
* Air quality and temperature sensors 
* Automated heater, air ventilation and air filter
* Runtime counter for the air filter
* Integration with home assistant fpr sensors and heater



## Bill of Material
* 1 * Led Strip 24v, the original prusa is not dimmable... so i recommend a cheaper, dimmable one
* 1 * original Prusa Air Filter, i had to cut the wires to connect to 24V cause i could not get a connector
* 1 * ESP32
* 1 * Sen54 Sensor
* 1 * Rotary Encoder
* 3 * Switches (Retro!)
* 1 * LCD ST7735
* 1 * IO Expander (MCP23017, overkill but i had it at home)
* 3 * Dual MOSFET Switch Module - Control 24v Leds, AirFilter, Fan via esp32
* 2 * Buck Converter (24v to 5v and 3.3v)
* 2 * 3.3V Relais (Heater and to control Printer power)
* 2 * DS18B20 Temperature Sensor
* 1 * 200W heater+fan combo (i use a MOSFET to control the fan)
* 1 * SG-90 type servo
* 1 * 70mm x 50mm prototype board (front)
* 1 * 70mm x 20mm prototype board (back)
* 1 * MeanWell LRS-200-24 or MeanWell RSP-200-24 
* A lot of wires (for front and back unit + connections) 
* Wires for the heater 19 to 20awg (100w@24v!)
* Kabel to power printer 
* JST connectors or similar


## Code
Based on esphome, wifi should be setup for ntp clock the code can be found here:
[smart-prusa-enclosure.yaml](/smart-prusa-enclosure.yaml)


## Printed Parts
4 Heat inserts  (M3\*5*6, voron style)
All part 1 time from the 3dParts folder except SmartEnclosureBracket.obj this we need 2 times 

From [Automated Heating System for Original Enclosure]( 
https://www.printables.com/model/561491-automated-heating-system-for-original-enclosure).
Hinge.stl
Flap.stl
Heater_Mount.stl
Air_TempSensor_Holder.stl
Servo_Adapter.stl
Case_TempSensor_Holder.stl

I am not super happy with the front case at the moment but it works for now.

## Wire diagram
![Frtzing](/images/fritzing.png)

The left side shows the back unit and the right side the front unit.

The back unit houses all the output and sensor components and the front unit the esp, lcd, io expander and other input stuff.
The are connected with a 11 wires under the enclosure.

Here is the back unit (not mounted)
![Back unit](/images/back-cases-with-electronics.jpg)

Here is the front unit (during build from the back)
![Front unit](/images/front-case-with-electronics.jpg)



### Front Unit
3.3V and GND are comming from the back unit, this lists all connnections within the front unit.
#### LCD
* Light -> ESP32 - GPIO04
* MISO -> NC
* SCLK -> ESP32 - GPIO14 (SPI CLK)
* MOSI -> ESP32 - GPIO26 (SPI MOSI)
* TFTCS -> ESP32 - GPIO27 (CS)
* CARDCS -> NC
* TFTDC -> ESP32 - GPIO13 
* RESET -> 3.3V
* 3.3V -> 3.3V
* GND -> GND

#### MCP23017
* SCL -> ESP32 - GPIO22 (I2C SCL)
* SDA -> ESP32 - GPIO21 (I2C SDA)
* 3.3V -> 3.3V
* GND -> GND

#### SWITCHES

4 switches, GND to 
* Light -> MCP23017 - 0
* Fan -> MCP23017 - 1
* AirFilter -> MCP23017 - 2
* Printer -> MCP23017 - 4

#### Rotary
* Button -> MCP23017 - 3
* A -> ESP32 - GPIO05 
* B -> ESP32 - GPIO17
* 3.3V -> 3.3V
* GND -> GND

### Back Unit and Sensors

Use two DC/DC to create 5v and 3.3v from the 24v,
The 3,3v is used to power the front unit, the 5v are only used in back unit and sensors.
Connections to ESP32  must be wired from back to front. I used a long ribbon wire with 11 wires to archive this.

#### 3,3v Relais - Printer 
* Signal -> ESP32 - GPIO25 
* 3.3V -> 3.3V
* GND -> GND
Use NC and COM to control printer power
* OUT-NC -> Printer 220V 
* OUT-NO -> Not connected 
* OUT-COM -> Printer 220V  (same line!, diagram is wrong! shows NC to NO, but NC to COM is correct)

#### 3,3v Relais - Heater 
* Signal -> ESP32 - GPIO33 
* 3.3V -> 3.3V
* GND -> GND
Use NO and COM to control heater
* OUT-NO -> Heater Gnd
* OUT-NC-> Not connected 
* OUT-COM -> Heater Gnd

#### MOSFET - FAN
* Signal -> ESP32 - GPIO19 
* 3.3V -> 3.3V
* GND -> GND
* Out -> Light 24V + GND


#### MOSFET - Air Filter
* Signal -> ESP32 - GPIO23 
* 3.3V -> 3.3V
* GND -> GND
* Out -> Air Filter 24V + GND

#### MOSFET - Light
* Signal -> ESP32 - GPIO16
* 3.3V -> 3.3V
* GND -> GND
* Out -> Light 24V + GND

#### Servo 18
* Sig -> ESP32 - GPIO18
* 5V -> 5V
* GND -> GND

#### DS18B20 Sensors 15
* Sig -> ESP32 - GPIO15
* 3.3V -> 3.3V
* GND -> GND

#### SEN54
* SCL -> ESP32 - GPIO22 (I2C SCL)
* SDA -> ESP32 - GPIO21 (I2C SDA)
* 5V -> 5V
* GND -> GND
















