
#include "LoRa.h"

/**
 * @fn LoRa::init(LoRaConfig_t config)
 * @brief Initialize LoRa interface. 
 * @param[in] config - struct with LoRa configuration (see \ref LoRaConfig_t).
 * @retval status code - 0 if successful initialization or error code.
 */
uint8_t LoRa::init(LoRaConfig_t config) {
    uint8_t statusCode = LORA_STATUS_UNINITIALIZED;
    this->config = config;

    // Set serial interface
    statusCode = setSerialInterface();
    if (statusCode != LORA_STATUS_OK) {
        return statusCode;
    }

    // Set serial interface
    statusCode = resetLoRaModule();
    if (statusCode != LORA_STATUS_OK) {
        return statusCode;
    }

    delay(500);
    
    // Show firmware version
    if (this->config.debug) {
        this->config.serialDebug->print("\n\t\tFirmware version: ");
        this->config.serialDebug->print(getFWVersion());
        this->config.serialDebug->flush();
    }
    
    // Set LoRa base band
    statusCode = setLoRaBaseBand();
    if (statusCode != LORA_STATUS_OK) {
        return statusCode;
    }

    // Set LoRa sub band
    statusCode = setLoRaSubBand();
    if (statusCode != LORA_STATUS_OK) {
        return statusCode;
    }

    // Set LoRa class
    statusCode = setLoRaClass();
    if (statusCode != LORA_STATUS_OK) {
        return statusCode;
    }

    // Set LoRa transmission power
    statusCode = setLoRaTxPwr();
    if (statusCode != LORA_STATUS_OK) {
        return statusCode;
    }

    // Set LoRa ADR (Automatic Data Rate)
    statusCode = setLoRaADR();
    if (statusCode != LORA_STATUS_OK) {
        return statusCode;
    }

    // Set LoRa uplink datarate
    statusCode = setLoRaUpDR();
    if (statusCode != LORA_STATUS_OK) {
        return statusCode;
    }

    // Set LoRa DevEUI
    statusCode = setLoRaDevEUI();
    if (statusCode != LORA_STATUS_OK) {
        return statusCode;
    }

    // Set LoRa AppEUI
    // statusCode = setLoRaAppEUI();
    // if (statusCode != LORA_STATUS_OK) {
    //     return statusCode;
    // }

    // Set LoRa authentication mode
    statusCode = setLoRaAuthMode();
    if (statusCode == LORA_STATUS_OK) {
        
        // Set LoRa DevAddr
        statusCode = setLoRaDevAddr();

        // Set LoRa NwkSKey
        statusCode = setLoRaNwkSKey();

        // Set LoRa AppSKey
        statusCode = setLoRaAppSKey();

    } else {
        return statusCode;
    }

    return statusCode;
}

uint8_t LoRa::setSerialInterface() {

    String loraReturn = "";    
    String at_cmd = "";

    // Configure UART communication between MCU and LoRaWAN modem
    if (this->config.debug) {
        this->config.serialDebug->print("\n\t\tInitiating UART port between MCU device and LoRa modem... ");
        this->config.serialDebug->flush();
    }
    
    // Initiate LoRa UART interface
    this->config.serialLora->begin(9600);

    // Wait for serial port to connect
    while (!this->config.serialLora) {;}
    
    if (this->config.debug) {
        this->config.serialDebug->print("[OK]");
        this->config.serialDebug->flush();
    }

    // Test UART communication
    if (this->config.debug) {
        this->config.serialDebug->print("\n\t\tTesting UART communciation between MCU and LoRa modem... ");
        this->config.serialDebug->flush();
    }
    at_cmd = "AT";
    this->config.serialLora->println(at_cmd);    
    loraReturn = this->config.serialLora->readString();
    if (strcmp((char*)loraReturn.c_str(), "+AT: OK\r\n") != 0) {
        if (this->config.debug) {
            this->config.serialDebug->print("[ERROR]\n\t");
            this->config.serialDebug->print(loraReturn);
            this->config.serialDebug->flush();            
        }
        return LORA_STATUS_UART_FAIL;
    } else {
        if (this->config.debug) {
            this->config.serialDebug->print("[OK]");
            this->config.serialDebug->flush();
        }
        return LORA_STATUS_OK;    
    }
}

