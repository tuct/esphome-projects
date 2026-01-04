# Smartified Levoit Mini

Custom PCB, 3D Printed Parts and Firmware (esphome based) to smartify the Levoit Mini Air Purifier
![a](./photos/da_06.jpg)


## Features
* Custom Firmware and PCB for the Levoit Mini - Non destructive and fully reversable.
* Home Assistant integration as Fan
  * 3 Speeds (33%, 66%, 100%)
  * Presets: Manual, Sleep(Speed 25%), Auto(Manual, 50%, requires PM based AQI sensor) 
* Filter Lifetime, based on current CFM and runtime
* Filter Lifetime can be configured between 1-12 Months, based on the air pollution you have.
* Fan Speed, Current and avg. CFM 

![Controls and Sensors](./photos/ha_one.png)
![Config and Diagnostics ](./photos/ha_two.png)

## Usage instructions

Click Button One time, for at least 1/4s to turn on and cycle speeds.

Double click Button quickly, to turn on and cycle preset modes (Manual, Auto, Sleep).

Long Press Button, 2.5s+ to turn off the device.

Filter can be resetted via HomeAssistant integration

## Build instructions

### Disassembly 

* Remove the Air Filter
* Remove the 4 Screws 

  ![a](./photos/da_01.jpg)
* User a long Screwdriver or similar item to carfully lift the top, by pushing inwards and down, after the first one or two are open, continue from the outside 

  ![a](./photos/da_02.jpg)
* Unplug the original PCB 

  ![a](./photos/da_03.jpg)



### BOM

* Xiao Seeed Studio ESP32S3
* [LCD - 0.91 Inch OLED Display I2C SSD1306](https://www.amazon.de/-/en/dp/B07BY6QN7Q)
* [Level Shifter](https://www.amazon.de/-/en/4-Channel-Converter-BiDirectional-Raspberry-Microcontroller/dp/B07RY15XMJ) - 3.3 / 5v Bidirectional
* [DC-DC Stepdown Converter]( https://www.amazon.de/-/en/dp/B08K37TS6F?ref_=ppx_hzod_title_dt_b_fed_asin_title_0_0&th=1), MP1584EN 22mm x 17mm x 4mm
* [Button](https://www.amazon.de/RUNCCI-YUN-Momentary-Tactile-12x12x7-3mm-Switches/dp/B0BF51N8CK), 12 mm x 12 mm x 7.3 mm
* Sockets and Connectors
  * JST-XH 2.5, 2pin and 4pin
  * JST-PH 2.0, 2pin and 4pin 
* Wires
* Custom PCB
* 3D Printed parts

### Custom PCB

![Custom PCB](./photos/pcb.png)

#### PCB Assembly 
I used poor man's smd soldering, but using pin header will also work - 
Add some solder to the bad with holes and then place the part on top and add solder to the top hole until it melts down, start with two opposite corners. Ensure connection is fine with Multimeter.



### 3D Printed Parts

### Button and LCD assembly
* Wire the LCD and the Button as shown here, use JST-XH connectors
* Insert the Button and slide the button holder in. Use hot or super glue to fix.
* use hot glue to glue the LCD in, i recommend doing this with the lcd on, so that you can find the correct position.




### Assembly

* Insert the replacment top part (bigger one) and connect the new PCB

  ![a](./photos/da_05.jpg)

* Check the strange use of the original connectors and connect the same way!

  ![a](./photos/da_04.jpg)
* Connect LCD and Button
* Insert the Button and LCD assemply, check orientation, Button should face the Levoit Logo. Slightly twist the part to find the correct position (snap).



#### Left over parts
![a](./photos/leftovers.jpg)