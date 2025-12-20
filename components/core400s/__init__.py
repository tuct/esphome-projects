import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor, binary_sensor, text_sensor, uart
from esphome.const import *

core400s_ns = cg.esphome_ns.namespace('core400s')
Core400sComponent = core400s_ns.class_('Core400sComponent', cg.PollingComponent)

DEPENDENCIES = ['uart']
AUTO_LOAD = ['uart', 'sensor', 'text_sensor']

CONF_SENSOR_PM25 = "pm25"
CONF_SENSOR_ROOMSIZE = "room_size"
CONF_TEXTSENSOR_FAN_SPEED = "fan_speed"
CONF_TEXTSENSOR_POWER = "power"
CONF_TEXTSENSOR_MCU_VERSION = "mcu_version"
CONF_TEXTSENSOR_FAN_MODE = "fan_mode"
CONF_TEXTSENSOR_DISPLAY_LIT = "display_lit"
CONF_TEXTSENSOR_DISPLAY_LOCKED = "display_locked"
CONF_TEXTSENSOR_FAN_AUTO_MODE = "fan_auto_mode"

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(Core400sComponent),

    cv.Optional(CONF_SENSOR_PM25):
        sensor.sensor_schema(accuracy_decimals=0).extend(),

    cv.Optional(CONF_SENSOR_ROOMSIZE):
        sensor.sensor_schema(accuracy_decimals=0).extend(),

    cv.Optional(CONF_TEXTSENSOR_FAN_SPEED):
        text_sensor.text_sensor_schema().extend(),
        
    cv.Optional(CONF_TEXTSENSOR_POWER):
        text_sensor.text_sensor_schema().extend(),

    cv.Optional(CONF_TEXTSENSOR_MCU_VERSION):
        text_sensor.text_sensor_schema().extend(),

    cv.Optional(CONF_TEXTSENSOR_FAN_MODE):
        text_sensor.text_sensor_schema().extend(),

    cv.Optional(CONF_TEXTSENSOR_DISPLAY_LIT):
        text_sensor.text_sensor_schema().extend(),

    cv.Optional(CONF_TEXTSENSOR_DISPLAY_LOCKED):
        text_sensor.text_sensor_schema().extend(),

    cv.Optional(CONF_TEXTSENSOR_FAN_AUTO_MODE):
        text_sensor.text_sensor_schema().extend()

}).extend(cv.polling_component_schema('5s')).extend(uart.UART_DEVICE_SCHEMA)

def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    yield cg.register_component(var, config)
    yield uart.register_uart_device(var, config)
    
    if CONF_SENSOR_PM25 in config:
        conf = config[CONF_SENSOR_PM25]
        sens = yield sensor.new_sensor(conf)
        cg.add(var.set_pm25_sensor(sens))
        
    if CONF_SENSOR_ROOMSIZE in config:
        conf = config[CONF_SENSOR_ROOMSIZE]
        sens = yield sensor.new_sensor(conf)
        cg.add(var.set_room_size_sensor(sens))

    if CONF_TEXTSENSOR_FAN_SPEED in config:
        conf = config[CONF_TEXTSENSOR_FAN_SPEED]
        sens = yield text_sensor.new_text_sensor(conf)
        cg.add(var.set_textsensor_fan_speed(sens))
        
    if CONF_TEXTSENSOR_POWER in config:
        conf = config[CONF_TEXTSENSOR_POWER]
        sens = yield text_sensor.new_text_sensor(conf)
        cg.add(var.set_textsensor_power(sens))
        
    if CONF_TEXTSENSOR_MCU_VERSION in config:
        conf = config[CONF_TEXTSENSOR_MCU_VERSION]
        sens = yield text_sensor.new_text_sensor(conf)
        cg.add(var.set_textsensor_mcu_fw(sens))
    
    if CONF_TEXTSENSOR_FAN_MODE in config:
        conf = config[CONF_TEXTSENSOR_FAN_MODE]
        sens = yield text_sensor.new_text_sensor(conf)
        cg.add(var.set_textsensor_fan_mode(sens))
    
    if CONF_TEXTSENSOR_DISPLAY_LIT in config:
        conf = config[CONF_TEXTSENSOR_DISPLAY_LIT]
        sens = yield text_sensor.new_text_sensor(conf)
        cg.add(var.set_textsensor_display_lit(sens))
    
    if CONF_TEXTSENSOR_DISPLAY_LOCKED in config:
        conf = config[CONF_TEXTSENSOR_DISPLAY_LOCKED]
        sens = yield text_sensor.new_text_sensor(conf)
        cg.add(var.set_textsensor_display_locked(sens))
    
    if CONF_TEXTSENSOR_FAN_AUTO_MODE in config:
        conf = config[CONF_TEXTSENSOR_FAN_AUTO_MODE]
        sens = yield text_sensor.new_text_sensor(conf)
        cg.add(var.set_textsensor_fan_auto_mode(sens))
