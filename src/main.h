/**
 * @file main.h
 * @author Robson Costa (robson.costa@ifsc.edu.br)
 * @brief Main library.
 * @version 0.1.0
 * @since 2019-01-14 
 * @date 2021-09-17
 * 
 * @copyright Copyright (c) 2019 - Robson Costa\n
 * Licensed under the Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International Unported License (the <em>"License"</em>). 
 * You may not use this file except in compliance with the License. You may obtain a copy of the License at 
 * \url{https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode}. Unless required by applicable law or agreed to in writing,  
 * software distributed under the License is distributed on an <em>"as is" basis, without warranties or 
 * conditions of any kind</em>, either express or implied. See the License for the specific language governing 
 * permissions and limitations under the License.
 */
#ifndef __MAIN_H__
#define __MAIN_H__

#include <Arduino.h>
#include "ats_02_setup.h"
#include "LoRa.h"
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
#ifdef ONE_WIRE_ENABLED
    #include <OneWire.h>
#endif
#ifdef SENSOR_SOIL_TEMP_ENABLED
    #include <DallasTemperature.h>
#endif
#ifdef SENSOR_POWER_SUPPLY_ENABLED
    #include <Adafruit_INA219.h>
#endif
#ifdef SENSOR_PRESSURE_ENABLED
    #include <Adafruit_BMP085.h>
#endif

struct up_time_t {
    uint8_t days;
    uint8_t months;
    uint8_t years;
    uint8_t hours;
    uint8_t minutes;
    uint8_t seconds;
};

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
    #ifdef SENSOR_SOIL_TEMP_ENABLED
        float soilTemp = 0.0f;
        uint8_t soilTempCount = 0;
    #endif
    #ifdef SENSOR_SOIL_MOISTURE_ENABLED
        uint16_t soilMoisture = 0;
        uint8_t soilMoistureCount = 0;
    #endif    
    #ifdef SENSOR_WIND_SOCK_ENABLED
        float windDirVoltage = 0.0f;
        uint8_t windDirCount = 0;
    #endif
    #ifdef SENSOR_ANEMOMETER_ENABLED
        float windSpeed = 0.0f;        
        uint8_t windSpeedCount = 0;
        uint32_t anemometerTurnAround = 0;
        uint32_t lastWindSampling = 0;
    #endif
    #ifdef SENSOR_PLUVIOMETER_ENABLED
        float rainVolume = 0.0f;        
        uint16_t pluviometerTurnAround = 0;        
    #endif
    #ifdef SENSOR_POWER_SUPPLY_ENABLED
        float powerSupply = 0.0f;
        uint8_t powerSupplyCount = 0;
    #endif
    #ifdef SENSOR_LEAF_MOISTURE_ENABLED
        uint16_t leafMoisture = 0;
        uint8_t leafMoistureCount = 0;
    #endif
    #ifdef SENSOR_PRESSURE_ENABLED
        uint32_t pressure = 0;
        uint8_t pressureCount = 0;
        float devTemp = 0.0f;
        uint8_t devTempCount = 0;
    #endif
};

/*******************************************************
 *                FUNCTIONS PROTOTYPES
 *******************************************************/
#ifdef SERIAL_DEBUG_ENABLED
    void printInitInfo();
    void printAverageValues();
    up_time_t getUpTime(uint32_t milliSeconds);
#endif
#ifdef SENSOR_DHT_ENABLED
    uint8_t initSensorDHT();
    uint8_t getDHTTemperature();
    uint8_t getDHTHumidity();
#endif
#ifdef I2C1_ENABLED
    uint8_t initI2C();
    #ifdef SENSOR_LIGHT_ENABLED
        uint8_t initSensorLight();
        uint8_t getLightSensorValue();
    #endif
#endif
#ifdef SENSOR_UV_ENABLED
    uint8_t initSensorUV();
    uint8_t getUVSensorValue();
    uint8_t convertMilliVoltsToIndex(uint16_t milliVolts);
#endif
#ifdef ONE_WIRE_ENABLED
    uint8_t initOneWire();
    #ifdef SENSOR_SOIL_TEMP_ENABLED
        uint8_t initSensorSoilTemp();
        uint8_t getSoilTempSensorValue();
    #endif
#endif
#ifdef SENSOR_SOIL_MOISTURE_ENABLED
    uint8_t initSensorSoilMoisture();
    uint8_t getSoilMoistureSensorValue();
#endif
#ifdef SENSOR_LEAF_MOISTURE_ENABLED
    uint8_t initSensorLeafMoisture();
    uint8_t getLeafMoistureSensorValue();
#endif
#ifdef SENSOR_WIND_SOCK_ENABLED
    uint8_t initSensorWindSock();
    uint8_t getWindDirectionSensorValue();
    uint16_t convertVoltsToWindDirection(float voltage);
#endif
#ifdef SENSOR_ANEMOMETER_ENABLED
    uint8_t initSensorAnemometer();
    void anemometerTurnAroundIncrement();
    uint8_t getWindSpeedSensorValue();
