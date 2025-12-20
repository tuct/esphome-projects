#pragma once

#define USE_UART_DEBUGGER

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/text_sensor/text_sensor.h"
#include "esphome/components/uart/uart.h"
#include <vector>
#include "esphome/components/uart/uart_debugger.h"

static const char *TAG = "component.core300s_UART";

namespace esphome {
namespace core300s {

class Core300sComponent : public PollingComponent, public uart::UARTDevice {
  public:
    void setup() override;
    void update() override;
    void dump_config() override;
    float get_setup_priority() const override;

    void set_pm25_sensor(sensor::Sensor *s) { sensor_pm25 = s; }
    void set_room_size_sensor(sensor::Sensor *s) { sensor_room_size = s; }
    void set_textsensor_fan_speed(text_sensor::TextSensor *s) { textsensor_fan_speed = s; }
    void set_textsensor_power(text_sensor::TextSensor *s) { textsensor_power = s; }
    void set_textsensor_mcu_fw(text_sensor::TextSensor *s) { textsensor_mcu_fw = s; }
    void set_textsensor_fan_mode(text_sensor::TextSensor *s) { textsensor_fan_mode = s; }
    void set_textsensor_display_lit(text_sensor::TextSensor *s) { textsensor_display_lit = s; }
    void set_textsensor_display_locked(text_sensor::TextSensor *s) { textsensor_display_locked = s; }
    void set_textsensor_fan_auto_mode(text_sensor::TextSensor *s) { textsensor_fan_auto_mode = s; }

    void power_on(void);
    void power_off(void);
    void set_fan_manual_high(void);
    void set_fan_manual_medium(void);
    void set_fan_manual_low(void);
    void set_fan_mode_manual(void);
    void set_fan_mode_sleep(void);
    void set_fan_mode_auto(void);
    void set_fan_auto_ppm(void);
    void set_fan_auto_ppmQuiet(void);
    void set_fan_auto_area(void);
    void lock_display(void);
    void unlock_display(void);
    void display_on(void);
    void display_off(void);
    void wifi_led_off(void);
    void wifi_led_flash(void);
    void wifi_led_on(void);
    void filter_led_on(void);
    void filter_led_off(void);    

  protected:
    sensor::Sensor *sensor_pm25;
    sensor::Sensor *sensor_room_size;
    
    text_sensor::TextSensor *textsensor_fan_speed;
    text_sensor::TextSensor *textsensor_power;
    text_sensor::TextSensor *textsensor_mcu_fw;
    text_sensor::TextSensor *textsensor_fan_mode;
    text_sensor::TextSensor *textsensor_display_lit;
    text_sensor::TextSensor *textsensor_display_locked;
    text_sensor::TextSensor *textsensor_fan_auto_mode;
    void defer_publish_text(const std::string &defer_id,
                          esphome::text_sensor::TextSensor *sensor,
                          const std::string &new_state);
                        
    void defer_publish_number(const std::string &defer_id,
                            esphome::sensor::Sensor *sensor,
                            float new_state);
    uint8_t b=0;
    std::vector<uint8_t> rx_buf,tx_buf;
    uint8_t tx_seq_num = 0;
    uint8_t wifi_led = 0;
    uint8_t areaH=0x01, areaL=0x3B;

    std::vector<uint8_t> cmd_get_status            = {0XA5, 0X22, 0XFF, 0X04, 0X00, 0XFF, 0X01, 0X31, 0X40, 0X00};

    std::vector<uint8_t> cmd_power_on              = {0XA5, 0X22, 0XFF, 0X05, 0X00, 0XFF, 0X01, 0X00, 0XA0, 0X00, 0X01};
    std::vector<uint8_t> cmd_power_off             = {0XA5, 0X22, 0XFF, 0X05, 0X00, 0XFF, 0X01, 0X00, 0xA0, 0X00, 0X00};  

