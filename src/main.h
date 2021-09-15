#ifndef __MAIN_H__
#define __MAIN_H__

#include <Arduino.h>
#include "ats_02_setup.h"
#ifdef RGB_LED_ENABLED
    #include "RGBLed.h"
#endif
#ifdef SENSOR_DHT_ENABLED
    #include <Adafruit_Sensor.h>
    #include <DHT_U.h>
#endif
#ifdef I2C1_ENABLED
    #include <Wire.h>
#endif
#ifdef SENSOR_LIGHT_ENABLED
    #include <BH1750.h>
#endif

struct station_sensor_t {
    #ifdef SENSOR_DHT_ENABLED
        float airTemp = 0.0f;
        uint8_t airTempCount = 0;
        float airHumid = 0.0f;
        uint8_t airHumidCount = 0;
    #endif
    #ifdef SENSOR_LIGHT_ENABLED
        uint32_t light = 0;
        uint8_t lightCount = 0;
    #endif
    #ifdef SENSOR_UV_ENABLED
        uint32_t uvVoltage = 0;
        uint8_t uvVoltageCount = 0;
    #endif
};

/*******************************************************
 *                FUNCTIONS PROTOTYPES
 *******************************************************/
#ifdef SERIAL_DEBUG_ENABLED
    void printInitInfo();
#endif
#ifdef SENSOR_DHT_ENABLED
    uint8_t initSensorDHT();
    uint8_t getDHTTemperature();
    uint8_t getDHTHumidity();
#endif
#ifdef SENSOR_UV_ENABLED
    uint8_t convertVoltsToIndex(uint16_t miliVolts);
#endif

/*******************************************************
 *                  GLOBAL VARIABLES
 *******************************************************/
unsigned long now = 0;
unsigned long lastSystemPeriod = 0;
bool turnAroundSystemOK = false;
unsigned long lastSamplingPeriod = 0;
bool turnAroundSamplingOK = false;
unsigned long lastTxPeriod = 0;
bool turnAroundTxOK = false;
station_sensor_t sensorsData;
#ifdef RGB_LED_ENABLED
    RGBLed rgb_led(LED_RGB_TYPE, LED_RGB_RED_PIN, LED_RGB_GREEN_PIN, LED_RGB_BLUE_PIN);  /**< Global variable to access RGB LED device. */
#endif
#ifdef SENSOR_DHT_ENABLED
    DHT_Unified dht(DHT_PIN, DHT_TYPE);                         /**< Global variable to access DHT sensor. */
#endif
#ifdef SENSOR_LIGHT_ENABLED
    BH1750 lightSensor; /**< Global variable to access light sensor (GY-30). */
#endif

/*******************************************************
 *             FUNCTIONS IMPLEMENTATIONS
 *******************************************************/

#ifdef SERIAL_DEBUG_ENABLED
void printInitInfo() {
    SERIAL_DEBUG.print(F("\n#############################################\n"));
    SERIAL_DEBUG.print(F("#                   ATS-02                  #\n"));
    SERIAL_DEBUG.print(F("#            AgroTechStation - 02           #\n"));
    SERIAL_DEBUG.print(F("#    Instituto Federal de Santa Catarina    #\n"));
    SERIAL_DEBUG.print(F("#                Campus Lages               #\n"));
    SERIAL_DEBUG.print(F("#         developed by AgroTechLab          #\n"));
    SERIAL_DEBUG.print(F("#############################################\n"));
    SERIAL_DEBUG.print(F("Starting ATS-02..."));
    SERIAL_DEBUG.print(F("\n\tPower supply..............: "));
    SERIAL_DEBUG.print(power_supply_str[POWER_SUPPLY]);
    SERIAL_DEBUG.print(F("\n\tMCU device board..........: "));
    SERIAL_DEBUG.print(MCU_BOARD);
    SERIAL_DEBUG.print(F("\n\tCommunication interface...: "));
    SERIAL_DEBUG.print(COMM_IF);
    SERIAL_DEBUG.print(F("\n\tFirmware version..........: "));
    SERIAL_DEBUG.print(FW_VERSION);
    SERIAL_DEBUG.print(F("\n\tHardware version..........: "));
    SERIAL_DEBUG.print(HW_VERSION);
    SERIAL_DEBUG.print(F("\n\tSensor list...............: "));
    SERIAL_DEBUG.print(DEV_SENSOR_LIST);
    SERIAL_DEBUG.print(F("\n\tActuator list.............: "));
    SERIAL_DEBUG.print(DEV_ACTUATOR_LIST);
    SERIAL_DEBUG.print(F("\n\tAwaiting 5 seconds for systems stabilization..."));    
}
#endif // SERIAL_DEBUG_ENABLED