#endif
#ifdef SENSOR_PLUVIOMETER_ENABLED
    uint8_t initSensorPluviometer();
    void pluviometerTurnAroundIncrement();
    uint8_t getRainVolumeSensorValue();
#endif
#ifdef SENSOR_POWER_SUPPLY_ENABLED
    uint8_t initSensorPowerSupply();
    uint8_t getPowerSupplySensorValue();
#endif
#ifdef SENSOR_PRESSURE_ENABLED
    uint8_t initSensorPressure();
    uint8_t getPressureSensorValue();
    uint8_t getDeviceTempSensorValue();
#endif
/*******************************************************
 *                  GLOBAL VARIABLES
 *******************************************************/
uint32_t now = 0;
uint32_t lastSystemPeriod = 0;
bool turnAroundSystemOK = false;
uint32_t lastSamplingPeriod = 0;
bool turnAroundSamplingOK = false;
uint32_t lastTxPeriod = 0;
bool turnAroundTxOK = false;
station_sensor_t sensorsData;
LoRaConfig_t loraCfg;                   /**< LoRa configuration struct. */
LoRa lora;                              /**< Global variable to access LoRa modem. */
String payload = "";
uint16_t payload_aux = 0;
#ifdef RGB_LED_ENABLED
    RGBLed rgb_led(LED_RGB_TYPE, LED_RGB_RED_PIN, LED_RGB_GREEN_PIN, LED_RGB_BLUE_PIN);  /**< Global variable to access RGB LED device. */
#endif
#ifdef SENSOR_DHT_ENABLED
    DHT_Unified dht(DHT_PIN, DHT_TYPE);                         /**< Global variable to access DHT sensor. */
#endif
#ifdef SENSOR_LIGHT_ENABLED
    BH1750 lightSensor; /**< Global variable to access light sensor (GY-30). */
#endif
#ifdef ONE_WIRE_ENABLED
    OneWire oneWire;
    #ifdef SENSOR_SOIL_TEMP_ENABLED
        DallasTemperature soil_temp_sensor;
        DeviceAddress soil_temp_sensor_addr;
    #endif
#endif
#ifdef SENSOR_POWER_SUPPLY_ENABLED
    Adafruit_INA219 ina219(0x40);
#endif
#ifdef SENSOR_PRESSURE_ENABLED
    Adafruit_BMP085 bmp;
#endif

/*******************************************************
 *             FUNCTIONS IMPLEMENTATIONS
 *******************************************************/

#ifdef SERIAL_DEBUG_ENABLED
/**
 * @fn printInitInfo
 * @brief Print initial information.
 */
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
    SERIAL_DEBUG.flush();    
}

