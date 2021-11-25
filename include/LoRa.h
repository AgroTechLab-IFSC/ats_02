/**
 * @file lora.h
 * @author Robson Costa (robson.costa@ifsc.edu.br)
 * @brief LoRa operation library.
 * @version alpha
 * @since 09/01/2019
 * @date 28/05/2021
 * 
 * @copyright Copyright (c) 2019 - Robson Costa\n
 * Licensed under the Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International Unported License (the <em>"License"</em>). 
 * You may not use this file except in compliance with the License. You may obtain a copy of the License at 
 * \url{https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode}. Unless required by applicable law or agreed to in writing,  
 * software distributed under the License is distributed on an <em>"as is" basis, without warranties or 
 * conditions of any kind</em>, either express or implied. See the License for the specific language governing 
 * permissions and limitations under the License.
 */
#ifndef __LORA_H__
#define __LORA_H__

#include <Arduino.h>

/**
 * @enum LoRaBaseBand_e
 * @brief LoRa base band operation.
 * @var EU868
 * European base band operation.
 * @var US915
 * America base band operation.
 * @var AU920
 * Australia (Brazilian) base band operation.
 */
enum LoRaBaseBand_e {
    EU868,
    US915,
    AU920
};

/**
 * @enum LoRaClass_e
 * @brief LoRa class.
 * @var A
 * LoRa class "A".
 * @var B
 * LoRa class "B".
 * @var C
 * LoRa class "C".
 */
enum LoRaClass_e {
    A,
    B,
    C
};

/**
 * @enum LoRaTxPower_e
 * @brief LoRa transmission power (in dBm).
 * @var A
 * LoRa class "A".
 * @var C
 * LoRa class "C".
 */
enum LoRaTxPower_e {
    dBm30,
    dBm28,
    dBm26,
    dBm24,
    dBm22,
    dBm20,
    dBm18,
    dBm16,
    dBm14,
    dBm12,
    dBm10
};

/**
 * @enum LoRaDR_e
 * @brief LoRa data rate.
 * @var DR0
 * [EU868/EU434] - LoRa SF12/125KHz.\n
 * [US915/AU920] - LoRa SF10/125KHz.
 * @var DR1
 * [EU868/EU434] - LoRa SF11/125KHz.\n
 * [US915/AU920] - LoRa SF9/125KHz.
 * @var DR2
 * [EU868/EU434] - LoRa SF10/125KHz.\n
 * [US915/AU920] - LoRa SF8/125KHz.
 * @var DR3
 * [EU868/EU434] - LoRa SF9/125KHz.\n
 * [US915/AU920] - LoRa SF7/125KHz.
 * @var DR4
 * [EU868/EU434] - LoRa SF8/125KHz.\n
 * [US915/AU920] - LoRa SF8/500KHz.
 * @var DR5
 * [EU868/EU434] - LoRa SF7/125KHz.\n
 * [US915/AU920] - RFU.
 * @var DR6
 * [EU868/EU434] - LoRa SF7/250KHz.\n
 * [US915/AU920] - RFU.
 * @var DR7
 * [EU868/EU434] - FSK 50Kbps.\n
 * [US915/AU920] - RFU.
 * @var DR8
 * [EU868/EU434] - RFU.\n
 * [US915/AU920] - LoRa SF12/500KHz.
 * @var DR9
 * [EU868/EU434] - RFU.\n
 * [US915/AU920] - LoRa SF11/500KHz.
 * @var DR10
 * [EU868/EU434] - RFU.\n
 * [US915/AU920] - LoRa SF10/500KHz.
 * @var DR11
 * [EU868/EU434] - RFU.\n
 * [US915/AU920] - LoRa SF9/500KHz.
 * @var DR12
 * [EU868/EU434] - RFU.\n
 * [US915/AU920] - LoRa SF8/500KHz.
 * @var DR13
 * [EU868/EU434] - RFU.\n
 * [US915/AU920] - LoRa SF7/500KHz.
 * @var DR14
 * [EU868/EU434] - RFU.\n
 * [US915/AU920] - RFU.
 * @var DR15
 * [EU868/EU434] - RFU.\n
 * [US915/AU920] - RFU.
 */