    std::vector<uint8_t> cmd_set_fan_manual_high   = {0XA5, 0X22, 0XFF, 0X07, 0X00, 0XFF, 0X01, 0X60, 0XA2, 0X00, 0X00, 0X01, 0X03};
    std::vector<uint8_t> cmd_set_fan_manual_medium = {0XA5, 0X22, 0XFF, 0X07, 0X00, 0XFF, 0X01, 0X60, 0XA2, 0X00, 0X00, 0X01, 0X02};
    std::vector<uint8_t> cmd_set_fan_manual_low    = {0XA5, 0X22, 0XFF, 0X07, 0X00, 0XFF, 0X01, 0X60, 0XA2, 0X00, 0X00, 0X01, 0X01};

    std::vector<uint8_t> cmd_set_fan_auto_ppm      = {0XA5, 0X22, 0XFF, 0X07, 0X00, 0XFF, 0X01, 0XE6, 0XA5, 0X00, 0X00, 0X00, 0X00};
    std::vector<uint8_t> cmd_set_fan_auto_ppmQuiet = {0XA5, 0X22, 0XFF, 0X07, 0X00, 0XFF, 0X01, 0XE6, 0XA5, 0X00, 0X01, 0x00, 0X00};
    std::vector<uint8_t> cmd_set_fan_auto_area     = {0XA5, 0X22, 0XFF, 0X07, 0X00, 0XFF, 0X01, 0XE6, 0XA5, 0X00, 0X02, 0X3B, 0X01};    
    
    std::vector<uint8_t> cmd_set_fan_mode_manual   = {0XA5, 0X22, 0XFF, 0X05, 0X00, 0XFF, 0X01, 0XE0, 0XA5, 0X00, 0X00};  //Not used by vesync app but works to restore last fan speed
    std::vector<uint8_t> cmd_set_fan_mode_sleep    = {0XA5, 0X22, 0XFF, 0X05, 0X00, 0XFF, 0X01, 0XE0, 0XA5, 0X00, 0X01};
    std::vector<uint8_t> cmd_set_fan_mode_auto     = {0XA5, 0X22, 0XFF, 0X05, 0X00, 0XFF, 0X01, 0XE0, 0XA5, 0X00, 0X02};

    std::vector<uint8_t> cmd_lock_display          = {0XA5, 0X22, 0XFF, 0X05, 0X00, 0XFF, 0X01, 0X00, 0XD1, 0X00, 0X01};
    std::vector<uint8_t> cmd_unlock_display        = {0XA5, 0X22, 0XFF, 0X05, 0X00, 0XFF, 0X01, 0X00, 0XD1, 0X00, 0X00};    

    std::vector<uint8_t> cmd_display_on            = {0XA5, 0X22, 0XFF, 0X05, 0X00, 0XFF, 0X01, 0X05, 0XA1, 0X00, 0X64};
    std::vector<uint8_t> cmd_display_off           = {0XA5, 0X22, 0XFF, 0X05, 0X00, 0XFF, 0X01, 0X05, 0XA1, 0X00, 0X00}; 

    std::vector<uint8_t> cmd_wifi_led_off          = {0XA5, 0X22, 0XFF, 0X0A, 0X00, 0XFF, 0X01, 0X29, 0XA1, 0X00, 0X00, 0XF4, 0X01, 0XF4, 0X01, 0X00};    
    std::vector<uint8_t> cmd_wifi_led_on           = {0XA5, 0X22, 0XFF, 0X0A, 0X00, 0XFF, 0X01, 0X29, 0XA1, 0X00, 0X01, 0X7D, 0X00, 0X7D, 0X00, 0X00};
    std::vector<uint8_t> cmd_wifi_led_flash        = {0XA5, 0X22, 0XFF, 0X0A, 0X00, 0XFF, 0X01, 0X29, 0XA1, 0X00, 0X02, 0XF4, 0X01, 0XF4, 0X01, 0X00};

    std::vector<uint8_t> cmd_filter_led_on         = {0XA5, 0X22, 0XFF, 0X05, 0X00, 0XFF, 0X01, 0XE2, 0XA5, 0X00, 0X01};
    std::vector<uint8_t> cmd_filter_led_off        = {0XA5, 0X22, 0XFF, 0X05, 0X00, 0XFF, 0X01, 0XE2, 0xA5, 0X00, 0X00};      

    void send_command(std::vector< uint8_t > &cmd);
    void acknowledge_packet();
    void process_packet();
};

} // namespace core300s
} // namespace esphome