void printAverageValues() {
    SERIAL_DEBUG.print(F("\n\n========= Transmitting average values at ")); SERIAL_DEBUG.print(now); SERIAL_DEBUG.print(F(" milliseconds ========="));
    SERIAL_DEBUG.print(F("\nAverage air temperature (in oC): ")); SERIAL_DEBUG.print(sensorsData.airTemp/sensorsData.airTempCount);
    SERIAL_DEBUG.print(F(" (")); SERIAL_DEBUG.print(sensorsData.airTempCount); SERIAL_DEBUG.print(F(" sampling)"));
    SERIAL_DEBUG.print(F("\nAverage air humidity (in %): ")); SERIAL_DEBUG.print(sensorsData.airHumid/sensorsData.airHumidCount);
    SERIAL_DEBUG.print(F(" (")); SERIAL_DEBUG.print(sensorsData.airHumidCount); SERIAL_DEBUG.print(F(" sampling)"));
    SERIAL_DEBUG.print(F("\nAverage light (in lux): ")); SERIAL_DEBUG.print(sensorsData.light/sensorsData.lightCount);
    SERIAL_DEBUG.print(F(" (")); SERIAL_DEBUG.print(sensorsData.lightCount); SERIAL_DEBUG.print(F(" sampling)"));
    SERIAL_DEBUG.print(F("\nAverage UV tension (in milliVolts): ")); SERIAL_DEBUG.print(sensorsData.uvVoltage/sensorsData.uvVoltageCount);
    SERIAL_DEBUG.print(F(" => Index: ")); SERIAL_DEBUG.print(convertMilliVoltsToIndex(sensorsData.uvVoltage/sensorsData.uvVoltageCount));
    SERIAL_DEBUG.print(F(" (")); SERIAL_DEBUG.print(sensorsData.uvVoltageCount); SERIAL_DEBUG.print(F(" sampling)"));    
    SERIAL_DEBUG.print(F("\nAverage soil temperature (in oC): ")); SERIAL_DEBUG.print(sensorsData.soilTemp/sensorsData.soilTempCount);
    SERIAL_DEBUG.print(F(" (")); SERIAL_DEBUG.print(sensorsData.soilTempCount); SERIAL_DEBUG.print(F(" sampling)")); 
    SERIAL_DEBUG.print(F("\nAverage soil moisture (in %): ")); SERIAL_DEBUG.print(sensorsData.soilMoisture/sensorsData.soilMoistureCount);
    SERIAL_DEBUG.print(F(" (")); SERIAL_DEBUG.print(sensorsData.soilMoistureCount); SERIAL_DEBUG.print(F(" sampling)"));   
    SERIAL_DEBUG.print(F("\nAverage leaf moisture (in %): ")); SERIAL_DEBUG.print(sensorsData.leafMoisture/sensorsData.leafMoistureCount);
    SERIAL_DEBUG.print(F(" (")); SERIAL_DEBUG.print(sensorsData.leafMoistureCount); SERIAL_DEBUG.print(F(" sampling)"));   
    SERIAL_DEBUG.print(F("\nAverage wind direction (in Volts): ")); SERIAL_DEBUG.print(sensorsData.windDirVoltage/sensorsData.windDirCount);
    SERIAL_DEBUG.print(F(" => Direction (in degrees): ")); SERIAL_DEBUG.print(convertVoltsToWindDirection(sensorsData.windDirVoltage/sensorsData.windDirCount));
    SERIAL_DEBUG.print(F(" (")); SERIAL_DEBUG.print(sensorsData.windDirCount); SERIAL_DEBUG.print(F(" sampling)"));   
    SERIAL_DEBUG.print(F("\nAverage wind speed (in Km/h): ")); SERIAL_DEBUG.print(sensorsData.windSpeed/sensorsData.windSpeedCount);
    SERIAL_DEBUG.print(F(" (")); SERIAL_DEBUG.print(sensorsData.windSpeedCount); SERIAL_DEBUG.print(F(" sampling)")); 
    SERIAL_DEBUG.print(F("\nAverage rain volume (in ml): ")); SERIAL_DEBUG.print(sensorsData.rainVolume);    
    SERIAL_DEBUG.print(F("\nAverage power supply (in Volts): ")); SERIAL_DEBUG.print(sensorsData.powerSupply/sensorsData.powerSupplyCount);
    SERIAL_DEBUG.print(F(" (")); SERIAL_DEBUG.print(sensorsData.powerSupplyCount); SERIAL_DEBUG.print(F(" sampling)"));    
    SERIAL_DEBUG.print(F("\nAverage pressure (in hPa): ")); SERIAL_DEBUG.print(sensorsData.pressure/sensorsData.pressureCount);
    SERIAL_DEBUG.print(F(" (")); SERIAL_DEBUG.print(sensorsData.pressureCount); SERIAL_DEBUG.print(F(" sampling)"));    
    SERIAL_DEBUG.print(F("\nAverage device temperature (in oC): ")); SERIAL_DEBUG.print(sensorsData.devTemp/sensorsData.devTempCount);
    SERIAL_DEBUG.print(F(" (")); SERIAL_DEBUG.print(sensorsData.devTempCount); SERIAL_DEBUG.print(F(" sampling)"));    
    SERIAL_DEBUG.print(F("\n===========================================================\n"));    
    SERIAL_DEBUG.flush();
}


up_time_t getUpTime(uint32_t milliSeconds) {
    up_time_t time;
    uint32_t secRemaing = 0;
    uint32_t allSeconds = milliSeconds / 1000;
    time.years = allSeconds / (3600 * 24 * 30 * 12);
    secRemaing = allSeconds % (3600 * 24 * 30 * 12);
    time.months = secRemaing / (3600 * 24 * 30);
    secRemaing = secRemaing % (3600 * 24 * 30);
    time.days = secRemaing / (3600 * 24);
    secRemaing = secRemaing % (3600 * 24);    
    time.hours = secRemaing / 3600;
    secRemaing = secRemaing % 3600;
    time.minutes = secRemaing / 60;
    time.seconds = secRemaing % 60;
    return time;
}
#endif // SERIAL_DEBUG_ENABLED

#ifdef SENSOR_DHT_ENABLED
uint8_t initSensorDHT() {    
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
            SERIAL_DEBUG.print(F("[OK]"));
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
        return 0;
    } else {
        #ifdef SERIAL_DEBUG_ENABLED
            SERIAL_DEBUG.print(F("[FAIL]"));
            SERIAL_DEBUG.flush();
        #endif
        return 1;        
    }    
}

uint8_t getDHTTemperature() {
    sensors_event_t event;
    dht.temperature().getEvent(&event);
    if (isnan(event.temperature) || (event.temperature < -40) || (event.temperature > 80)) {
        #ifdef SERIAL_DEBUG_ENABLED
            SERIAL_DEBUG.print(F("\nError reading air temperature!"));
            SERIAL_DEBUG.flush();
        #endif
        return 1;
    }
    else {
        sensorsData.airTemp += event.temperature;
        sensorsData.airTempCount++;
        #ifdef SERIAL_DEBUG_ENABLED
            SERIAL_DEBUG.print(F("\nAir temperature (in oC): "));
            SERIAL_DEBUG.print(event.temperature);           
            SERIAL_DEBUG.flush();
        #endif
        return 0;
    }
}

