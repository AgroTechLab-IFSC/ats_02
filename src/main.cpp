#include "main.h"
#include "convert_tools.h"

void setup() {
  static uint8_t setupStatus = 0;

  // Turn on LED_BUILTIN if device model isn't a Low Energy model
  if (POWER_SUPPLY == POWER_LINE) {
    pinMode(LED_BUILTIN_PIN, OUTPUT);

    // // Initiate RGB led
    #ifdef RGB_LED_ENABLED      
      rgb_led.setup();    
      rgb_led.on(Color(255,0,255));
    #endif
  }
  
  // Check if serial debug is enabled
  #ifdef SERIAL_DEBUG_ENABLED        
    SERIAL_DEBUG.begin(SERIAL_BAUDRATE);
    while (!SERIAL_DEBUG) {;}
    printInitInfo();    
  #endif

  // Delay time (5 sec.) for systems stabilization
  delay(5000);

  // Initiate and check air temperature and humidity (DHT-22) sensor
  #ifdef SENSOR_DHT_ENABLED
    setupStatus = initSensorDHT();
    if (setupStatus != 0) {            
      // Put RGB LED in error mode
      #ifdef RGB_LED_ENABLED        
        rgb_led.on(Color(255,0,0));
      #endif
    }
  #endif

  // Initiate I2C interface
  #ifdef I2C1_ENABLED    
    setupStatus = initI2C();
    
    if (setupStatus == 0) { 
      #ifdef SENSOR_LIGHT_ENABLED    
        // Initiate light (BH-1750) sensor 
        setupStatus = initSensorLight();
      #endif

      #ifdef SENSOR_POWER_SUPPLY_ENABLED
        // Initiate power supply (INA-219) sensor
        setupStatus = initSensorPowerSupply();
      #endif 

      #ifdef SENSOR_PRESSURE_ENABLED
        // Initiate pressure (BMP-085) sensor
        setupStatus = initSensorPressure();
      #endif      
    }

    if (setupStatus != 0) {
      // Put RGB LED in error mode
      #ifdef RGB_LED_ENABLED        
        rgb_led.on(Color(255,0,0));
      #endif
    }
  #endif  
  
  // Initiate ultra violet (UVM-30A) sensor
  #ifdef SENSOR_UV_ENABLED
    if (initSensorUV() != 0) {
      // Put RGB LED in error mode
      #ifdef RGB_LED_ENABLED        
        rgb_led.on(Color(255,0,0));
      #endif
    }   
  #endif

  // Initiate One Wire interface
  #ifdef ONE_WIRE_ENABLED    
    setupStatus = initOneWire();
        
    if (setupStatus == 0) {     
      #ifdef SENSOR_SOIL_TEMP_ENABLED
        // Initiate soil temperature (DS18B20) sensor
        setupStatus = initSensorSoilTemp();
      #endif
    }

    if (setupStatus != 0) {
      // Put RGB LED in error mode
      #ifdef RGB_LED_ENABLED        
        rgb_led.on(Color(255,0,0));
      #endif
    }
  #endif    

  // Initiate soil moisture (HD-38) sensor
  #ifdef SENSOR_SOIL_MOISTURE_ENABLED
    if (initSensorSoilMoisture() != 0) {
      // Put RGB LED in error mode
      #ifdef RGB_LED_ENABLED        
        rgb_led.on(Color(255,0,0));
      #endif
    }  
  #endif  

  // Initiate leaf moisture (YL-38) sensor
  #ifdef SENSOR_LEAF_MOISTURE_ENABLED
    if (initSensorLeafMoisture() != 0) {
      // Put RGB LED in error mode
      #ifdef RGB_LED_ENABLED        
        rgb_led.on(Color(255,0,0));
      #endif
    }  
  #endif 

  // Initiate wind direction (WIND SOCK) sensor
  #ifdef SENSOR_WIND_SOCK_ENABLED
    if (initSensorWindSock() != 0) {
      // Put RGB LED in error mode
      #ifdef RGB_LED_ENABLED        
        rgb_led.on(Color(255,0,0));
      #endif
    }  
  #endif // SENSOR_WIND_SOCK_ENABLED    

  // Initiate wind speed (ANEMOMETER) sensor
  #ifdef SENSOR_ANEMOMETER_ENABLED
    if (initSensorAnemometer() != 0) {
      // Put RGB LED in error mode
      #ifdef RGB_LED_ENABLED        
        rgb_led.on(Color(255,0,0));
      #endif
    }  
  #endif // SENSOR_ANEMOMETER_ENABLED   

  // Initiate rain (PLUVIOMETER) sensor
  #ifdef SENSOR_PLUVIOMETER_ENABLED
    if (initSensorPluviometer() != 0) {
      // Put RGB LED in error mode
      #ifdef RGB_LED_ENABLED        
        rgb_led.on(Color(255,0,0));
      #endif
    }  
  #endif // SENSOR_ANEMOMETER_ENABLED   

  // Populate LoRa cofiguration struct
  loraCfg.serialLora = &SERIAL_LORA;
  loraCfg.baseband = AU920;
  loraCfg.subband = 2;
  loraCfg.op_class = A;
  loraCfg.tx_power = dBm20;
  loraCfg.uplink_dr = DR1;
  // loraCfg.chan0_freq = chan0_freq;
  loraCfg.chan0_dr = DR1;
  // loraCfg.chan1_freq = chan1_freq;
  loraCfg.chan1_dr = DR1;
  // loraCfg.rxwin2_freq = rxwin2_freq;
  loraCfg.rxwin2_dr = DR8;
  loraCfg.adr = OFF;
  loraCfg.auth_mode = LWABP;
  loraCfg.dev_eui = dev_eui;
  loraCfg.app_eui = app_eui;
  // loraCfg.repeat = repeat;
  // loraCfg.retry = retry;
  loraCfg.dev_addr = dev_addr;
  // loraCfg.app_key = app_key;
  loraCfg.apps_key = apps_key;
  loraCfg.nwks_key = nwks_key;
  #ifdef SERIAL_DEBUG_ENABLED
    loraCfg.serialDebug = &SERIAL_DEBUG;
    loraCfg.debug = true;
  #endif

  // Initiate LoRa modem
  #ifdef SERIAL_DEBUG_ENABLED
    SERIAL_DEBUG.print(F("\n\tInitiating LoRaWAN modem... "));
    SERIAL_DEBUG.flush();
  #endif
  if (lora.init(loraCfg) != LORA_STATUS_OK) {
    #ifdef SERIAL_DEBUG_ENABLED
      if (!loraCfg.debug) {
        SERIAL_DEBUG.print(F("[FAIL]"));      
        SERIAL_DEBUG.flush();
      }      
    #endif

    // Put RGB LED in error mode
    #ifdef RGB_LED_ENABLED        
      rgb_led.on(Color(255,0,0));
    #endif
    
    // Put station in ERROR mode
    delay(error_reset_period);
    setup();
  } else {
    #ifdef SERIAL_DEBUG_ENABLED
      if (!loraCfg.debug) {
        SERIAL_DEBUG.print(F("[OK]"));
        SERIAL_DEBUG.flush();
      }
    #endif
  }

  // If setup status OK, turn off RGB LED
  if (POWER_SUPPLY == POWER_LINE) {
    
    // Power off RGB LED
    if (setupStatus == 0) {
      #ifdef RGB_LED_ENABLED      
        rgb_led.off();
      #endif
    }

  lastSystemPeriod = lastSamplingPeriod = lastTxPeriod = millis();
  }  
}

