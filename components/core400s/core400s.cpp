#include "core400s.h"
#include <cmath>
namespace esphome {
namespace core400s {

void Core400sComponent::setup() {

    ESP_LOGD(TAG, "setup()...");
    
    rx_buf.reserve(128);
    tx_buf.reserve(32);

    this->sensor_pm25->publish_state(NAN);
    this->sensor_room_size->publish_state(NAN);
    
    this->textsensor_fan_speed->publish_state("Unknown");        
    this->textsensor_power->publish_state("Unknown");      
    this->textsensor_mcu_fw->publish_state("Unknown");    
    this->textsensor_fan_mode->publish_state("Unknown");    
    this->textsensor_display_lit->publish_state("Unknown");    
    this->textsensor_display_locked->publish_state("Unknown");    
    this->textsensor_fan_auto_mode->publish_state("Unknown");    
    
    send_command(cmd_get_status);

}


void Core400sComponent::update() {
        //longest messages should take about 2msec to tx from MCU
        while ( available() ) {
            read_byte(&b);

            //Look for start byte
            if( (b == 0xA5) )  {
                rx_buf.clear();
                rx_buf.push_back(b);

                //Read until checksum
                for(int i=0; i<5; i++) {
                    if(read_byte(&b))
                        rx_buf.push_back(b);
                    else
                        break;
                }

                if(rx_buf.size() == 6) {
                    //Read rest of packet
                    for(int i=0; i<rx_buf[3]; i++) {
                        if(read_byte(&b))
                            rx_buf.push_back(b);
                        else
                            break;
                    }

                    //Process packet if payload was read complete
                    if(rx_buf.size() == (rx_buf[3]+6) ) {
                        defer("process packet", [=, this] { process_packet(); });

                    }
                }
            }
          }
}

float Core400sComponent::get_setup_priority() const
{ 
    return esphome::setup_priority::LATE; 
}

void Core400sComponent::dump_config() {
  ESP_LOGCONFIG(TAG, "Core400s Component:");
  // Additional configuration logging
}

void Core400sComponent::send_command(std::vector< uint8_t > &cmd) {
    cmd[2]=tx_seq_num++;
    cmd[5]=0; //zero checksum byte
    uint8_t cs=255;
    for(int i=0; i<cmd.size(); i++) {
        cs -= cmd[i];
    }
    cmd[5]=cs;
    write_array(cmd);
}

void Core400sComponent::acknowledge_packet() {
    tx_buf.clear();
    tx_buf.assign(rx_buf.begin(),rx_buf.begin()+9);
    tx_buf.push_back(0);
    tx_buf[1]=0x12;  //ack byte
    tx_buf[3]=4; //payload lenght
    tx_buf[5]=0; //checksum byte
    uint8_t cs=255;
    for(int i=0; i<tx_buf.size(); i++) {
        cs -= tx_buf[i];
    }
    tx_buf[5]=cs;
    write_array(tx_buf);
}

void Core400sComponent::defer_publish_text(const std::string &defer_id,
                        esphome::text_sensor::TextSensor *sensor,
                        const std::string &new_state) {
    defer(defer_id, [=, this]() {
        auto current = sensor->get_state();
        if (current != new_state) {
            sensor->publish_state(new_state);
        }
    });
}
void Core400sComponent::defer_publish_number(
    const std::string &defer_id,
    esphome::sensor::Sensor *sensor,
    float new_state) {

    defer(defer_id, [=, this]() {
        float current = sensor->get_state();

        if ((std::isnan(current) && !std::isnan(new_state)) ||
            (!std::isnan(current) && std::isnan(new_state)) ||
            (!std::isnan(current) && !std::isnan(new_state) && current != new_state)) {
            sensor->publish_state(new_state);
        }
    });
}
void Core400sComponent::process_packet()  {
    //uart::UARTDebug::log_hex(uart::UARTDirection::UART_DIRECTION_RX , rx_buf, '-');
    char buf[32];        
    unsigned int packet_type=0;
    packet_type = (rx_buf[6] << 16) + (rx_buf[7] << 8) + rx_buf[8];
    //ESP_LOGD(TAG, "process_packet type %X", packet_type);


    
    if(rx_buf[1]==0x22)
        acknowledge_packet();

    switch (packet_type) {
        case 0x013040:  //Status packet
        case 0x01B040: //status core400S
        case 0x013140:
            if (rx_buf.size() != 28)
                return;

            sprintf(buf, "%d.%d.%d",rx_buf[12], rx_buf[11],rx_buf[10]);
            //ESP_LOGD(TAG, "Buf type %s", buf);
            

            defer("pub_fw", [=, this]() { 
                auto mcu_state = textsensor_mcu_fw->get_state();
                if(mcu_state!=buf){
                    textsensor_mcu_fw->publish_state(buf); 
                }
            });
                
            switch(rx_buf[13]) {  //Power
                case 0x00:
                defer_publish_text("pub_pw", textsensor_power, "Off");
                break;

                case 0x01:
                defer_publish_text("pub_pw", textsensor_power, "On");
                break;

                default:
                sprintf(buf, "Error %X", rx_buf[13]);
                defer("pub_pw", [=, this]()-> void {textsensor_power->publish_state(buf); });
            }

            switch(rx_buf[14]) {  //Fan Mode
                case 0x00:
                defer_publish_text("pub_md", textsensor_fan_mode, "Manual");  
                break;

                case 0x01:
                defer_publish_text("pub_md", textsensor_fan_mode, "Sleep");  
                break;

                case 0x02:
                defer_publish_text("pub_md", textsensor_fan_mode, "Auto");  
                break;                

                default:
                sprintf(buf, "Error %X", rx_buf[14]);
                defer("pub_md", [=, this]()-> void {textsensor_fan_mode->publish_state(buf); });
            }   

            switch(rx_buf[15]) {  //Fan Speed,  core400S different!
                case 0x00:
                defer_publish_text("pub_sp", textsensor_fan_speed, "Sleep");
                break;

                case 0x01:
                defer_publish_text("pub_sp", textsensor_fan_speed, "Low");
                break;

                case 0x02:
                defer_publish_text("pub_sp", textsensor_fan_speed, "Medium");
                break;  

                case 0x03:
                defer_publish_text("pub_sp", textsensor_fan_speed, "High");
                break;
                
                case 0x04:
                defer_publish_text("pub_sp", textsensor_fan_speed, "Highest");
                break;

                case 0xFF:
                defer_publish_text("pub_sp", textsensor_fan_speed, "Off");
                break;  

                default:
                sprintf(buf, "Error %X", rx_buf[18]);
                defer("pub_psp", [=, this]()-> void {textsensor_fan_speed->publish_state(buf); });
            } 

            switch(rx_buf[17]) {  //Screen,  core400S different!
                case 0x00:
                defer_publish_text("pub_lit", textsensor_display_lit, "Off");
                break;

                case 0x64:
                defer_publish_text("pub_lit", textsensor_display_lit, "On");
                break;

                default:
                sprintf(buf, "Error %X", rx_buf[16]);
                defer("pub_lit", [=, this]()-> void {textsensor_display_lit->publish_state(buf); });
            }
            float pm25;
            if (rx_buf[22] == 0xFF && rx_buf[21] == 0xFF) {
                // 0xFFFF returned when off
                pm25 = NAN;
            } else {
                pm25 = static_cast<float>((rx_buf[22] << 8) | rx_buf[21]);
            }
            defer_publish_number("pub_pm",sensor_pm25,pm25);

            switch(rx_buf[23]) {  //Display Lock
                case 0x00:
                defer_publish_text("pub_dl", textsensor_display_locked, "Unlocked");
                break;

                case 0x01:
                defer_publish_text("pub_dl", textsensor_display_locked, "Locked");
                break;

                default:
                sprintf(buf, "Error %X", rx_buf[23]);
                defer("pub_dl", [=, this]()-> void {textsensor_display_locked->publish_state(buf); });
            }

            switch(rx_buf[24]) {  //Fan Mode
                case 0x00:
                defer_publish_text("pub_fa", textsensor_fan_auto_mode, "Default");
                break;

                case 0x01:
                defer_publish_text("pub_fa", textsensor_fan_auto_mode, "Quiet");
                break;

                case 0x02:
                defer_publish_text("pub_fa", textsensor_fan_auto_mode, "Room Size");
                break;                

                default:
                sprintf(buf, "Error %X", rx_buf[24]);
                defer("pub_fa", [=, this]()-> void {textsensor_fan_auto_mode->publish_state(buf); });
            }

           // areaL=rx_buf[25];
          //  areaH=rx_buf[26];
          uint16_t room_size = (rx_buf[26] << 8) | rx_buf[25];
          defer("pub_ar", [=, this]()-> void {
            auto current = sensor_room_size->get_state();
            if (current != room_size) {
                sensor_room_size->publish_state(room_size);
            }
          });

        break;
    }

}

void Core400sComponent::power_on(void) {
    ESP_LOGD(TAG, "power_on");
    send_command(cmd_power_on);
}
    
void Core400sComponent::power_off(void) {
    ESP_LOGD(TAG, "power_off");
    send_command(cmd_power_off);    
}

void Core400sComponent::set_fan_manual_highest(void) {
    ESP_LOGD(TAG, "set_fan_manual_highest");
    send_command(cmd_set_fan_manual_highest);
}
  

void Core400sComponent::set_fan_manual_high(void) {
    ESP_LOGD(TAG, "set_fan_manual_high");
    send_command(cmd_set_fan_manual_high);
}
    
void Core400sComponent::set_fan_manual_medium(void) {
    ESP_LOGD(TAG, "set_fan_manual_medium");
    send_command(cmd_set_fan_manual_medium);    
}
    
void Core400sComponent::set_fan_manual_low(void) {
    ESP_LOGD(TAG, "set_fan_manual_low");
    send_command(cmd_set_fan_manual_low);
}

void Core400sComponent::set_fan_mode_manual(void) {
    ESP_LOGD(TAG, "set_fan_mode_manual");
    send_command(cmd_set_fan_mode_manual);
}

void Core400sComponent::set_fan_mode_sleep(void) {
    ESP_LOGD(TAG, "set_fan_mode_sleep");
    send_command(cmd_set_fan_mode_sleep);
}
    
void Core400sComponent::set_fan_mode_auto(void) {
    ESP_LOGD(TAG, "set_fan_mode_auto");
    send_command(cmd_set_fan_mode_auto);    
}
    
void Core400sComponent::set_fan_auto_ppm(void) {
    ESP_LOGD(TAG, "cmd_set_fan_auto_ppm");
    send_command(cmd_set_fan_auto_ppm);
}
    
void Core400sComponent::set_fan_auto_ppmQuiet(void) {
    ESP_LOGD(TAG, "set_fan_auto_ppmQuiet");
    send_command(cmd_set_fan_auto_ppmQuiet);
}
    
void Core400sComponent::set_fan_auto_area(void) {
    ESP_LOGD(TAG, "set_fan_auto_area");
    cmd_set_fan_auto_area[11]=areaL;
    cmd_set_fan_auto_area[12]=areaH;
    send_command(cmd_set_fan_auto_area);    
}

void Core400sComponent::lock_display(void) {
    ESP_LOGD(TAG, "lock_display");
    send_command(cmd_lock_display);
}

void Core400sComponent::unlock_display(void) {
    ESP_LOGD(TAG, "unlock_display");
    send_command(cmd_unlock_display);
}
    
void Core400sComponent::display_on(void) {
    ESP_LOGD(TAG, "display_on");
    send_command(cmd_display_on);    
}
    
void Core400sComponent::display_off(void) {
    ESP_LOGD(TAG, "display_off");
    send_command(cmd_display_off);
}

void Core400sComponent::wifi_led_off(void) {
    if (wifi_led==0) {
        return;
    }
    ESP_LOGD(TAG, "wifi_led_off");
    send_command(cmd_wifi_led_off);
    wifi_led=0;
}
    
void Core400sComponent::wifi_led_flash(void) {
    if (wifi_led==2) {
        return;
    }
    ESP_LOGD(TAG, "wifi_led_flash");
            send_command(cmd_wifi_led_flash);    
    wifi_led=2;
}
    
void Core400sComponent::wifi_led_on(void) {
    if (wifi_led==1) {
        return;
    }
    ESP_LOGD(TAG, "wifi_led_on");
    send_command(cmd_wifi_led_on);
    wifi_led=1;
}      

void Core400sComponent::filter_led_on(void) {
    ESP_LOGD(TAG, "filter_led_on");
    send_command(cmd_filter_led_on);    
}
    
void Core400sComponent::filter_led_off(void) {
    ESP_LOGD(TAG, "filter_led_off");
    send_command(cmd_filter_led_off);
}

} // namespace core400s
} // namespace esphome