uint8_t getDHTHumidity() {
    sensors_event_t event;
    dht.humidity().getEvent(&event);
    if (isnan(event.relative_humidity) || (event.relative_humidity < 0) || (event.relative_humidity > 100)) {
        #ifdef SERIAL_DEBUG_ENABLED
            SERIAL_DEBUG.print(F("\nError reading air humidity!"));
            SERIAL_DEBUG.flush();
        #endif
        return 1;
    }
    else {
        sensorsData.airHumid += event.relative_humidity;
        sensorsData.airHumidCount++;
        #ifdef SERIAL_DEBUG_ENABLED
            SERIAL_DEBUG.print(F("\nAir humidity (in %): "));
            SERIAL_DEBUG.print(event.relative_humidity);            
            SERIAL_DEBUG.flush();
        #endif
        return 0;
    }
}
#endif

#ifdef I2C1_ENABLED
uint8_t initI2C() {    
    #ifdef SERIAL_DEBUG_ENABLED
        SERIAL_DEBUG.print(F("\n\tInitiating I2C1 interface as master... "));
        SERIAL_DEBUG.flush();
    #endif
    Wire.begin();
    #ifdef SERIAL_DEBUG_ENABLED
        SERIAL_DEBUG.print(F("[OK]"));
        SERIAL_DEBUG.flush();
    #endif    
    return 0;
}

#ifdef SENSOR_LIGHT_ENABLED
uint8_t initSensorLight() {    
    #ifdef SERIAL_DEBUG_ENABLED
        SERIAL_DEBUG.print(F("\n\t\tInitiating light sensor... "));
        SERIAL_DEBUG.flush();
    #endif
    if (lightSensor.begin(BH1750::CONTINUOUS_HIGH_RES_MODE_2)) {
        #ifdef SERIAL_DEBUG_ENABLED
          SERIAL_DEBUG.print(F("[OK]"));
          SERIAL_DEBUG.flush();
        #endif
        return 0;
    } else {        
        #ifdef SERIAL_DEBUG_ENABLED
          SERIAL_DEBUG.print(F("[FAIL]"));
          SERIAL_DEBUG.flush();
        #endif
        return 1;
    }    
}

uint8_t getLightSensorValue() {    
    float lux = lightSensor.readLightLevel();
    if (isnan(lux) || (lux < 1) || (lux > 65535)) {        
        #ifdef SERIAL_DEBUG_ENABLED
            SERIAL_DEBUG.print(F("\nError reading light sensor!"));
            SERIAL_DEBUG.flush();        
        #endif
        return 1;
    } else {
        sensorsData.light += (uint32_t)lux;
        sensorsData.lightCount++;
        #ifdef SERIAL_DEBUG_ENABLED
            SERIAL_DEBUG.print(F("\nLight (in Lux): ")); SERIAL_DEBUG.print(lux);
            SERIAL_DEBUG.flush();
        #endif
        return 0;
    }    
}
#endif // SENSOR_LIGHT_ENABLED

#endif // I2C1_ENABLED


#ifdef SENSOR_UV_ENABLED
uint8_t initSensorUV() {    
    #ifdef SERIAL_DEBUG_ENABLED
        SERIAL_DEBUG.print(F("\n\tInitiating UV sensor... "));
        SERIAL_DEBUG.flush();
    #endif
    //pinMode(SENSOR_UV_PIN, INPUT);
    #ifdef SERIAL_DEBUG_ENABLED
        SERIAL_DEBUG.print(F("[OK]"));
    #endif    
    return 0;
}

uint8_t getUVSensorValue() {   

    // Get analog port value and convert to voltage (in miliVolts)
    int sensorValue = analogRead(SENSOR_UV_PIN);                // Get sensor value
    int sensorVoltage = (sensorValue * (5.0 / 1023.0)) * 1000;  // Convert sensor value to miliVolts

    // Check values
    if (isnan(sensorVoltage) || (sensorVoltage < 0) || (sensorVoltage > 6500)) {        
        #ifdef SERIAL_DEBUG_ENABLED
            SERIAL_DEBUG.print(F("\nError reading UV sensor!"));
            SERIAL_DEBUG.flush();
        #endif
        return 1;
    } else {
        sensorsData.uvVoltage += (uint32_t)sensorVoltage;
        sensorsData.uvVoltageCount++;
        #ifdef SERIAL_DEBUG_ENABLED
            SERIAL_DEBUG.print(F("\nUV voltage (in milliVolts): ")); SERIAL_DEBUG.print(sensorVoltage);
            SERIAL_DEBUG.print(F(" => Index: ")); SERIAL_DEBUG.print(convertMilliVoltsToIndex(sensorVoltage));
            SERIAL_DEBUG.flush();
        #endif
        return 0;
    }    
}