void loop() {  
  // If device is power line based it will run continually
  if (POWER_SUPPLY == POWER_LINE) {
    
    // Get time
    now = millis();

    // Check if turn around time (~ after 50 days)    
    if (now < lastSystemPeriod) {      
      if ((UINT32_MAX - lastSystemPeriod + now) >= systemPeriod) {
        turnAroundSystemOK = true;
      }
    }

    // Check system period
    if (((now - lastSystemPeriod) >= systemPeriod) || turnAroundSystemOK) {

      // Update lastPeriod and turnAroundTimeOK
      lastSystemPeriod = now;
      turnAroundSystemOK = false;
      
      // Toogle LED BUILTIN
      if (digitalRead(LED_BUILTIN_PIN) == HIGH) {
        digitalWrite(LED_BUILTIN_PIN, LOW);
      } else {
        digitalWrite(LED_BUILTIN_PIN, HIGH);
      }

      // Check turn around time (~ after 50 days)
      if (now < lastSamplingPeriod) {      
        if ((UINT32_MAX - lastSamplingPeriod + now) >= samplingPeriod) {
          turnAroundSamplingOK = true;
        }
      }

      // Check sampling period
      if (((now - lastSamplingPeriod) >= samplingPeriod) || turnAroundSamplingOK) {
        
        // Update lastSampling
        lastSamplingPeriod = now;
        turnAroundSamplingOK = false;

        // Power on RGB LED in sampling mode
        #ifdef RGB_LED_ENABLED          
          rgb_led.on(Color(0,255,0));
        #endif

        // Initiage sampling process
        #ifdef SERIAL_DEBUG_ENABLED
          up_time_t time = getUpTime(now);
          SERIAL_DEBUG.print(F("\n\n==========================================================="));
          SERIAL_DEBUG.print(F("\nSampling sensor values with up time "));
          SERIAL_DEBUG.print(time.years); SERIAL_DEBUG.print(F(" years, "));
          SERIAL_DEBUG.print(time.months); SERIAL_DEBUG.print(F(" months, "));
          SERIAL_DEBUG.print(time.days); SERIAL_DEBUG.print(F(" days, "));
          SERIAL_DEBUG.print(time.hours); SERIAL_DEBUG.print(F(" hours, "));
          SERIAL_DEBUG.print(time.minutes); SERIAL_DEBUG.print(F(" minutes and "));
          SERIAL_DEBUG.print(time.seconds); SERIAL_DEBUG.print(F(" seconds"));
          SERIAL_DEBUG.flush();
        #endif

        // Get air temperature and humidity sensor values
        #ifdef SENSOR_DHT_ENABLED
          if (getDHTTemperature() != 0) {
            // Power on RGB LED in error mode
            #ifdef RGB_LED_ENABLED          
              rgb_led.on(Color(255,0,0));
            #endif
          }
          if (getDHTHumidity() != 0) {
            // Power on RGB LED in error mode
            #ifdef RGB_LED_ENABLED          
              rgb_led.on(Color(255,0,0));
            #endif
          }
        #endif

        // Get light sensor value
        #ifdef SENSOR_LIGHT_ENABLED
          if (getLightSensorValue() != 0) {
            // Power on RGB LED in error mode
            #ifdef RGB_LED_ENABLED          
              rgb_led.on(Color(255,0,0));
            #endif
          }
        #endif

        // Get UV sensor value
        #ifdef SENSOR_UV_ENABLED
          if (getUVSensorValue() != 0) {
            // Power on RGB LED in error mode
            #ifdef RGB_LED_ENABLED          
              rgb_led.on(Color(255,0,0));
            #endif
          }  
        #endif

        // Get soil temperature sensor value
        #ifdef SENSOR_SOIL_TEMP_ENABLED       
          if (getSoilTempSensorValue() != 0) {
            // Power on RGB LED in error mode
            #ifdef RGB_LED_ENABLED          
              rgb_led.on(Color(255,0,0));
            #endif
          }  
        #endif                 

        // Get soil moisture sensor value
        #ifdef SENSOR_SOIL_MOISTURE_ENABLED
          if (getSoilMoistureSensorValue() != 0) {
            // Power on RGB LED in error mode
            #ifdef RGB_LED_ENABLED          
              rgb_led.on(Color(255,0,0));
            #endif
          }           
        #endif 

        // Get leaf moisture sensor value
        #ifdef SENSOR_LEAF_MOISTURE_ENABLED
          if (getLeafMoistureSensorValue() != 0) {
            // Power on RGB LED in error mode
            #ifdef RGB_LED_ENABLED          
              rgb_led.on(Color(255,0,0));
            #endif
          }           
        #endif        
        
        // Get pressure and device temperature sensor values
        #ifdef SENSOR_PRESSURE_ENABLED
          if (getPressureSensorValue() != 0) {
            // Power on RGB LED in error mode
            #ifdef RGB_LED_ENABLED          
              rgb_led.on(Color(255,0,0));
            #endif
          }
          if (getDeviceTempSensorValue() != 0) {
            // Power on RGB LED in error mode
            #ifdef RGB_LED_ENABLED          
              rgb_led.on(Color(255,0,0));
            #endif
          }           
        #endif        

        // Get power supply sensor value
        if (getPowerSupplySensorValue() != 0) {
          // Power on RGB LED in error mode
          #ifdef RGB_LED_ENABLED          
            rgb_led.on(Color(255,0,0));
          #endif
        }                   

        // Get wind direction sensor value
        #ifdef SENSOR_WIND_SOCK_ENABLED
          if (getWindDirectionSensorValue() != 0) {
            // Power on RGB LED in error mode
            #ifdef RGB_LED_ENABLED          
              rgb_led.on(Color(255,0,0));
            #endif
          }            
        #endif

        // Get wind speed sensor value
        #ifdef SENSOR_ANEMOMETER_ENABLED
          if (getWindSpeedSensorValue() != 0) {
            // Power on RGB LED in error mode
            #ifdef RGB_LED_ENABLED          
              rgb_led.on(Color(255,0,0));
            #endif
          }            
        #endif

        // Get rain volume sensor value
        #ifdef SENSOR_PLUVIOMETER_ENABLED
          if (getRainVolumeSensorValue() != 0) {
            // Power on RGB LED in error mode
            #ifdef RGB_LED_ENABLED          
              rgb_led.on(Color(255,0,0));
            #endif
          }            
        #endif

        // Terminating sampling process
        #ifdef SERIAL_DEBUG_ENABLED
          SERIAL_DEBUG.print(F("\n===========================================================\n"));    
          SERIAL_DEBUG.flush();
        #endif

        // Power off RGB LED
        #ifdef RGB_LED_ENABLED                  
          rgb_led.off();
        #endif
      } // if (((now - lastSamplingPeriod) >= samplingPeriod) || turnAroundSamplingOK) {
      
      // Since sampling period can be large, get time again
      now = millis();

      // Check turn around time (~ after 50 days)
      if (now < lastTxPeriod) {      
        if ((UINT32_MAX - lastTxPeriod + now) >= txPeriod) {
          turnAroundTxOK = true;
        }
      }

      // Check transmission period
      if (((now - lastTxPeriod) >= txPeriod) || turnAroundTxOK) {
        
        // Update lastTxPeriod and turnAroundTxOK
        lastTxPeriod = now;
        turnAroundTxOK = false;

        // Power on RGB LED in transmission mode
        #ifdef RGB_LED_ENABLED          
          rgb_led.on(Color(0,0,255));
        #endif

        delay(500);

        // Check if serial debug is enabled
        #ifdef SERIAL_DEBUG_ENABLED
          printAverageValues();                       
        #endif

        // Create message payload (25 bytes)
        // ATS-02 payload layout:
        // 2 bytes - air temperature (float2int15)
        // 2 bytes - air umidity (float2uint16)        
        // 2 bytes - soil temperature (float2int15)
        // 2 bytes - soil moisture (float2uint16)
        // 2 bytes - leaf moisture (float2uint16)
        // 1 byte  - UV index (uint8)
        // 2 bytes - light (float2uint16)        
        // 2 bytes - wind direction voltage (float2uint16)
        // 2 bytes - wind speed (float2uint16)
        // 2 bytes - pluviometer turn around (uint16)
        // 2 bytes - pressure (float2uint16)
        // 2 byte  - device temperature (float2int15)
        // 2 bytes - power supply (float2uint16)
        payload = "";        
        payload.concat(short2hex(float2int15((sensorsData.airTemp/sensorsData.airTempCount), 2)));
        payload.concat(short2hex(float2uint16((sensorsData.airHumid/sensorsData.airHumidCount), 2)));
        payload.concat(short2hex(float2int15((sensorsData.soilTemp/sensorsData.soilTempCount), 2)));
        payload.concat(short2hex(float2uint16((sensorsData.soilMoisture/sensorsData.soilMoistureCount), 2)));
        payload.concat(short2hex(float2uint16((sensorsData.leafMoisture/sensorsData.leafMoistureCount), 2)));
        payload.concat(byte2hex(convertMilliVoltsToIndex(sensorsData.uvVoltage/sensorsData.uvVoltageCount)));              
        payload.concat(short2hex(float2uint16((sensorsData.light/sensorsData.lightCount), 0)));
        //payload.concat(short2hex(convertVoltsToWindDirection(sensorsData.windDirVoltage/sensorsData.windDirCount)));
        payload.concat(short2hex(float2uint16((sensorsData.windDirVoltage/sensorsData.windDirCount), 2)));
        payload.concat(short2hex(float2uint16((sensorsData.windSpeed/sensorsData.windSpeedCount), 2)));
        noInterrupts();
        uint16_t turn_around = sensorsData.pluviometerTurnAround;
        sensorsData.pluviometerTurnAround = 0;
        interrupts(); 
        payload.concat(short2hex(turn_around));
        payload.concat(short2hex(float2uint16((sensorsData.pressure/sensorsData.pressureCount), 0)));
        payload.concat(short2hex(float2int15((sensorsData.devTemp/sensorsData.devTempCount), 2)));
        payload.concat(short2hex(float2uint16((sensorsData.powerSupply/sensorsData.powerSupplyCount), 2)));                

        // Send data values        
        lora.sendNoAckMsgHex(1, payload);

        // Reset sensor data struct
        resetSensorDataStruct();

        // Power off RGB LED
        #ifdef RGB_LED_ENABLED                  
          rgb_led.off();
        #endif
      } // if (((now - lastTxPeriod) >= txPeriod) || turnAroundTxOK)

    } // if (((now - lastSystemPeriod) >= systemPeriod) || turnAroundSystemOK) {

  } // if (POWER_SUPPLY == POWER_LINE) {

  // If device is battery based it will run, sleep and awake
  else if (POWER_SUPPLY == BATTERY) {

  }
}