enum LoRaDR_e {
    DR0,
    DR1,
    DR2,
    DR3,
    DR4,
    DR5,
    DR6,
    DR7,
    DR8,
    DR9,
    DR10,
    DR11,
    DR12,
    DR13,
    DR14,
    DR15
};

/**
 * @enum LoRaBool_e
 * @brief LoRa boolean.
 * @var ON
 * parameter is ON.
 * @var OFF
 * parameter is OFF.
 */
enum LoRaBool_e {
    ON,
    OFF
};

/**
 * @enum LoRaAuthMode_e
 * @brief LoRa authentication mode.
 * @var LWABP
 * Authentication by Personalisation.
 * @var LWOTAA
 * Over the Air Authentication.
 * @var LWTEST
 * Test mode.
 */
enum LoRaAuthMode_e {
    LWABP,
    LWOTAA,
    LWTEST
};

/**
 * @enum LoRaStatusCode_e
 * @brief LoRa status code.
 */
enum LoRaStatusCode_e {
    LORA_STATUS_OK,
    LORA_STATUS_UART_FAIL,
    LORA_STATUS_UNINITIALIZED    
};

/**
 * @struct LoRaConfig_t
 * @brief LoRa configuration struct.
 */
struct LoRaConfig_t {
    LoRaBaseBand_e baseband;    /**< LoRa base band. */
    uint8_t subband;            /**< LoRa sub band. */
    LoRaClass_e op_class;       /**< LoRa class. */
    LoRaTxPower_e tx_power;     /**< LoRa transmission power. */
    LoRaDR_e uplink_dr;         /**< LoRa uplink datarate. */
    LoRaBool_e adr;             /**< LoRa ADR (Automatic Data Rate). */
    LoRaAuthMode_e auth_mode;   /**< LoRa authentication mode. */
    String dev_eui;             /**< LoRa DevEUI. */
    String app_eui;             /**< LoRa AppEUI. */
    String repeat;              /**< LoRa unconfirmed message repeat time. */
    String retry;               /**< LoRa confirmed message retry times. */
    String dev_addr;            /**< LoRa device address. */
    String app_key;             /**< LoRa application key. */
    String apps_key;            /**< LoRa application session key. */
    String nwks_key;            /**< LoRa network session key. */
    String rxwin2_freq;         /**< LoRa receive window 2 frequency. */
    LoRaDR_e rxwin2_dr;         /**< LoRa receive window 2 datarate. */
    String chan0_freq;          /**< LoRa channel 0 frequency. */
    LoRaDR_e chan0_dr;          /**< LoRa channel 0 datarate. */
    String chan1_freq;          /**< LoRa channel 1 frequency. */
    LoRaDR_e chan1_dr;          /**< LoRa channel 1 datarate. */
    bool debug;                 /**< Enable/disable LoRa debug. */
    HardwareSerial* serialDebug; /**< Serial used to debug. */
    HardwareSerial* serialLora;  /**< Serial used to LoRaWAN modem. */
};

class LoRa {
    private:
        LoRaConfig_t config;
        bool loraBusy = false;
        uint8_t setSerialInterface();                               
        uint8_t setLoRaBaseBand();
        String getLoRaBaseBandStr(LoRaBaseBand_e loraBaseBand);
        uint8_t setLoRaSubBand();
        uint8_t setLoRaClass();
        String getLoRaClassStr(LoRaClass_e loraClass);
        uint8_t setLoRaTxPwr();
        String getLoRaTxPwrStr(LoRaTxPower_e loraTxPower);
        uint8_t setLoRaUpDR();
        String getLoRaUpDRStr(LoRaDR_e loraDR);
        uint8_t setLoRaADR();
        String getLoRaBoolStr(LoRaBool_e loraBool);
        uint8_t setLoRaAuthMode();
        String getLoRaAuthModeStr(LoRaAuthMode_e loraAuthMode);
        uint8_t setLoRaDevEUI();
        uint8_t setLoRaAppEUI();
        uint8_t setLoRaDevAddr();
        uint8_t setLoRaNwkSKey();
        uint8_t setLoRaAppSKey();
        uint8_t resetLoRaModule();

