# Levoit LV-PUR131S 



## Hardware

Not like the other Levoits i have, only uses one ESP12F and no 2nd MCU

MCU: ESP12F 


### PCB

![pcb front](./images/pcp_front.png)

![pcb back](./images/pcp_back.png)

![pcb side](./images/pcp_side.png)


#### ESP12F Pinout

#### Display driver - TM1628 

SPI 500khz/ 1mhz?, 8 Bits, LSB Mode 0, enable active low

DIO -> GPIO13
SCK -> GPIO14
STP -> GPIO15 


Command 1# 0x03

Command 2# 0x40

14 bytes data
Command 4# 0x8C

Data:

Mask On for Levoit 131s
00 00 80 01 00 00 00 00 00 00 00 00 00 00 => POWER
00 00 00 00 00 00 00 00 00 00 00 00 18 00 => LOW fan
00 00 00 00 00 00 00 00 00 00 00 00 60 00 => MID fan
00 00 00 00 00 00 00 00 00 00 00 00 80 01 => HIGH fan
00 00 00 00 00 00 00 00 00 00 00 02 01 00 => AUTO
00 00 00 00 00 00 00 00 00 00 00 00 06 00 => SLEEP
00 00 00 00 80 01 00 00 00 00 00 00 00 00 => WIFI
AQI
00 00 00 00 00 00 49 02 24 01 02 00 00 00 => RED
00 00 00 00 00 00 DB 02 6D 03 06 00 00 00 =>Yellow
00 00 00 00 00 00 24 01 92 00 09 00 00 00 => blue
7seg:
00 02 00 00 00 00 00 00 00 00 00 00 00 00  => points
06 02 3F 00 BF 00 00 00 00 00 29 00 00 00 => 1:00h



#### Buttons to Serial - ????
RX -> GPIO10
990 Baud
7 Bit
1 Stop bit, no parity, LSB

Start cmd: 0x1A 
2 bytes Btn idx
2nd byte differs on release! sometime -> ignore!
15 ms between sends while presses

| Button | B1    |  B2    | What |
|--------:|:------:|:-------:|:-----|
| BTN1   | 0x1B  | 0x5A   | power|
| BTN2   | 0x1B  | 0x53   | display|
| BTN3   | 0x1B  | 0x5B   | sleep|
| BTN4   | 0x5B  | 0x5A   | auto |
| BTN5   | 0x5B  | 0x53   | Fan  |
| BTN6   | 0x5B  | 0x52   | timer|
    
#### PM1003
PWM/AQI ->GPIO03
Reset -> GPIO02

#### Fan - PWM out 
GPIO04
3Khz, 2.976Khz

| Speed  | Duty cylce    | 
|--------:|:------|
| LOW   | 40%  |
| MID   | 60%  |
| HIGH   | 80%  |



#### Beep

uses 2 pins, one sends short on sig, 2nd provides freq => nice after hall effect ;)



![beep - over ](./images/pwm_beep2.png)

![beep - start](./images/pwm_beep.png)

![start beep - detail](./images/pwm_beep3.png)


GPIO16 -> high 1.25s,low 220us, high 50ms
GPIO12 -> PWM 2.975MHZ, 50%, starts after 220us low to high, contiunes for 5sec after start