String LoRa::getFWVersion() {
    String loraReturn = "";    
    String at_cmd = "";

    at_cmd = "AT+VER";
    this->config.serialLora->println(at_cmd);
    loraReturn = this->config.serialLora->readString();    
    return loraReturn.substring(6, loraReturn.length() - 2);
}

/**
 * @fn resetLoRaModule()
 * @brief Reset LoRa module. 
 */ 
uint8_t LoRa::resetLoRaModule() {    
    String loraReturn = "";
    String at_cmd = "";

    if (this->config.debug) {
        this->config.serialDebug->print("\n\t\tReseting LoRa module... ");        
        this->config.serialDebug->flush();
    }

    at_cmd = "AT+RESET";    
    this->config.serialLora->println(at_cmd);
    loraReturn = this->config.serialLora->readString();
    if (this->config.debug) {        
        this->config.serialDebug->print(loraReturn.substring(0, loraReturn.length() - 2));
        this->config.serialDebug->flush();         
    }

    return LORA_STATUS_OK;
}

/**
 * @fn setLoRaBaseBand()
 * @brief Set LoRa base band. 
 */ 
uint8_t LoRa::setLoRaBaseBand() {    
    String loraReturn = "";
    String at_cmd = "";

    if (this->config.debug) {
        this->config.serialDebug->print("\n\t\tSetting LoRa base band... ");        
        this->config.serialDebug->flush();
    }

    at_cmd = "AT+DR=";
    at_cmd.concat(getLoRaBaseBandStr(this->config.baseband));
    this->config.serialLora->println(at_cmd);
    loraReturn = this->config.serialLora->readString();
    if (this->config.debug) {        
        this->config.serialDebug->print(loraReturn.substring(0, loraReturn.length() - 2));
        this->config.serialDebug->flush();         
    }

    return LORA_STATUS_OK;
}

/**
 * @fn loraBand_toString(LoRaBand_e loraBand)
 * @brief Convert LoRa band enum to String.
 * @param[in] lora_band - LoRa port used to send message.
 * @return String - LoRa band string.
 */ 
String LoRa::getLoRaBaseBandStr(LoRaBaseBand_e loraBaseBand) {
    switch (loraBaseBand) {
        case EU868:
            return "EU868";
        case US915:
            return "US915";
        case AU920:
            return "AU920";
        default:
            return "ERROR";
    }
}

/**
 * @fn setLoRaSubBand()
 * @brief Sets the sub-band. This will disable all channels not belonging to the specified sub-band.
 */ 
uint8_t LoRa::setLoRaSubBand() {    
    String loraReturn = "";
    String at_cmd = "";

    if (this->config.debug) {
        this->config.serialDebug->print("\n\t\tSetting LoRa sub band... ");        
        this->config.serialDebug->flush();
    }

    if (this->config.subband == 1) {
        for (int i = 8; i <= 71; i++) {
            at_cmd = "AT+CH=";
            at_cmd.concat(i);
            at_cmd.concat(", 0");
            this->config.serialLora->println(at_cmd);
            loraReturn = this->config.serialLora->readString();
        }        
    } else if (this->config.subband == 2) {
        for (int i = 0; i <= 7; i++) {
            at_cmd = "AT+CH=";
            at_cmd.concat(i);
            at_cmd.concat(", 0");
            this->config.serialLora->println(at_cmd);
            loraReturn = this->config.serialLora->readString();
        }
        for (int i = 16; i <= 64; i++) {
            at_cmd = "AT+CH=";
            at_cmd.concat(i);
            at_cmd.concat(", 0");
            this->config.serialLora->println(at_cmd);
            loraReturn = this->config.serialLora->readString();
        }
        for (int i = 66; i <= 71; i++) {
            at_cmd = "AT+CH=";
            at_cmd.concat(i);
            at_cmd.concat(", 0");
            this->config.serialLora->println(at_cmd);
            loraReturn = this->config.serialLora->readString();
        }
    }        

    return LORA_STATUS_OK;
}

