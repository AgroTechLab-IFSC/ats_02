/**
 * @file convert_tools.h
 * @author Robson Costa (robson.costa@ifsc.edu.br)
 * @brief Conversion library.
 * @version 0.1.0
 * @since 2021-09-21 
 * @date 2021-09-21
 * 
 * @copyright Copyright (c) 2021 - Robson Costa\n
 * Licensed under the Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International Unported License (the <em>"License"</em>). 
 * You may not use this file except in compliance with the License. You may obtain a copy of the License at 
 * \url{https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode}. Unless required by applicable law or agreed to in writing,  
 * software distributed under the License is distributed on an <em>"as is" basis, without warranties or 
 * conditions of any kind</em>, either express or implied. See the License for the specific language governing 
 * permissions and limitations under the License.
 */
#ifndef __CONVERT_TOOLS_H__
#define  __CONVERT_TOOLS_H__

#include <Arduino.h>

/*******************************************************
 *                FUNCTIONS PROTOTYPES
 *******************************************************/
uint16_t float2int15(float value, uint8_t decimal);
uint16_t float2uint16(float value, uint8_t decimal);
String byte2hex(uint8_t value);
String short2hex(uint16_t value);
String long2hex(uint32_t value);


/*******************************************************
 *             FUNCTIONS IMPLEMENTATIONS
 *******************************************************/

/**
 * @fn float2int15
 * @brief Convert float to INT_15.
 * @details 16th bit will be signal and from 15th to 1st bit will be number.
 *          Range: decimal = 0 (-32768 <-> 32768)
 *                 decimal = 1 (-3276.8 <-> 3276.8)
 *                 decimal = 2 (-327.68 <-> 327.68)
 * @param[in] value - float value.
 * @param[in] decimal - decimal digits.
 * @return uint16_t - Converted value.
 */ 
uint16_t float2int15(float value, uint8_t decimal) {
    uint16_t converted = 0;
    float aux = value;
    
    if (value < 0) {
        aux = aux * -1;        
    }

    if (decimal == 0) {
        converted = (uint16_t)aux;
    } else if (decimal == 1) {
        converted = (uint16_t)(aux * 10);
    } else if (decimal == 2) {
        converted = (uint16_t)(aux * 100);
    }
    
    if (value < 0) {        
        converted = converted ^ 0b1000000000000000;
    }
    
    return converted;
}

/**
 * @fn float2uint16
 * @brief Convert float to UINT_16.
 * @details Range: decimal = 0 (0 <-> 65535)
 *                 decimal = 1 (0 <-> 6553.5)
 *                 decimal = 2 (0 <-> 655.35)
 *                 decimal = 3 (0 <-> 65.535)
 * @param[in] value - float value.
 * @param[in] decimal - decimal digits.
 * @return uint16_t - Converted value.
 */ 
uint16_t float2uint16(float value, uint8_t decimal) {
    uint16_t converted = 0;
    float aux = value;

    if (value < 0) {
        aux = aux * -1;        
    }

    if (decimal == 0) {
        converted = (uint16_t)aux;
    } else if (decimal == 1) {
        converted = (uint16_t)(aux * 10);
    } else if (decimal == 2) {
        converted = (uint16_t)(aux * 100);
    }        

    return converted;
}

String byte2hex(uint8_t value) {
    String hex = "";

    if (value <= 0xF) {
        hex.concat("0");
    }
    hex.concat(String(value, HEX));

    return hex;    
}

String short2hex(uint16_t value) {
    byte low = 0;
    byte high = 0;
    String hex = "";

    low = lowByte(value);
    if (low <= 0xF) {
        hex.concat("0");
    }
    hex.concat(String(low, HEX));

    high = highByte(value);
    if (high <= 0xF) {
        hex.concat("0");
    }
    hex.concat(String(high, HEX));

    return hex;
}

String long2hex(uint32_t value) {
    String hex = "";
    byte aux = 0;

    aux = (byte)((value & 0xFF000000) >> 24);
    if (aux <= 0xF) {
        hex.concat("0");
    }
    hex.concat(String(aux, HEX));

    aux = (byte)((value & 0x00FF0000) >> 16);
    if (aux <= 0xF) {
        hex.concat("0");
    }
    hex.concat(String(aux, HEX));

    aux = (byte)((value & 0x0000FF00) >> 8);
    if (aux <= 0xF) {
        hex.concat("0");
    }
    hex.concat(String(aux, HEX));

    aux = (byte)((value & 0x000000FF));
    if (aux <= 0xF) {
        hex.concat("0");
    }
    hex.concat(String(aux, HEX));

    return hex;
}

#endif //  __CONVERT_TOOLS_H__