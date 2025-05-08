#pragma once

#include <WiiChuck.h>
#include <EEPROM.h>

// Uncomment the following line to enable debug output
#define DEBUG

#ifdef DEBUG
#include <Arduino.h>
#define DEBUG_PRINT(x) Serial.print(x)
#define DEBUG_PRINTLN(x) Serial.println(x)
#else
#define DEBUG_PRINT(x)
#define DEBUG_PRINTLN(x)
#endif

template <uint8_t menuCount>
Accessory MenuHandler<menuCount>::controller;

template <uint8_t menuCount>
uint8_t MenuHandler<menuCount>::currentMenu;

template <uint8_t menuCount>
uint8_t MenuHandler<menuCount>::currentSetting = 0;

template <uint8_t menuCount>
uint8_t MenuHandler<menuCount>::currentValue[menuCount];

template <uint8_t menuCount>
uint8_t MenuHandler<menuCount>::maxValue[menuCount];
template <uint8_t menuCount>
uint8_t MenuHandler<menuCount>::faceChoices = 0;
template <uint8_t menuCount>
bool MenuHandler<menuCount>::menuActive = false;

template <uint8_t menuCount>
bool MenuHandler<menuCount>::didBegin = false;

template <uint8_t menuCount>
const unsigned long MenuHandler<menuCount>::debounceDelay; // Remove duplicate initialization

template <uint8_t menuCount>
unsigned long MenuHandler<menuCount>::lastActionTime = 0; // Initialize last action time

template <uint8_t menuCount>
bool MenuHandler<menuCount>::isMenuActive = false; // Initialize isMenuActive

template <uint8_t menuCount>

bool MenuHandler<menuCount>::Initialize() {
    DEBUG_PRINTLN("Initializing MenuHandler...");
    DEBUG_PRINT("Menu count: ");
    DEBUG_PRINTLN(maxValue[currentMenu]);
    controller.begin();
    if (controller.type == Unknown) {
        controller.type = NUNCHUCK;  // Default to Nunchuck if type is unknown
        DEBUG_PRINTLN("Controller type unknown, defaulting to NUNCHUCK.");
    }

    didBegin = true;

    for (uint8_t i = 0; i < menuCount; i++) {
        uint8_t value = ReadEEPROM(i);
        currentValue[i] = (value == 255) ? 0 : value;  // Handle uninitialized EEPROM values
        DEBUG_PRINT("Menu ");
        DEBUG_PRINT(i);
        DEBUG_PRINT(" initialized with value: ");
        DEBUG_PRINTLN(currentValue[i]);
    }

    bool initialized = ReadEEPROM(menuCount + 1) != 255;
    DEBUG_PRINTLN(initialized ? "MenuHandler initialized from EEPROM." : "MenuHandler not initialized in EEPROM.");
    return initialized;
}

template <uint8_t menuCount>
void MenuHandler<menuCount>::Update() {
    if (!didBegin) return;

    unsigned long currentTime = millis();
    if (currentTime - lastActionTime < debounceDelay) return; // Debounce check

    controller.readData();

    // Example: Use joystick input to navigate menus
    if (controller.getButtonC()) {  // Joystick moved right
        currentMenu =1;
        isMenuActive = true;
        lastActionTime = currentTime; 
    }



    if(isMenuActive){
        if (controller.getButtonZ()) {  // Joystick moved left
            currentMenu = 0;
            isMenuActive = false;
            lastActionTime = currentTime; 
        }
        if (controller.getJoyY() > 200) {  // Joystick moved right
            currentValue[currentMenu] += 1;
            if (currentValue[currentMenu] >= maxValue[currentMenu]) currentValue[currentMenu] = 0;
            lastActionTime = currentTime; 
        } else if (controller.getJoyY() < 50) {  // Joystick moved left
            currentValue[currentMenu] -= 1;
            if (currentValue[currentMenu] < 0) currentValue[currentMenu] = maxValue[currentMenu];
            lastActionTime = currentTime; 
        }
        if (controller.getJoyX() > 200) {  // Joystick moved right
            if (currentMenu <= 12){
            currentMenu++;
            if (currentMenu >= 12) currentMenu = 1;
            } 

            DEBUG_PRINTLN(currentMenu);
            lastActionTime = currentTime; 
        } else if (controller.getJoyX() < 50) {  // Joystick moved left
            currentMenu--;
            lastActionTime = currentTime;
        }
    }
    else
    {

        if (controller.getJoyX() > 200) {  // Joystick moved right
            currentValue[currentMenu] += 1;
            if (currentValue[currentMenu] >= maxValue[currentMenu]) currentValue[currentMenu] = 0;
            lastActionTime = currentTime; 
        } 
        else if (controller.getJoyX() < 50) {  // Joystick moved left
            currentValue[currentMenu] -= 1;
            if (currentValue[currentMenu] == 0) currentValue[currentMenu] = maxValue[currentMenu] - 1;
            lastActionTime = currentTime; 
        }


    }
}

template <uint8_t menuCount>
uint8_t MenuHandler<menuCount>::ReadEEPROM(uint16_t index) {
    return EEPROM.read(index);
}

template <uint8_t menuCount>
void MenuHandler<menuCount>::WriteEEPROM(uint16_t index, uint8_t value) {
    EEPROM.write(index, value);
}

template <uint8_t menuCount>
void MenuHandler<menuCount>::SetDefaultValue(uint16_t menu, uint8_t value) {
    if (menu >= menuCount) return;

    currentValue[menu] = value;

    WriteEEPROM(menu, value);
}

template <uint8_t menuCount>
void MenuHandler<menuCount>::SetInitialized() {
    WriteEEPROM(menuCount + 1, 0);
}

template <uint8_t menuCount>
void MenuHandler<menuCount>::SetMenuMax(uint8_t menu, uint8_t maxValue) {
    if (menu >= menuCount) return;

    MenuHandler::maxValue[menu] = maxValue;
}

template <uint8_t menuCount>
uint8_t MenuHandler<menuCount>::GetMenuValue(uint8_t menu) {
    return currentValue[menu];
}

template <uint8_t menuCount>
uint8_t MenuHandler<menuCount>::GetCurrentMenu() {
    return currentMenu;
}