uint8_t LoRa::setLoRaClass() {
    String loraReturn = "";
    String at_cmd = "";

    if (this->config.debug) {
        this->config.serialDebug->print("\n\t\tSetting LoRa class... ");
        this->config.serialDebug->flush();
    }
    
    at_cmd = "AT+CLASS=";
    at_cmd.concat(getLoRaClassStr(this->config.op_class));
    this->config.serialLora->println(at_cmd);
    loraReturn = this->config.serialLora->readString();
    if (this->config.debug) {        
        this->config.serialDebug->print(loraReturn.substring(0, loraReturn.length() - 2));
        this->config.serialDebug->flush();        
    }

    return LORA_STATUS_OK;
}

/**
 * @fn getLoRaClassStr()
 * @brief Convert LoRa operation class enum to String.
 * @param[in] lora_class - LoRa operation class enum.
 * @return String - LoRa class operation string.
 */ 
String LoRa::getLoRaClassStr(LoRaClass_e loraClass) {    
    switch (loraClass) {
        case A:
            return "A";
        case B:
            return "B";
        case C:
            return "C";
        default:
            return "ERROR";
    }
}

uint8_t LoRa::setLoRaTxPwr() {
    String loraReturn = "";
    String at_cmd = "";

    if (this->config.debug) {
        this->config.serialDebug->print("\n\t\tSetting LoRa transmission power (in dBm)... ");
        this->config.serialDebug->flush();
    }
    
    at_cmd = "AT+POWER=";
    at_cmd.concat(getLoRaTxPwrStr(this->config.tx_power));
    this->config.serialLora->println(at_cmd);
    loraReturn = this->config.serialLora->readString();
    if (this->config.debug) {        
        this->config.serialDebug->print(loraReturn.substring(0, loraReturn.length() - 2));
        this->config.serialDebug->flush();        
    }

    return LORA_STATUS_OK;
}

/**
 * @fn getLoRaTxPwrStr(LoRaTxPower_e loraTxPower)
 * @brief Convert LoRa transmission power enum to String.
 * @param[in] loraTxPower - LoRa transmission power enum.
 * @return String - LoRa transmission power string.
 */ 
String LoRa::getLoRaTxPwrStr(LoRaTxPower_e loraTxPower) {
    switch (loraTxPower) {
        case dBm30:
            return "30";
        case dBm28:
            return "28";
        case dBm26:
            return "26";
        case dBm24:
            return "24";
        case dBm22:
            return "22";
        case dBm20:
            return "20";
        case dBm18:
            return "18";
        case dBm16:
            return "16";
        case dBm14:
            return "14";
        case dBm12:
            return "12";
        case dBm10:
            return "10";
        default:
            return "ERROR";
    }
}

uint8_t LoRa::setLoRaUpDR() {
    String loraReturn = "";
    String at_cmd = "";
    
    if (this->config.debug) {
        this->config.serialDebug->print("\n\t\tSetting LoRa uplink datarate... ");
        this->config.serialDebug->flush();
    }
    
    at_cmd = "AT+DR=";
    at_cmd.concat(getLoRaUpDRStr(this->config.uplink_dr));
    this->config.serialLora->println(at_cmd);
    loraReturn = this->config.serialLora->readString();
    if (this->config.debug) {     
        this->config.serialDebug->print(loraReturn.substring(0, loraReturn.indexOf("\n") - 1));
        this->config.serialDebug->print(" | ");
        this->config.serialDebug->print(loraReturn.substring(loraReturn.indexOf("\n") + 2, loraReturn.length() - 2));
        this->config.serialDebug->flush();         
    }

    return LORA_STATUS_OK;
}

/**
 * @fn getLoRaUpDRStr(LoRaDR_e loraDR)
 * @brief Convert LoRa datarate enum to String.
 * @param[in] loraDR - LoRa datarate enum.
 * @return String - LoRa datarate string.
 */ 
String LoRa::getLoRaUpDRStr(LoRaDR_e loraDR) {
    switch (loraDR) {
        case DR0:
            return "DR0";
        case DR1:
            return "DR1";
        case DR2:
            return "DR2";
        case DR3:
            return "DR3";
        case DR4:
            return "DR4";
        case DR5:
            return "DR5";
        case DR6:
            return "DR6";
        case DR7:
            return "DR7";
        case DR8:
            return "DR8";
        case DR9:
            return "DR9";
        case DR10:
            return "DR10";
        case DR11:
            return "DR11";
        case DR12:
            return "DR12";
        case DR13:
            return "DR13";
        case DR14:
            return "DR14";
        case DR15:
            return "DR15";
        default:
            return "ERROR";
    }
}