uint8_t convertMilliVoltsToIndex(uint16_t milliVolts) {
    if (milliVolts < 227) {
        return 0;
    } else if (milliVolts >= 227 && milliVolts < 318) {
        return 1;
    } else if (milliVolts >= 318 && milliVolts < 408) {
        return 2;
    } else if (milliVolts >= 408 && milliVolts < 503) {
        return 3;
    } else if (milliVolts >= 503 && milliVolts < 606) {
        return 4;
    } else if (milliVolts >= 606 && milliVolts < 696) {
        return 5;
    } else if (milliVolts >= 696 && milliVolts < 795) {
        return 6;
    } else if (milliVolts >= 795 && milliVolts < 881) { 
        return 7;
    } else if (milliVolts >= 881 && milliVolts < 976) {
        return 8;
    } else if (milliVolts >= 976 && milliVolts < 1079) {
        return 9;
    } else if (milliVolts >= 1079 && milliVolts < 1170) {
        return 10;
    } else {
        return 11;
    }  
}
#endif // SENSOR_UV_ENABLED

#ifdef ONE_WIRE_ENABLED    
uint8_t initOneWire() {    
    #ifdef SERIAL_DEBUG_ENABLED
        SERIAL_DEBUG.print(F("\n\tInitiating One Wire interface... "));
        SERIAL_DEBUG.flush();
    #endif
    oneWire.begin(ONE_WIRE_PIN);
    #ifdef SERIAL_DEBUG_ENABLED
        SERIAL_DEBUG.print(F("[OK]"));
        SERIAL_DEBUG.flush();
    #endif    
    return 0;
}

#ifdef SENSOR_SOIL_TEMP_ENABLED
uint8_t initSensorSoilTemp() {    
    #ifdef SERIAL_DEBUG_ENABLED
        SERIAL_DEBUG.print(F("\n\t\tInitiating soil temperature sensor... "));
        SERIAL_DEBUG.flush();
    #endif
    soil_temp_sensor.setOneWire(&oneWire);
    soil_temp_sensor.begin();
    soil_temp_sensor.getAddress(soil_temp_sensor_addr, 0);
    if (soil_temp_sensor.isConnected(soil_temp_sensor_addr)) {
        #ifdef SERIAL_DEBUG_ENABLED
            SERIAL_DEBUG.print(F("[OK]"));
            SERIAL_DEBUG.print(F("\n\t\t\tGlobal resolution...: ")); SERIAL_DEBUG.print(soil_temp_sensor.getResolution()); SERIAL_DEBUG.print(F(" bits"));
            SERIAL_DEBUG.print(F("\n\t\t\tDevice resolution...: ")); SERIAL_DEBUG.print(soil_temp_sensor.getResolution(soil_temp_sensor_addr)); SERIAL_DEBUG.print(F(" bits"));            
            SERIAL_DEBUG.flush();
        #endif                
        return 0;
    } else {
        #ifdef SERIAL_DEBUG_ENABLED
            SERIAL_DEBUG.print(F("[FAIL]"));
            SERIAL_DEBUG.flush();
        #endif
        return 1;        
    }     
}

uint8_t getSoilTempSensorValue() {    

    // Get soil temperature
    soil_temp_sensor.requestTemperatures();
    float soil_temp = soil_temp_sensor.getTempC(soil_temp_sensor_addr);
    
    // Check values
    if (isnan(soil_temp) || (soil_temp < -55) || (soil_temp > 125)) {        
        #ifdef SERIAL_DEBUG_ENABLED
            SERIAL_DEBUG.print(F("\nError reading soil temperature sensor!"));
            SERIAL_DEBUG.flush();
        #endif
        return 1;
    } else {
        sensorsData.soilTemp += soil_temp;
        sensorsData.soilTempCount++;
        #ifdef SERIAL_DEBUG_ENABLED
            SERIAL_DEBUG.print(F("\nSoil temperature (in oC): ")); SERIAL_DEBUG.print(soil_temp);
            SERIAL_DEBUG.flush();
        #endif
        return 0;
    }    
}
#endif // SENSOR_SOIL_TEMP_ENABLED

#endif // ONE_WIRE_ENABLED


#ifdef SENSOR_SOIL_MOISTURE_ENABLED
uint8_t initSensorSoilMoisture() {    
    #ifdef SERIAL_DEBUG_ENABLED
        SERIAL_DEBUG.print(F("\n\tInitiating soil moisture sensor... "));
        SERIAL_DEBUG.flush();
    #endif
    //pinMode(SENSOR_SOIL_MOISTURE_PIN, INPUT);
    #ifdef SERIAL_DEBUG_ENABLED
      SERIAL_DEBUG.print(F("[OK]"));
      SERIAL_DEBUG.flush();
    #endif
    return 0;
}   

