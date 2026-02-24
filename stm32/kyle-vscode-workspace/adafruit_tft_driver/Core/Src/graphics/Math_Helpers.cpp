#include <stdint.h>
#include "Math_Helpers.h"

/**
******************************************************************************
* @file           : Math_Helpers.cpp
* @brief          : Math helper functions for graphics drivers
******************************************************************************
* This software is licensed under terms that can be found in the LICENSE file
* in the root directory of this software component.
* If no LICENSE file comes with this software, it is provided AS-IS.
******************************************************************************
*/

/**
* @brief      Map a number from one range to another
* @param      x the number to map
* @param      in_min the lower bound of the range to map from
* @param      in_max the upper bound of the range to map from
* @param      out_min the lower bound of the range to map to
* @param      out_max the upper bound of the range to map to
*
*  @return     the mapped value
*
* Note: Port of function from Arduino to STM32. Information on original function can be found at 
* https://docs.arduino.cc/language-reference/en/functions/math/map/
*/
uint32_t map(uint32_t x, uint32_t in_min, uint32_t in_max, uint32_t out_min, uint32_t out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

/**
* @brief      Return the smaller of two numbers
* @param      a the first number
* @param      b the second number
*/
uint32_t min(uint32_t a, uint32_t b) {
  return (a < b) ? a : b;
}