uint8_t LoRa::setLoRaADR() {
    String loraReturn = "";
    String at_cmd = "";

    if (this->config.debug) {
        this->config.serialDebug->print("\n\t\tSetting LoRa ADR... ");
        Serial.flush();
    }
    
    at_cmd = "AT+ADR=";
    at_cmd.concat(getLoRaBoolStr(this->config.adr));
    this->config.serialLora->println(at_cmd);
    loraReturn = this->config.serialLora->readString();
    if (this->config.debug) {        
        this->config.serialDebug->print(loraReturn.substring(0, loraReturn.length() - 2));
        this->config.serialDebug->flush();        
    }

    return LORA_STATUS_OK;
}

/**
 * @fn getLoRaBoolStr(LoRaBool_e loraBool)
 * @brief Convert LoRa boolean enum to String.
 * @param[in] loraBool - LoRa boolean enum.
 * @return String - LoRa boolean string.
 */ 
String LoRa::getLoRaBoolStr(LoRaBool_e loraBool) {
    switch (loraBool) {
        case ON:
            return "ON";
        case OFF:
            return "OFF";
        default:
            return "ERROR";
    }
}

uint8_t LoRa::setLoRaAuthMode() {
    String loraReturn = "";
    String at_cmd = "";

    if (this->config.debug) {
        this->config.serialDebug->print("\n\t\tSetting LoRa authentication mode... ");
        this->config.serialDebug->flush();
    }
    
    at_cmd = "AT+MODE=";
    at_cmd.concat(getLoRaAuthModeStr(this->config.auth_mode));
    this->config.serialLora->println(at_cmd);
    loraReturn = this->config.serialLora->readString();
    if (this->config.debug) {        
        this->config.serialDebug->print(loraReturn.substring(0, loraReturn.length() - 2));
        this->config.serialDebug->flush();
    }

    return LORA_STATUS_OK;
}


/**
 * @fn getLoRaAuthModeStr(LoRaAuthMode_e loraAuthMode)
 * @brief Convert LoRa authentication mode enum to String.
 * @param[in] loraAuthMode - LoRa authentication mode enum.
 * @return String - LoRa authentication mode string.
 */ 
String LoRa::getLoRaAuthModeStr(LoRaAuthMode_e loraAuthMode) {
    switch (loraAuthMode) {
        case LWABP:
            return "LWABP";
        case LWOTAA:
            return "LWOTAA";
        case LWTEST:
            return "LWTEST";
        default:
            return "ERROR";
    }
}

uint8_t LoRa::setLoRaDevEUI() {
    String loraReturn = "";
    String at_cmd = "";

    if (this->config.debug) {
        this->config.serialDebug->print("\n\t\tSetting LoRa DevEUI... ");
        this->config.serialDebug->flush();
    }
    
    at_cmd = "AT+ID=DevEui,\"";
    at_cmd.concat(this->config.dev_eui);
    at_cmd.concat("\"");
    this->config.serialLora->println(at_cmd);
    loraReturn = this->config.serialLora->readString();
    if (this->config.debug) {        
        this->config.serialDebug->print(loraReturn.substring(0, loraReturn.length() - 2));
        this->config.serialDebug->flush();        
    }

    return LORA_STATUS_OK;
}

uint8_t LoRa::setLoRaAppEUI() {
    String loraReturn = "";
    String at_cmd = "";
    
    if (this->config.debug) {
        this->config.serialDebug->print("\n\t\tSetting LoRa AppEUI... ");
        this->config.serialDebug->flush();
    }
    
    at_cmd = "AT+ID=AppEui,\"";
    at_cmd.concat(this->config.app_eui);
    at_cmd.concat("\"");
    this->config.serialLora->println(at_cmd);
    loraReturn = this->config.serialLora->readString();
    if (this->config.debug) {        
        this->config.serialDebug->print(loraReturn.substring(0, loraReturn.length() - 2));
        this->config.serialDebug->flush();        
    }

    return LORA_STATUS_OK;
}