#ifdef SENSOR_DHT_ENABLED
uint8_t initSensorDHT() {
    uint8_t status = 0;
    #ifdef SERIAL_DEBUG_ENABLED
        SERIAL_DEBUG.print(F("\n\tInitiating DHT sensor... "));
        SERIAL_DEBUG.flush();
    #endif
    pinMode(DHT_PIN, INPUT);
    dht.begin();
    sensor_t dht_sensor;
    dht.temperature().getSensor(&dht_sensor);
    if (strcmp(dht_sensor.name, "DHT22") == 0) {
        #ifdef SERIAL_DEBUG_ENABLED
            SERIAL_DEBUG.print(F("\n\t\tSensor type...: ")); SERIAL_DEBUG.print(dht_sensor.name);
            SERIAL_DEBUG.print(F("\n\t\tDriver version: ")); SERIAL_DEBUG.print(dht_sensor.version);
            SERIAL_DEBUG.print(F("\n\t\tUnique ID.....: ")); SERIAL_DEBUG.print(dht_sensor.sensor_id);
            SERIAL_DEBUG.print(F("\n\t\tMinimum delay.: ")); SERIAL_DEBUG.print(dht_sensor.min_delay/1000); SERIAL_DEBUG.print(F(" ms"));
            SERIAL_DEBUG.print(F("\n\t\tAir Temperature "));
            SERIAL_DEBUG.print(F("\n\t\t\tMinimum value...: ")); SERIAL_DEBUG.print(dht_sensor.min_value); SERIAL_DEBUG.print(F(" oC"));
            SERIAL_DEBUG.print(F("\n\t\t\tMaximum value...: ")); SERIAL_DEBUG.print(dht_sensor.max_value); SERIAL_DEBUG.print(F(" oC"));
            SERIAL_DEBUG.print(F("\n\t\t\tResolution......:   ")); SERIAL_DEBUG.print(dht_sensor.resolution); SERIAL_DEBUG.print(F(" oC"));
            SERIAL_DEBUG.flush();
        #endif
        dht.humidity().getSensor(&dht_sensor);
        #ifdef SERIAL_DEBUG_ENABLED
            SERIAL_DEBUG.print(F("\n\t\tAir Humidity "));
            SERIAL_DEBUG.print(F("\n\t\t\tMinimum value...:   ")); SERIAL_DEBUG.print(dht_sensor.min_value); SERIAL_DEBUG.print(F(" %"));
            SERIAL_DEBUG.print(F("\n\t\t\tMaximum value...: ")); SERIAL_DEBUG.print(dht_sensor.max_value); SERIAL_DEBUG.print(F(" %"));
            SERIAL_DEBUG.print(F("\n\t\t\tResolution......:   ")); SERIAL_DEBUG.print(dht_sensor.resolution); SERIAL_DEBUG.print(F(" %"));
            SERIAL_DEBUG.flush();
        #endif
    } else {
        #ifdef SERIAL_DEBUG_ENABLED
            SERIAL_DEBUG.print(F("[FAIL]"));
            SERIAL_DEBUG.flush();
        #endif
        status = 1;
    }
    return status;
}