    public:              
        uint8_t init(LoRaConfig_t config);
        String getFWVersion(); 
        // bool sendNoAckMsg(LoRaConfig_t loraCfg, uint8_t port, String buf);
        // bool sendAckMsg(LoRaConfig_t loraCfg, uint8_t port, String buf);
        uint8_t sendNoAckMsgHex(uint8_t port, String buf);
        // bool sendAckMsgHex(LoRaConfig_t loraCfg, uint8_t port, String buf);
        // void callback_RX();
};

    

    // // Set LoRa channel 0 configuration
    // if (loraConfig.debug) {
    //     Serial.print("\nSetting LoRa channel 0 configuration... ");
    //     Serial.flush();
    // }
    // loraReturn = "";
    // at_cmd = "AT+CH=0,";
    // at_cmd.concat(loraConfig.chan0_freq);
    // at_cmd.concat(",");
    // at_cmd.concat(loraDR_toString(loraConfig.chan0_dr));
    // Serial1.println(at_cmd);
    // loraReturn = Serial1.readString();
    // if (loraConfig.debug) {
    //     Serial.print("\n\t");
    //     Serial.print(loraReturn);
    //     Serial.flush();        
    // }

    // // Set LoRa channel 1 configuration
    // if (loraConfig.debug) {
    //     Serial.print("\nSetting LoRa channel 1 configuration... ");
    //     Serial.flush();
    // }
    // loraReturn = "";
    // at_cmd = "AT+CH=1,";
    // at_cmd.concat(loraConfig.chan1_freq);
    // at_cmd.concat(",");
    // at_cmd.concat(loraDR_toString(loraConfig.chan1_dr));
    // Serial1.println(at_cmd);
    // loraReturn = Serial1.readString();
    // if (loraConfig.debug) {
    //     Serial.print("\n\t");
    //     Serial.print(loraReturn);
    //     Serial.flush();        
    // }

    // // Set LoRa RX window 2
    // if (loraConfig.debug) {
    //     Serial.print("\nSetting LoRa RX window 2... ");
    //     Serial.flush();
    // }
    // loraReturn = "";
    // at_cmd = "AT+RXWIN2=";
    // at_cmd.concat(loraConfig.rxwin2_freq);
    // at_cmd.concat(",");
    // at_cmd.concat(loraDR_toString(loraConfig.rxwin2_dr));
    // Serial1.println(at_cmd);
    // loraReturn = Serial1.readString();
    // if (loraConfig.debug) {
    //     Serial.print("\n\t");
    //     Serial.print(loraReturn);
    //     Serial.flush();        
    // }

    

    

    

    // // // Set LoRa unconfirmed message repeats time
    // // if (loraConfig.debug) {
    // //     Serial.print("\nSetting LoRa unconfirmed message repeats time... ");
    // //     Serial.flush();
    // // }
    // // loraReturn = "";
    // // at_cmd = "AT+REPT=";
    // // at_cmd.concat(loraConfig.repeat);
    // // Serial1.println(at_cmd);
    // // loraReturn = Serial1.readString();
    // // if (loraConfig.debug) {
    // //     Serial.print("\n\t");
    // //     Serial.print(loraReturn);
    // //     Serial.flush();        
    // // }

    // // Set LoRa confirmed message retry times
    // if (loraConfig.debug) {
    //     Serial.print("\nSetting LoRa confirmed message retry times... ");
    //     Serial.flush();
    // }
    // loraReturn = "";
    // at_cmd = "AT+RETRY=";
    // at_cmd.concat(loraConfig.retry);
    // Serial1.println(at_cmd);
    // loraReturn = Serial1.readString();
    // if (loraConfig.debug) {
    //     Serial.print("\n\t");
    //     Serial.print(loraReturn);
    //     Serial.flush();        
    // }

    

    // // Set another LoRa parameters based on authentication mode
    // // Authentication LWABP
    // if (loraAuthMode_toString(loraConfig.auth_mode).compareTo("LWABP") == 0) {


    // } else {
    //     // Authentication OTAA
    //     // Set LoRa application key
    //     if (loraConfig.debug) {
    //         Serial.print("\nSetting LoRa application key... ");
    //         Serial.flush();
    //     }
    //     loraReturn = "";
    //     at_cmd = "AT+KEY=AppKey,\"";
    //     at_cmd.concat(loraConfig.app_key);
    //     at_cmd.concat("\"");
    //     Serial1.println(at_cmd);
    //     loraReturn = Serial1.readString();
    //     if (loraConfig.debug) {
    //         Serial.print("\n\t");
    //         Serial.print(loraReturn);
    //         Serial.flush();        
    //     }

    //     // Join to the LoRa network
    //     if (loraConfig.debug) {
    //         Serial.print("\nJoining... ");
    //         Serial.flush();
    //     }
    //     loraReturn = "";
    //     at_cmd = "AT+Join";
    //     loraBusy = true;
    //     Serial1.println(at_cmd);
    //     loraReturn = Serial1.readString();
    //     do {
    //         while (Serial1.available()) {
    //             loraReturn = Serial1.readString();
    //             if (loraConfig.debug) {
    //                 Serial.print("\n");
    //                 Serial.print(loraReturn);
    //                 Serial.flush();        
    //             }
    //             char str[30];
    //             loraReturn.toCharArray(str, sizeof(str), 0);
    //             if (strstr(str, "Done")) {
    //                 loraBusy = false;
    //             }
    //         }  
    //     } while (loraBusy);

    // }

    // Return success initialization
