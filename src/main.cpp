#include "main.h"

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
    SERIAL_DEBUG.flush();
  #endif

  // Delay time (5 sec.) for systems stabilization
  delay(5000);

  // Initiate and check DHT sensor
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
    #ifdef SERIAL_DEBUG_ENABLED
        SERIAL_DEBUG.print(F("\n\tInitiating I2C1 interface as master... "));
    #endif
    Wire.begin();
    #ifdef SERIAL_DEBUG_ENABLED
        SERIAL_DEBUG.print(F("[OK]"));
    #endif

    #ifdef SENSOR_LIGHT_ENABLED
      #ifdef SERIAL_DEBUG_ENABLED
        SERIAL_DEBUG.print(F("\n\t\tInitiating light sensor... "));
      #endif
      if (lightSensor.begin(BH1750::CONTINUOUS_HIGH_RES_MODE_2)) {
        #ifdef SERIAL_DEBUG_ENABLED
          SERIAL_DEBUG.print(F("[OK]"));
        #endif
      }
      else {
        setupStatus = 1;
        #ifdef RGB_LED_ENABLED        
          rgb_led.on(Color(255,0,0));
        #endif
        #ifdef SERIAL_DEBUG_ENABLED
          SERIAL_DEBUG.print(F("[FAIL]"));
        #endif
      }
    #endif
  #endif

  #ifdef SENSOR_UV_ENABLED
    #ifdef SERIAL_DEBUG_ENABLED
      SERIAL_DEBUG.print(F("\n\tInitiating UV sensor... "));
    #endif
    pinMode(SENSOR_UV_PIN, INPUT);
    #ifdef SERIAL_DEBUG_ENABLED
      SERIAL_DEBUG.print(F("[OK]"));
    #endif
  #endif

  // If setup status OK, turn off RGB LED
  if (POWER_SUPPLY == POWER_LINE) {
    
    // Power off RGB LED
    if (setupStatus == 0) {
      #ifdef RGB_LED_ENABLED      
        rgb_led.off();
      #endif
    }

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

        // Get DHT sensor values
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
          uint16_t lux = lightSensor.readLightLevel();
          if (isnan(lux)) {
            #ifdef SERIAL_DEBUG_ENABLED
              SERIAL_DEBUG.println(F("Fail reading light sensor..."));
            #endif
          } else {
            sensorsData.light += lux;
            sensorsData.lightCount++;
            #ifdef SERIAL_DEBUG_ENABLED
              SERIAL_DEBUG.print(F("\nLight: ")); SERIAL_DEBUG.print(lux); SERIAL_DEBUG.print(F(" lux"));
            #endif
          }
        #endif

        // Get UV sensor value
        #ifdef SENSOR_UV_ENABLED
          // Get voltage in miliVolts
          int sensorValue = analogRead(SENSOR_UV_PIN);    // Get sensor value
          int sensorVoltage = (sensorValue * (5.0 / 1023.0)) * 1000;    // Convert sensor value to miliVolts      
          if (isnan(sensorVoltage)) {
            #ifdef SERIAL_DEBUG_ENABLED
              SERIAL_DEBUG.println(F("Fail reading UV sensor..."));
            #endif
          } else {
            sensorsData.uvVoltage += sensorVoltage;
            sensorsData.uvVoltageCount++;
            #ifdef SERIAL_DEBUG_ENABLED
              SERIAL_DEBUG.print(F("\nUV tension: ")); SERIAL_DEBUG.print(sensorVoltage); SERIAL_DEBUG.print(F(" miliVolts => Index: "));
              SERIAL_DEBUG.print(convertVoltsToIndex(sensorVoltage));
            #endif
          }
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
          SERIAL_DEBUG.print("\nAverage air temperature: "); SERIAL_DEBUG.print(sensorsData.airTemp/sensorsData.airTempCount);
          SERIAL_DEBUG.print(" oC ("); SERIAL_DEBUG.print(sensorsData.airTempCount); SERIAL_DEBUG.print(" sampling)");
          SERIAL_DEBUG.print("\nAverage air humidity: "); SERIAL_DEBUG.print(sensorsData.airHumid/sensorsData.airHumidCount);
          SERIAL_DEBUG.print(" oC ("); SERIAL_DEBUG.print(sensorsData.airHumidCount); SERIAL_DEBUG.print(" sampling)");
          SERIAL_DEBUG.print("\nAverage light: "); SERIAL_DEBUG.print(sensorsData.light/sensorsData.lightCount);
          SERIAL_DEBUG.print(" lux ("); SERIAL_DEBUG.print(sensorsData.lightCount); SERIAL_DEBUG.print(" sampling)");
          SERIAL_DEBUG.print("\nAverage UV tension: "); SERIAL_DEBUG.print(sensorsData.uvVoltage/sensorsData.uvVoltageCount);
          SERIAL_DEBUG.print(" miliVolts => Index: "); SERIAL_DEBUG.print(convertVoltsToIndex(sensorsData.uvVoltage/sensorsData.uvVoltageCount));
          SERIAL_DEBUG.print(" ("); SERIAL_DEBUG.print(sensorsData.uvVoltageCount); SERIAL_DEBUG.print(" sampling)");          
          SERIAL_DEBUG.flush();
        #endif

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