uint8_t LoRa::setLoRaDevAddr() {
    String loraReturn = "";
    String at_cmd = "";
     
    if (this->config.debug) {
        this->config.serialDebug->print("\n\t\tSetting LoRa device address... ");
        this->config.serialDebug->flush();
    }
    
    at_cmd = "AT+ID=DevAddr,\"";
    at_cmd.concat(this->config.dev_addr);
    at_cmd.concat("\"");
    this->config.serialLora->println(at_cmd);
    loraReturn = this->config.serialLora->readString();
    if (this->config.debug) {    
        this->config.serialDebug->print(loraReturn.substring(0, loraReturn.length() - 2));
        this->config.serialDebug->flush();        
    }

    return LORA_STATUS_OK;
}

uint8_t LoRa::setLoRaNwkSKey() {
    String loraReturn = "";
    String at_cmd = "";

    if (this->config.debug) {
        this->config.serialDebug->print("\n\t\tSetting LoRa network session key... ");
        this->config.serialDebug->flush();
    }
    
    at_cmd = "AT+KEY=NwkSKey,\"";
    at_cmd.concat(this->config.nwks_key);
    at_cmd.concat("\"");
    this->config.serialLora->println(at_cmd);
    loraReturn = this->config.serialLora->readString();
    if (this->config.debug) {
        this->config.serialDebug->print(loraReturn.substring(0, loraReturn.length() - 2));
        this->config.serialDebug->flush();        
    }

    return LORA_STATUS_OK;
}

uint8_t LoRa::setLoRaAppSKey() {
    String loraReturn = "";
    String at_cmd = "";

    if (this->config.debug) {
        this->config.serialDebug->print("\n\t\tSetting LoRa application session key... ");
        this->config.serialDebug->flush();
    }
    
    at_cmd = "AT+KEY=AppSKey,\"";
    at_cmd.concat(this->config.apps_key);
    at_cmd.concat("\"");
    this->config.serialLora->println(at_cmd);
    loraReturn = this->config.serialLora->readString();
    if (this->config.debug) {    
        this->config.serialDebug->print(loraReturn.substring(0, loraReturn.length() - 2));
        this->config.serialDebug->flush();        
    }

    return LORA_STATUS_OK;
}
    
 /**
 * @fn sendNoAckMsgHex(uint8_t port, char* msg_ptr, uint8_t msg_size)
 * @brief Send unconfirmed messages in a hexadecimal format.
 * @param[in] port - LoRa port used to send message.
 * @param[in] msg_ptr - pointer to message.
 * @param[in] msg_size - message size.
 * @retval true - successful transmission.
 * @retval false - transmission fail.
 */ 
uint8_t LoRa::sendNoAckMsgHex(uint8_t port, String buf) {    
    String loraReturn = "";
    String at_cmd = "";

    // Set LoRa modem as busy
    this->loraBusy = true;

    // Set LoRa port
    if (this->config.debug) {
        Serial.print("\nSending LoRa noACK hexadecimal message...");
        Serial.print("\n\tSetting LoRa port... ");       
        Serial.flush();
    }
    
    at_cmd = "AT+PORT=";
    at_cmd.concat(toAscii(port));
    this->config.serialLora->println(at_cmd);
    loraReturn = this->config.serialLora->readString();
    if (this->config.debug) {        
        this->config.serialDebug->print(loraReturn.substring(0, loraReturn.length() - 2));
        this->config.serialDebug->flush();     
    }

    // Send LoRa message
    if (this->config.debug) {
        this->config.serialDebug->print("\nSending message... ");
        this->config.serialDebug->flush();
    }
    loraReturn = "";
    at_cmd = "AT+MSGHEX=\"";
    at_cmd.concat(buf);
    at_cmd.concat("\"");    
    this->config.serialLora->println(at_cmd);    
    do {
        while (this->config.serialLora->available()) {
            loraReturn = this->config.serialLora->readString();
            if (this->config.debug) {
                this->config.serialDebug->print("\n");
                this->config.serialDebug->print(loraReturn);
                this->config.serialDebug->flush();        
            }                        
            if (loraReturn.indexOf("Done") != -1) {                
                this->loraBusy = false;
            }
        }  
    } while (this->loraBusy);

    return LORA_STATUS_OK;
}