//     return true;
// }
















// /**
//  * @fn sendNoAckMsg(uint8_t port, char* msg_ptr, uint8_t msg_size)
//  * @brief Send unconfirmed messages in a string format.
//  * @param[in] port - LoRa port used to send message.
//  * @param[in] msg_ptr - pointer to message.
//  * @param[in] msg_size - message size.
//  * @retval true - successful transmission.
//  * @retval false - transmission fail.
//  */ 
// bool LoRa::sendNoAckMsg(LoRaConfig_t loraCfg, uint8_t port, String buf) {

//     String at_cmd = "";

//     // Set LoRa port
//     if (loraCfg.debug) {
//         Serial.print("\nSetting LoRa port... ");
//         Serial.flush();
//     }
//     loraReturn = "";
//     at_cmd = "AT+PORT=";
//     at_cmd.concat(toAscii(port));
//     Serial1.println(at_cmd);
//     loraReturn = Serial1.readString();
//     if (loraCfg.debug) {
//         Serial.print("\n\t");
//         Serial.print(loraReturn);
//         Serial.flush();        
//     }

//     // Send LoRa message
//     if (loraCfg.debug) {
//         Serial.print("\nSending unconfirmed LoRa string message... ");
//         Serial.flush();
//     }
//     loraReturn = "";
//     at_cmd = "AT+MSG=\"";
//     at_cmd.concat(buf);
//     at_cmd.concat("\"");
//     Serial1.println(at_cmd);
//     loraReturn = Serial1.readString();
//     if (loraCfg.debug) {
//         Serial.print("\n\t");
//         Serial.print(loraReturn);
//         Serial.flush();        
//     }

//     return true;
// }

