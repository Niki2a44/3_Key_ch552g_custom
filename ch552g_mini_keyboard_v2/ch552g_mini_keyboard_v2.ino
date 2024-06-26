#ifndef USER_USB_RAM
#error "Require USB RAM. Go Tools > USB Setting and pick the 2nd option in the dropdown list"
#endif

//lib include
#include "src/neo/neo.h"
#include "src/userUsbHidKeyboardMouse/USBHIDKeyboardMouse.h"

//app include
// #include "src/auto_mode.h"
#include "src/buttons.h"
#include "src/encoder.h"
#include "src/keyboard.h"
#include "src/led.h"
#include "src/util.h"

// Button (Mechnical, left to right)
#define PIN_BTN_1 11
#define PIN_BTN_2 17
#define PIN_BTN_3 16
#define PIN_BTN_ENC 33

#define ENCODER_A 31
#define ENCODER_B 30


#define LED_PIN 34 // Pin for the LED strip, configure neo leds in src/neo/config.h




// ===================================================================================
// Main section
// ============================================================================
// Initialize pins
void setup()
{

  // Initialize neopixels
  NEO_init();
  delay(10);
  NEO_clearAll();

  // Go in bootloader more if connected with encoder button pressed
  if (!digitalRead(PIN_BTN_ENC))
  {
    led_boot();
    
                             // update pixels
    BOOT_now();     // jump to bootloader
  }
  load_profile();
  load_rgbtq();
  buttons_setup(PIN_BTN_1, PIN_BTN_2, PIN_BTN_3, PIN_BTN_ENC);
  keyboard_setup();
  encoder_setup(ENCODER_A, ENCODER_B);
  led_set_mode(LED_LOOP);
  USBInit();
}


//Main loop, read buttons
void loop()
{

  //task update
  buttons_update();
  // auto_update();
  encoder_update();
  led_update();

  //debouncing
  delay(3); 
}
