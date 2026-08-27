/**
 * @file printMacro.hpp
 * @brief File that handles print macro for all of the project.
 *        When importing this file, you must declare a [static const bool DEBUG_PRINT] to
 *        true if you want the prints in your file to be displayed, or false if not. 
 * 
 * @author Gabriel Desrochers
 * @date 2026-08-27
 */

#ifndef PRINT_MACRO_HPP
#define PRINT_MACRO_HPP

// macros wrapped in do while(false) to protect from weird runtime bugs.
// https://vcstutoring.ca/wrapping-multiline-macros-in-c-with-do-while/

/** 
 * @brief macro for the Serial.print() function, 
 * DEBUG_PRINT must be set to true for the prints to be displayed on this file
 */
#define PRINT(x) do { if (DEBUG_PRINT) Serial.print(x); } while(false)

/** 
 * @brief macro for the Serial.println() function, 
 * DEBUG_PRINT must be set to true for the prints to be displayed on this file
 */
#define PRINTLN(x) do { if (DEBUG_PRINT) Serial.println(x); } while(false)

/** 
 * @brief macro for the Serial.printf() function, 
 * DEBUG_PRINT must be set to true for the prints to be displayed on this file
 */
#define PRINTF(...) do { if (DEBUG_PRINT) Serial.printf(__VA_ARGS__); } while(false)

#endif