uint8_t getSoilMoistureSensorValue() {
    
    // Get soil moisture (ADC 10 bits)
    int soil_analog = analogRead(SENSOR_SOIL_MOISTURE_PIN);

    // Check value
    if (isnan(soil_analog) || (soil_analog < 0) || (soil_analog > 1023)) {
        #ifdef SERIAL_DEBUG_ENABLED
            SERIAL_DEBUG.print(F("\nError reading soil moisture sensor!"));
            SERIAL_DEBUG.flush();
        #endif
        return 1;
    } else {
        // Map analog value using 10 bits resolution ADC
        sensorsData.soilMoisture += map(soil_analog, 0, 1023, 100, 0);
        sensorsData.soilMoistureCount++;
        #ifdef SERIAL_DEBUG_ENABLED
            SERIAL_DEBUG.print(F("\nSoil moisture (in %): ")); SERIAL_DEBUG.print(map(soil_analog, 0, 1023, 100, 0));
            SERIAL_DEBUG.flush();
        #endif
        return 0;
    }
}
#endif // SENSOR_SOIL_MOISTURE_ENABLED

#ifdef SENSOR_LEAF_MOISTURE_ENABLED
uint8_t initSensorLeafMoisture() {    
    #ifdef SERIAL_DEBUG_ENABLED
        SERIAL_DEBUG.print(F("\n\tInitiating leaf moisture sensor... "));
        SERIAL_DEBUG.flush();
    #endif
    //pinMode(SENSOR_LEAF_MOISTURE_PIN, INPUT);
    #ifdef SERIAL_DEBUG_ENABLED
      SERIAL_DEBUG.print(F("[OK]"));
      SERIAL_DEBUG.flush();
    #endif
    return 0;
} 

uint8_t getLeafMoistureSensorValue() {    
    // Get leaf moisture
    int leaf_analog = analogRead(SENSOR_LEAF_MOISTURE_PIN);

    // Check value
    if (isnan(leaf_analog) || (leaf_analog < 0) || (leaf_analog > 1023)) {
        #ifdef SERIAL_DEBUG_ENABLED
            SERIAL_DEBUG.print(F("\nError reading leaf moisture sensor!"));
            SERIAL_DEBUG.flush();
        #endif
        return 1;
    } else {
        // Map analog value using 10 bits resolution ADC
        sensorsData.leafMoisture += map(leaf_analog, 0, 1023, 100, 0);
        sensorsData.leafMoistureCount++;
        #ifdef SERIAL_DEBUG_ENABLED
            SERIAL_DEBUG.print(F("\nLeaf moisture (in %): ")); SERIAL_DEBUG.print(map(leaf_analog, 0, 1023, 100, 0));
            SERIAL_DEBUG.flush();
        #endif
        return 0;
    }
}
#endif // SENSOR_LEAF_MOISTURE_ENABLED

#ifdef SENSOR_WIND_SOCK_ENABLED
uint8_t initSensorWindSock() {    
    #ifdef SERIAL_DEBUG_ENABLED
      SERIAL_DEBUG.print(F("\n\tInitiating wind direction sensor... "));
      SERIAL_DEBUG.flush();
    #endif
    //pinMode(WIND_SOCK_PIN, INPUT);    
    #ifdef SERIAL_DEBUG_ENABLED
      SERIAL_DEBUG.print(F("[OK]"));
      SERIAL_DEBUG.flush();
    #endif
    return 0;
}

uint8_t getWindDirectionSensorValue() {
    
    // Get analog value (ADC 10 bits)
    int windDir = analogRead(WIND_SOCK_PIN);        
    
    // Check values
    if (isnan(windDir) || (windDir < 0) || (windDir > 1023)) {
        #ifdef SERIAL_DEBUG_ENABLED
            SERIAL_DEBUG.print(F("\nError reading UV sensor!"));
            SERIAL_DEBUG.flush();
        #endif
        return 1;
    } else {
        // Convert analog value to voltage (in Volts)  
        float windDirVoltage = windDir * (5.0 / 1023.0);
        sensorsData.windDirVoltage += windDirVoltage;
        sensorsData.windDirCount++;
        #ifdef SERIAL_DEBUG_ENABLED
            SERIAL_DEBUG.print(F("\nWind direction (in degree): ")); SERIAL_DEBUG.print(convertVoltsToWindDirection(windDirVoltage));
            SERIAL_DEBUG.flush();
        #endif
        return 0;
    }
}

uint16_t convertVoltsToWindDirection(float voltage) {
    if (voltage <= 0.27) {
        return 315;
    } else if (voltage <= 0.32) {
        return 270;
    } else if (voltage <= 0.38) {
        return 225;
    } else if (voltage <= 0.45) {
        return 180;
    } else if (voltage <= 0.57) {
        return 135;
    } else if (voltage <= 0.75) {
        return 90;
    } else if (voltage <= 1.25) {
        return 45;
    } else {
        return 0;
    }
}   
#endif // SENSOR_WIND_SOCK_ENABLED

#ifdef SENSOR_POWER_SUPPLY_ENABLED
uint8_t initSensorPowerSupply() {
    #ifdef SERIAL_DEBUG_ENABLED
        SERIAL_DEBUG.print(F("\n\t\tInitiating power supply sensor... "));
        SERIAL_DEBUG.flush();
    #endif
    if (ina219.begin()) {
        #ifdef SERIAL_DEBUG_ENABLED
          SERIAL_DEBUG.print(F("[OK]"));
          SERIAL_DEBUG.flush();
        #endif
        return 0;
    } else {        
        #ifdef SERIAL_DEBUG_ENABLED
          SERIAL_DEBUG.print(F("[FAIL]"));
          SERIAL_DEBUG.flush();
        #endif
        return 1;
    }       
}