uint8_t getDHTTemperature() {
    sensors_event_t event;
    dht.temperature().getEvent(&event);
    if (isnan(event.temperature)) {
        #ifdef SERIAL_DEBUG_ENABLED
            SERIAL_DEBUG.println(F("Error reading air temperature!"));
            SERIAL_DEBUG.flush();
        #endif
        return 1;
    }
    else {
        sensorsData.airTemp += event.temperature;
        sensorsData.airTempCount++;
        #ifdef SERIAL_DEBUG_ENABLED
            SERIAL_DEBUG.print(F("\nAir temperature: "));
            SERIAL_DEBUG.print(event.temperature);
            SERIAL_DEBUG.print(F(" oC"));
            SERIAL_DEBUG.flush();
        #endif
        return 0;
    }
}

uint8_t getDHTHumidity() {
    sensors_event_t event;
    dht.humidity().getEvent(&event);
    if (isnan(event.relative_humidity)) {
        #ifdef SERIAL_DEBUG_ENABLED
            SERIAL_DEBUG.println(F("Error reading air humidity!"));
            SERIAL_DEBUG.flush();
        #endif
        return 1;
    }
    else {
        sensorsData.airHumid += event.relative_humidity;
        sensorsData.airHumidCount++;
        #ifdef SERIAL_DEBUG_ENABLED
            SERIAL_DEBUG.print(F("\nAir humidity: "));
            SERIAL_DEBUG.print(event.relative_humidity);
            SERIAL_DEBUG.print(F(" %"));
            SERIAL_DEBUG.flush();
        #endif
        return 0;
    }
}
#endif

void resetSensorDataStruct() {
    #ifdef SENSOR_DHT_ENABLED
        sensorsData.airTemp = 0.0f;
        sensorsData.airTempCount = 0;
        sensorsData.airHumid = 0.0f;
        sensorsData.airHumidCount = 0;
    #endif
    #ifdef SENSOR_LIGHT_ENABLED
        sensorsData.light = 0;
        sensorsData.lightCount = 0;
    #endif
    #ifdef SENSOR_UV_ENABLED
        sensorsData.uvVoltage = 0;
        sensorsData.uvVoltageCount = 0;
    #endif
    #ifdef SENSOR_SOIL_MOISTURE_ENABLED
        sensorsData.soil_moisture = 0;
    #endif
    #ifdef SENSOR_SOIL_TEMP_ENABLED
        sensorsData.soil_temperature = 0.0f;
    #endif
//     // uint8_t leaf_surface_moisture[2] = {0, 0};
    #ifdef SENSOR_WIND_SOCK_ENABLED
        sensorsData.wind_dir = 0;
    #endif
    #ifdef SENSOR_ANEMOMETER_ENABLED
        sensorsData.wind_speed = 0;
    #endif    
}

#ifdef SENSOR_UV_ENABLED
uint8_t convertVoltsToIndex(uint16_t miliVolts) {
    if (miliVolts < 227) {
        return 0;
    } else if (miliVolts >= 227 && miliVolts < 318) {
        return 1;
    } else if (miliVolts >= 318 && miliVolts < 408) {
        return 2;
    } else if (miliVolts >= 408 && miliVolts < 503) {
        return 3;
    } else if (miliVolts >= 503 && miliVolts < 606) {
        return 4;
    } else if (miliVolts >= 606 && miliVolts < 696) {
        return 5;
    } else if (miliVolts >= 696 && miliVolts < 795) {
        return 6;
    } else if (miliVolts >= 795 && miliVolts < 881) { 
        return 7;
    } else if (miliVolts >= 881 && miliVolts < 976) {
        return 8;
    } else if (miliVolts >= 976 && miliVolts < 1079) {
        return 9;
    } else if (miliVolts >= 1079 && miliVolts < 1170) {
        return 10;
    } else if (miliVolts >= 1170) {
        return 11;
    }  
}
#endif 

#endif // #ifndef __MAIN_H__