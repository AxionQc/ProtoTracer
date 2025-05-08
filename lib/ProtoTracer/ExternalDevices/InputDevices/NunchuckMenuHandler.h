/**
 * @file NunchuckMenuHandler.h
 * @brief Declares the MenuHandler template class for managing menu interactions using WiiChuck.
 *
 * This file defines the MenuHandler template class, which provides functionality for managing
 * multiple menus and settings through a WiiChuck controller interface, including EEPROM
 * storage for persistent settings.
 *
 * @date 22/12/2024
 */

 #pragma once

 #include <Arduino.h> // Include for Arduino compatibility.
 #include <WiiChuck.h> // Include for WiiChuck controller support.
 #include <EEPROM.h>  // Include for EEPROM functionality.
 
 /**
  * @class MenuHandler
  * @brief Manages multiple menus and settings using WiiChuck.
  *
  * The MenuHandler class provides functionality for navigating and managing settings across
  * multiple menus, including support for EEPROM storage, custom settings, and dynamic updates.
  *
  * @tparam menuCount The number of menus to manage.
  */
 template <uint8_t menuCount>
 class MenuHandler {
 private:
     static Accessory controller; ///< Instance of the WiiChuck controller.
     static uint8_t currentMenu; ///< Index of the currently active menu.
     static uint8_t currentSetting; ///< Index of the currently active setting within the menu.
     static uint8_t currentValue[menuCount]; ///< Array of current values for each menu.
     static uint8_t maxValue[menuCount]; ///< Array of maximum values for each menu.
     static uint8_t faceChoices; ///< Number of face choices available.
     static bool menuActive; ///< Flag indicating whether the menu is active.
     static bool isMenuActive; ///< New flag indicating if the menu is currently active.
     static bool didBegin; ///< Flag indicating whether the initialization process has been completed.
     static unsigned long lastActionTime; ///< Timestamp of the last action for debounce.
     static const unsigned long debounceDelay = 200; ///< Debounce delay in milliseconds.
 
     /**
      * @brief Reads a value from EEPROM at the specified index.
      *
      * @param index The EEPROM address to read from.
      * @return The value read from EEPROM.
      */
     static uint8_t ReadEEPROM(uint16_t index);
 
     /**
      * @brief Writes a value to EEPROM at the specified index.
      *
      * @param index The EEPROM address to write to.
      * @param value The value to write to EEPROM.
      */
     static void WriteEEPROM(uint16_t index, uint8_t value);
 
 public:
     /**
      * @brief Updates the menu state and processes user interactions.
      */
     static void Update();
 
     /**
      * @brief Initializes the MenuHandler and sets up the WiiChuck controller.
      *
      * @return True if initialization was successful, false otherwise.
      */
     static bool Initialize();
 
     /**
      * @brief Sets a default value for a specific menu.
      *
      * @param menu The menu index to set the default value for.
      * @param value The default value to set.
      */
     static void SetDefaultValue(uint16_t menu, uint8_t value);
 
     /**
      * @brief Marks the menu system as initialized.
      */
     static void SetInitialized();
 
     /**
      * @brief Sets the maximum value for a specific menu.
      *
      * @param menu The menu index to set the maximum value for.
      * @param maxValue The maximum value to set.
      */
     static void SetMenuMax(uint8_t menu, uint8_t maxValue);
 
     /**
      * @brief Retrieves the current value of a specific menu.
      *
      * @param menu The menu index to retrieve the value from.
      * @return The current value of the menu.
      */
     static uint8_t GetMenuValue(uint8_t menu);
 
     /**
      * @brief Retrieves the index of the currently active menu.
      *
      * @return The index of the active menu.
      */
     static uint8_t GetCurrentMenu();
 };
 
 #include "NunchuckMenuHandler.tpp" // Include the template implementation.