uint8_t getPowerSupplySensorValue() {
    float bus_voltage = ina219.getBusVoltage_V();

    // Check values
    if (isnan(bus_voltage) || (bus_voltage < 0) || (bus_voltage > 24)) {        
        #ifdef SERIAL_DEBUG_ENABLED
            SERIAL_DEBUG.print(F("\nError reading power supply sensor!"));
            SERIAL_DEBUG.flush();        
        #endif
        return 1;
    } else {
        sensorsData.powerSupply += bus_voltage;
        sensorsData.powerSupplyCount++;
        #ifdef SERIAL_DEBUG_ENABLED
            SERIAL_DEBUG.print(F("\nPower supply (in Volts): ")); SERIAL_DEBUG.print(bus_voltage);
            SERIAL_DEBUG.flush();
        #endif
        return 0;
    }
}
#endif // SENSOR_POWER_SUPPLY_ENABLED

#ifdef SENSOR_PRESSURE_ENABLED
uint8_t initSensorPressure() {
    #ifdef SERIAL_DEBUG_ENABLED
        SERIAL_DEBUG.print(F("\n\t\tInitiating pressure / device temperature sensor... "));
        SERIAL_DEBUG.flush();
    #endif
    if (bmp.begin(BMP085_ULTRAHIGHRES)) {
        #ifdef SERIAL_DEBUG_ENABLED
          SERIAL_DEBUG.print(F("[OK]"));
          SERIAL_DEBUG.flush();
        #endif
        return 0;
    } else {        
        #ifdef SERIAL_DEBUG_ENABLED
          SERIAL_DEBUG.print(F("[FAIL]"));
          SERIAL_DEBUG.flush();
        #endif
        return 1;
    }       
}

uint8_t getPressureSensorValue() {
    // Get pressure (in Pa) and convert to hPa
    uint32_t pressure = (uint32_t)bmp.readPressure()/100;
    
    // Check values
    if (isnan(pressure) || (pressure < 300) || (pressure > 1100)) {        
        #ifdef SERIAL_DEBUG_ENABLED
            SERIAL_DEBUG.print(F("\nError reading pressure sensor!"));
            SERIAL_DEBUG.flush();        
        #endif
        return 1;
    } else {
        sensorsData.pressure += pressure;
        sensorsData.pressureCount++;
        #ifdef SERIAL_DEBUG_ENABLED
            SERIAL_DEBUG.print(F("\nPressure (in hPa): ")); SERIAL_DEBUG.print(pressure);
            SERIAL_DEBUG.flush();
        #endif
        return 0;
    }
}

uint8_t getDeviceTempSensorValue() {
    float devTemp = bmp.readTemperature();
    if (isnan(devTemp) || (devTemp < -40) || (devTemp > 85)) {        
        #ifdef SERIAL_DEBUG_ENABLED
            SERIAL_DEBUG.println(F("Fail reading device temperature sensor..."));
            SERIAL_DEBUG.flush();        
        #endif
        return 1;
    } else {
        sensorsData.devTemp += devTemp;
        sensorsData.devTempCount++;
        #ifdef SERIAL_DEBUG_ENABLED
            SERIAL_DEBUG.print(F("\nDevice temperature (in oC): ")); SERIAL_DEBUG.print(devTemp);
            SERIAL_DEBUG.flush();
        #endif
        return 0;
    }
}

#endif // SENSOR_POWER_SUPPLY_ENABLED

#ifdef SENSOR_ANEMOMETER_ENABLED
uint8_t initSensorAnemometer() {
    #ifdef SERIAL_DEBUG_ENABLED
      SERIAL_DEBUG.print(F("\n\tInitiating anemometer sensor... "));
      SERIAL_DEBUG.flush();
    #endif
    //pinMode(ANEMOMETER_PIN, INPUT_PULLUP);
    sensorsData.anemometerTurnAround = 0;
    attachInterrupt(digitalPinToInterrupt(ANEMOMETER_PIN), anemometerTurnAroundIncrement, RISING);
    #ifdef SERIAL_DEBUG_ENABLED
      SERIAL_DEBUG.print(F("[OK]"));
      SERIAL_DEBUG.flush();
    #endif
    return 0;
}

void anemometerTurnAroundIncrement() {
    sensorsData.anemometerTurnAround++;
}

