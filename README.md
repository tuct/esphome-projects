# Tuct - esphome stuff

This is a collection of esphome projects and components that i build over the years.

Mostly DIY esphome devices, but also some some components and esphome based custom firmware

## ESPHome Components

* [Sensirion Sen66](./components/sen6x) - Allow using the "new" sen66 with esphome
* [Levoit Core300s](./components/core300s) - Technical integration for custom esphome based firmware for the Levoit Core300s
* [Levoit Core400s](./components/core400s) - Technical integration for custom esphome based firmware for the Levoit Core400s

## Project - Free Levoit Air Purifiers

I like the Levoit Air Purifiers! I have a Core 300s, Core 400s, two LV PUR 131s (got one under warranty), plus a Levoit Mini!
I hated the fact that i had to use the cloud hosted by levoit and wanted fully Home Assistant integration without the cloud.

Here is a list of all 4 models, running with esphome, Core300/400s do not need a hardware hack, only desaaembly to flash new firmware, the Mini and the LV PUR 131s need some extra hardware and hacking.

* [levoit-core300s](./projects/levoit-core300s), (esphome) - Custom Firmware
* [levoit-core400s](./projects/levoit-core400s), (esphome) - Custom Firmware
* [levoit-lv-pur131s](./projects/levoit-lv131s/), (esphome, pcb hack, extra hardware) - Custom Firmware + better MCU and Sensor, plays Doom!
* [levoit-mini](./projects/levoit-mini), (esphome, 3d parts, custom pcb, extra hardware) - Smartify the Levoit Mini, Custom Hardware and Firmware


## Projects

* [aqMood](./projects/aqMood), (esphome, 3d parts, custom pcb) - A Slighlty emotional AirQuality MoodLight, based on the Sen66 sensor
* [smart-control-for-prusa-enclosure](./projects/smart-control-for-prusa-enclosure) (esphome, 3d parts) - 
* [tiny-wash-and-cure](./projects/tiny-wash-and-cure), (esphome, custom pcb) - 
* [automated-iris](./projects/automated-iris), (esphome, 3d parts) - Link to Printables and folder!


## TODO:
* adopt awMood to wall version
* add readme to tiny-wash-and cure
* add readme to automated-iris

