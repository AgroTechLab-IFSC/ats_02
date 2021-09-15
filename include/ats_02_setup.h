#ifndef __ATS_02_SETUP_H__
#define __ATS_02_SETUP_H__

#include <Arduino.h>

enum power_supply_e {BATTERY, POWER_LINE};
const char* power_supply_str[] = {"Battery", "Power Line"};

/**
 * \def POWER_SUPPLY 
 * Define the power supply of AgroTechStation (BATTERY/POWER_LINE) 
 */
#define POWER_SUPPLY            POWER_LINE

/**
 * \def SERIAL_DEBUG_ENABLED 
 * Enable or disable the serial debug.
 */
#define SERIAL_DEBUG_ENABLED

#if (POWER_SUPPLY == POWER_LINE)
    /**
    * \def RGB_LED_ENABLED 
    * Enable or disable RGB led.
    */
    #define RGB_LED_ENABLED
#endif

/**
 * \def SENSOR_DHT_ENABLED 
 * Enable or disable the DHT sensor.
 */
#define SENSOR_DHT_ENABLED

/**
 * \def I2C1_ENABLED 
 * Enable or disable I2C 1 interface.
 */
#define I2C1_ENABLED

#ifdef I2C1_ENABLED
    /**
    * \def SENSOR_LIGHT_ENABLED 
    * Enable or disable the LIGHT sensor.
    */
    #define SENSOR_LIGHT_ENABLED
#endif

/**
 * \def SENSOR_UV_ENABLED 
 * Enable or disable UV sensor.
 */
#define SENSOR_UV_ENABLED

/**
 * \def ONE_WIRE_ENABLED 
 * Enable or disable one wire interface.
 */
#define ONE_WIRE_ENABLED

#ifdef ONE_WIRE_ENABLED
    /**
    * \def SENSOR_SOIL_TEMP_ENABLED 
    * Enable or disable soil temperature sensor.
    */
    #define SENSOR_SOIL_TEMP_ENABLED    
#endif

/**
 * \def SENSOR_SOIL_MOISTURE_ENABLED 
 * Enable or disable soil moisture sensor.
 */
#define SENSOR_SOIL_MOISTURE_ENABLED

/*******************************************************
 *                 DEVICE PARAMETERS
 *******************************************************/
/**
 * \def MCU_BOARD 
 * MCU device board.
 */
#define MCU_BOARD               "MEGA2560"

/**
 * \def COMM_IF 
 * Communication interface.
 */
#define COMM_IF                 "LoRaWAN - RHF76-052"

/**
 * \def FW_VERSION 
 * Firmware version.
 */
#define FW_VERSION              "0.1.0"

/**
 * \def HW_VERSION 
 * Hardware version.
 */
#define HW_VERSION              "0.1.0"

/**
 * \def DEV_SENSOR_LIST 
 * List of sensors enabled in the station.
 */
#define DEV_SENSOR_LIST         "DHT22 | GY-30 | UVM-30A | DS18B20 | HD-38"

/**
 * \def DEV_ACTUATOR_LIST 
 * List of actuators enabled in the station.
 */
#define DEV_ACTUATOR_LIST       "none"


#ifdef SERIAL_DEBUG_ENABLED
    /**
    * \def SERIAL_BAUDRATE 
    * Define the serial baudrate.
    */
    #define SERIAL_BAUDRATE     115200
#endif

#ifdef RGB_LED_ENABLED
    /**
    * \def LED_RGB_TYPE 
    * RGB LED type (anode/cathode).
    */
    #define LED_RGB_TYPE                ANODE
#endif

#ifdef SERIAL_DEBUG_ENABLED    
    /**
    * \def SERIAL_DEBUG 
    * Serial used to DEBUG terminal
    */    
    #define SERIAL_DEBUG    Serial
#endif

/**
 * \def SERIAL_LORA 
 * Serial used to LoRaWAN
 */
#define SERIAL_LORA     loraSerial

/*******************************************************
 *                   SYSTEM PINOUT
 *******************************************************/
 /**
  * \def LED_BUILTIN_PIN 
  * LED builtin pin.
  */
#define LED_BUILTIN_PIN         13

#ifdef RGB_LED_ENABLED
    /**
    * \def LED_RGB_RED_PIN
    * Red RGB LED device pin.
    */
    #define LED_RGB_RED_PIN             2

    /**
    * \def LED_RGB_GREEN_PIN 
    * Green RGB LED device pin.
    */
    #define LED_RGB_GREEN_PIN           3

    /**
    * \def LED_RGB_BLUE_PIN
    * Blue RGB LED device pin.
    */
    #define LED_RGB_BLUE_PIN            4
#endif

#ifdef SENSOR_DHT_ENABLED
    /**
    * \def DHT_PIN 
    * DHT sensor pin.
    */
    #define DHT_PIN                     53

    /**
    * \def DHT_TYPE 
    * DHT sensor type.
    */
    #define DHT_TYPE                    DHT22
#endif

#ifdef I2C1_ENABLED
    /**
    * \def I2C1_SDA_PIN 
    * SDA pin of I2C 1 interface.
    */
    #define I2C1_SDA_PIN                20

    /**
    * \def I2C1_SCL_PIN 
    * SCL pin of I2C 1 interface.
    */
    #define I2C1_SCL_PIN                21
#endif

#ifdef SENSOR_UV_ENABLED
    /**
    * \def SENSOR_UV_PIN 
    * UV sensor pin.
    */
    #define SENSOR_UV_PIN               A0
#endif

#ifdef ONE_WIRE_ENABLED
    /**
    * \def ONE_WIRE_PIN 
    * One Wire interface pin.
    */
    #define ONE_WIRE_PIN                51
#endif

#ifdef SENSOR_SOIL_MOISTURE_ENABLED
    /**
    * \def SENSOR_SOIL_MOISTURE_PIN 
    * Soil moisture sensor pin.
    */
    #define SENSOR_SOIL_MOISTURE_PIN    A1
#endif

/*******************************************************
 *                  SYSTEM PARAMETERS
 *******************************************************/
const unsigned long systemPeriod = 1000;                        /**< System run period (in ms). */
const unsigned long samplingPeriod = 10 * systemPeriod;          /**< Sampling period (in ms). */
const unsigned long txPeriod = 5 * samplingPeriod;              /**< Transmission period (in ms). */


#endif // #ifndef __ATS_02_SETUP_H__