uint8_t getWindSpeedSensorValue() {    
    
    // Get instant time
    uint32_t now = millis();

    // Get anemometer turn around times and reset    
    noInterrupts();
    uint32_t turn_around = sensorsData.anemometerTurnAround;
    sensorsData.anemometerTurnAround = 0;
    interrupts();
    
    // Check values
    if (isnan(turn_around)) {
        #ifdef SERIAL_DEBUG_ENABLED
            SERIAL_DEBUG.print(F("\nError reading anemometer sensor!"));
            SERIAL_DEBUG.flush();
        #endif
        return 1;
    } else {
        // Compute sampling interval - check turn around time (~ after 50 days)
        uint32_t interval = 0;
        if (now > sensorsData.lastWindSampling) {
            interval = now - sensorsData.lastWindSampling;
        } else {        
            interval = UINT32_MAX - lastSamplingPeriod + now;
        }

        // Update last sampling
        sensorsData.lastWindSampling = now;    

        // Compute RPM (Revolution Per Minute)
        float RPM = (turn_around * 60) / (interval / 1000);
    
        // Compute wind speed (in Km/h)    
        float wind_speed = 2 * 3.6 * pi * anemometer_radius * RPM / 60;

        // Storage wind speed
        sensorsData.windSpeed += wind_speed;
        sensorsData.windSpeedCount++;
        #ifdef SERIAL_DEBUG_ENABLED
            SERIAL_DEBUG.print(F("\nWind speed (in Km/h): ")); SERIAL_DEBUG.print(wind_speed);
            SERIAL_DEBUG.print(F(" - RPM: ")); SERIAL_DEBUG.print(RPM);
            SERIAL_DEBUG.flush();
        #endif
        return 0;
    }    
}
#endif // SENSOR_ANEMOMETER_ENABLED

#ifdef SENSOR_PLUVIOMETER_ENABLED
uint8_t initSensorPluviometer() {
    #ifdef SERIAL_DEBUG_ENABLED
      SERIAL_DEBUG.print(F("\n\tInitiating pluviometer sensor... "));
      SERIAL_DEBUG.flush();
    #endif
    //pinMode(ANEMOMETER_PIN, INPUT_PULLUP);
    sensorsData.pluviometerTurnAround = 0;
    attachInterrupt(digitalPinToInterrupt(PLUVIOMETER_PIN), pluviometerTurnAroundIncrement, RISING);
    #ifdef SERIAL_DEBUG_ENABLED
      SERIAL_DEBUG.print(F("[OK]"));
      SERIAL_DEBUG.flush();
    #endif
    return 0;
}

void pluviometerTurnAroundIncrement() {
    sensorsData.pluviometerTurnAround++;
}

uint8_t getRainVolumeSensorValue() {    
       
    // Get pluviometer turn around times
    noInterrupts();
    uint16_t turn_around = sensorsData.pluviometerTurnAround;
    //sensorsData.pluviometerTurnAround = 0;
    interrupts();        

    // Check value
    if (isnan(turn_around) || (turn_around < 0) || (turn_around > 12200)) {
        #ifdef SERIAL_DEBUG_ENABLED
            SERIAL_DEBUG.print(F("\nError reading device temperature sensor!"));
            SERIAL_DEBUG.flush();        
        #endif
        return 1;
    } else {
        // Storage rain volume (25 ml / turn around)
        // sensorsData.rainVolume += (turn_around * 0.025);    
        #ifdef SERIAL_DEBUG_ENABLED
            //SERIAL_DEBUG.print(F("\nRain volume (in L): ")); SERIAL_DEBUG.print(turn_around * 0.025);             
            SERIAL_DEBUG.print(F("\nRain turnaround times: ")); SERIAL_DEBUG.print(turn_around);             
            SERIAL_DEBUG.flush();
        #endif
        return 0;    
    }
}

#endif // SENSOR_PLUVIOMETER_ENABLED

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
    #ifdef SENSOR_SOIL_TEMP_ENABLED
        sensorsData.soilTemp = 0.0f;
        sensorsData.soilTempCount = 0;
    #endif
    #ifdef SENSOR_SOIL_MOISTURE_ENABLED
        sensorsData.soilMoisture = 0;
        sensorsData.soilMoistureCount = 0;
    #endif    
    #ifdef SENSOR_LEAF_MOISTURE_ENABLED
        sensorsData.leafMoisture = 0;
        sensorsData.leafMoistureCount = 0;
    #endif
    #ifdef SENSOR_WIND_SOCK_ENABLED
        sensorsData.windDirVoltage = 0.0f;
        sensorsData.windDirCount = 0;
    #endif
    #ifdef SENSOR_ANEMOMETER_ENABLED
        sensorsData.windSpeed = 0.0f;        
        sensorsData.windSpeedCount = 0;        
    #endif
    #ifdef SENSOR_PLUVIOMETER_ENABLED
        sensorsData.rainVolume = 0;            
    #endif
    #ifdef SENSOR_POWER_SUPPLY_ENABLED
        sensorsData.powerSupply = 0.0f;
        sensorsData.powerSupplyCount = 0;       
    #endif
    #ifdef SENSOR_PRESSURE_ENABLED
        sensorsData.pressure = 0;
        sensorsData.pressureCount = 0;
        sensorsData.devTemp = 0.0f;
        sensorsData.devTempCount = 0;
    #endif
}

#endif // #ifndef __MAIN_H__