// /**
//  * @fn sendAckMsg(uint8_t port, char* msg_ptr, uint8_t msg_size)
//  * @brief Send confirmed messages in a string format.
//  * @param[in] port - LoRa port used to send message.
//  * @param[in] msg_ptr - pointer to message.
//  * @param[in] msg_size - message size.
//  * @retval true - successful transmission.
//  * @retval false - transmission fail.
//  */ 
// bool LoRa::sendAckMsg(LoRaConfig_t loraCfg, uint8_t port, String buf) {

//     String at_cmd = "";

//     // Set LoRa port
//     if (loraCfg.debug) {
//         Serial.print("\nSetting LoRa port... ");
//         Serial.flush();
//     }
//     loraReturn = "";
//     at_cmd = "AT+PORT=";
//     at_cmd.concat(toAscii(port));
//     Serial1.println(at_cmd);
//     loraReturn = Serial1.readString();
//     if (loraCfg.debug) {
//         Serial.print("\n\t");
//         Serial.print(loraReturn);
//         Serial.flush();        
//     }

//     // Send LoRa message
//     if (loraCfg.debug) {
//         Serial.print("\nSending confirmed LoRa string message... ");
//         Serial.flush();
//     }
//     loraReturn = "";
//     at_cmd = "AT+CMSG=\"";
//     at_cmd.concat(buf);
//     at_cmd.concat("\"");
//     Serial1.println(at_cmd);
//     loraReturn = Serial1.readString();
//     if (loraCfg.debug) {
//         Serial.print("\n");
//         Serial.print(loraReturn);
//         Serial.flush();        
//     }

//     return true;
// }



// /**
//  * @fn sendAckMsgHex(uint8_t port, char* msg_ptr, uint8_t msg_size)
//  * @brief Send confirmed messages in a hexadecimal format.
//  * @param[in] port - LoRa port used to send message.
//  * @param[in] msg_ptr - pointer to message.
//  * @param[in] msg_size - message size.
//  * @retval true - successful transmission.
//  * @retval false - transmission fail.
//  */ 
// bool LoRa::sendAckMsgHex(LoRaConfig_t loraCfg, uint8_t port, String buf) {

//     String at_cmd = "";

//     // Set LoRa modem as busy
//     loraBusy = true;

//     // Set LoRa port
//     if (loraCfg.debug) {
//         Serial.print("\nSetting LoRa port... ");
//         Serial.flush();
//     }
//     loraReturn = "";
//     at_cmd = "AT+PORT=";
//     at_cmd.concat(toAscii(port));
//     Serial1.println(at_cmd);
//     loraReturn = Serial1.readString();
//     if (loraCfg.debug) {
//         Serial.print("\n\t");
//         Serial.print(loraReturn);
//         Serial.flush();        
//     }

//     // Send LoRa message
//     if (loraCfg.debug) {
//         Serial.print("\nSending confirmed LoRa hexadecimal message... ");
//         Serial.flush();
//     }
//     loraReturn = "";
//     at_cmd = "AT+CMSGHEX=\"";
//     at_cmd.concat(buf);
//     at_cmd.concat("\"");
//     Serial1.println(at_cmd);
//     do {
//         while (Serial1.available()) {
//             loraReturn = Serial1.readString();
//             if (loraCfg.debug) {
//                 Serial.print("\n");
//                 Serial.print(loraReturn);
//                 Serial.flush();        
//             }
//             char str[1024];
//             loraReturn.toCharArray(str, sizeof(str), 0);
//             if (strstr(str, "Done")) {
//                 loraBusy = false;
//             }
//         }  
//     } while (loraBusy);

//     return true;
// }

// /**
//  * @fn loraSerialRx_callback()
//  * @brief Callback to process data from LoRa modem.
//  */
// void LoRa::callback_RX() {
//     // while (Serial1.available()) {    
//     //     loraReturn = Serial1.readString();
//     //     // if (loraConfig.debug) {
//     //         Serial.print("\n");
//     //         Serial.print(loraReturn);
//     //         Serial.flush();
//     //     // }
//     // }
// }

#endif // __LORA_H_