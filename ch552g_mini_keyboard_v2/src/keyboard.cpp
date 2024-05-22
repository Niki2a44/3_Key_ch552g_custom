#include <Arduino.h>
#include "userUsbHidKeyboardMouse/USBHIDKeyboardMouse.h"
// #include "auto_mode.h"
#include "led.h"
#include "keyboard.h"
#include "../configuration.h"

#define MENU_CONF NUM_CONFIGURATION - 1

static int current_mode_s = 0;                                       // current mode of keyboard
static int menu_mode_s = 0;                                          // during menu coice

static const int DATA_FLASH_PROFILE_ADDR = 0;
static void set_menu_led(void);
static void enter_menu(void);
static void keyboard_press_mode_1(keyboard_button_t button, keyboard_button_keyboard_mode_t mode);
static void keyboard_press_mode_2(keyboard_button_t button, keyboard_button_keyboard_mode_t mode);
static void keyboard_press_auto(keyboard_button_t button, keyboard_button_keyboard_mode_t mode);
static void keyboard_press_menu(keyboard_button_t button, keyboard_button_keyboard_mode_t mode);
uint8_t WriteDataFlash(uint8_t addr, uint8_t* buf, uint8_t len);
uint8_t ReadDataFlash(uint8_t addr, uint8_t len, uint8_t* buf);
void save_profile(int profile);
// ===================================================================================
// create keyboard configuration
// ===================================================================================

const button_function_t button_function_null = {
    .type = BUTTON_NULL};



static void set_menu_led(void)
{

  int led_index = menu_mode_s % 3;
  int color_background = NEO_CYAN + (menu_mode_s / 3) * 32;

  led_set_mode(LED_FIX);
  led_set_color_hue((led_index == 0) ? NEO_RED : color_background,
                    (led_index == 1) ? NEO_RED : color_background,
                    (led_index == 2) ? NEO_RED : color_background);
}

void keyboard_press_enc(keyboard_button_keyboard_mode_t mode)
{
  if (mode == BTM_PRESS)
  {
   
    menu_mode_s = current_mode_s;
    current_mode_s = MENU_CONF;
    set_menu_led();
  }
  if (mode == BTM_RELEASE)
  {
    current_mode_s = menu_mode_s;
    save_profile(current_mode_s);
    led_set_mode(LED_LOOP);
  }
}

void button_menu_up(keyboard_button_keyboard_mode_t mode)
{
  if (mode == BTM_CLICK)
  {
    if (menu_mode_s >= MENU_CONF - 1)
    {
      menu_mode_s = MENU_CONF - 1;
    }
    else
    {
      menu_mode_s++;
    }
    set_menu_led();
  }
}

void button_menu_down(keyboard_button_keyboard_mode_t mode)
{
  if (mode == BTM_CLICK)
  {
    if (menu_mode_s <= 0)
    {
      menu_mode_s = 0;
    }
    else
    {
      menu_mode_s--;
    }
    set_menu_led();
  }
}
void save_profile(int profile) {
    uint8_t buf[2] = {DATA_FLASH_PROFILE_ADDR, (uint8_t)profile}; // Combine address and profile
    if (WriteDataFlash(0, buf, 2) == 2) { // Assuming your WriteDataFlash function takes offset as the first argument.
        // Successfully wrote to data flash
    } else {
        // Handle data flash write error 
    }
}

void load_profile() {
    uint8_t buf[2] = {0}; // Initialize buffer
    if (ReadDataFlash(DATA_FLASH_PROFILE_ADDR, 2, buf) == 2) {
        int profile = buf[1]; // Extract profile from the buffer
        current_mode_s = profile;
        menu_mode_s = profile;
        set_menu_led();
        delay(500);
        led_set_mode(LED_LOOP);
    } else {
        // Handle data flash read error
    }
}

void button_brightness_up(keyboard_button_keyboard_mode_t mode){
   
    increase_brightness();
  
}
void button_brightness_down(keyboard_button_keyboard_mode_t mode){

    decrease_brightness();
  
}
void button_loop_led(keyboard_button_keyboard_mode_t mode){
 if (mode == BTM_RELEASE)
  {
    loop_modes();
  }
}

static void keyboard_run_key_sequence(button_sequence_t sequence, keyboard_button_keyboard_mode_t mode)
{
  if (mode == BTM_RELEASE)
  {
    return;
  }

  for (uint8_t i = 0; i < sequence.length; i++)
  {
    Keyboard_press(sequence.sequence[i]);
    delay(10);
    if (sequence.delay > 0)
    {
      Keyboard_release(sequence.sequence[i]);
      delay(sequence.delay);
    }
  }
  Keyboard_releaseAll();
}

static void keyboard_run_mouse_sequence(button_mouse_t sequence, keyboard_button_keyboard_mode_t mode)
{
  if (mode == BTM_RELEASE)
  {
    return;
  }
  if (sequence.keypress > 0)
  {
    Keyboard_press(sequence.keypress);
    delay(30);
  }
  for (uint8_t i = 0; i < sequence.length; i++)
  {
    switch (sequence.mouse_event_sequence[i].type)
    {
    case SCROLL_UP:
      Mouse_scroll(sequence.mouse_event_sequence[i].value);
      break;
    case SCROLL_DOWN:
      Mouse_scroll(-sequence.mouse_event_sequence[i].value);
      break;
    default:
      break;
    }
    if (sequence.keypress > 0)
    {
      Keyboard_releaseAll();
    }
    delay(sequence.delay);
  }
}

void keyboard_press_button(keyboard_button_t button, keyboard_button_keyboard_mode_t mode)
{
  if (button >= BTN_1 && button <= BTN_3)
  {
    if (mode == BTM_PRESS)
    {
      led_presskey(button);
    }
    else
    {
      led_presskey(-1);
    }
  }

  switch (configurations[current_mode_s].button[button].type)
  {
  case BUTTON_SEQUENCE:
    keyboard_run_key_sequence(configurations[current_mode_s].button[button].function.sequence, mode);
    break;
  case BUTTON_MOUSE:
    keyboard_run_mouse_sequence(configurations[current_mode_s].button[button].function.mouse, mode);
    break;
  case BUTTON_FUNCTION:
    configurations[current_mode_s].button[button].function.functionPointer(mode);
    break;
  case BUTTON_NULL:
    
    break;
  default:
    break;
  }
}

void keyboard_setup()
{

}

uint8_t WriteDataFlash(uint8_t addr, uint8_t* buf, uint8_t len) {
	uint8_t i;
	SAFE_MOD = 0x55;
	SAFE_MOD = 0xAA;

	GLOBAL_CFG |= bDATA_WE;

	SAFE_MOD = 0;

	ROM_ADDR_H = DATA_FLASH_ADDR >> 8;
	addr <<= 1;

	for (i = 0; i < len; i++) {
		ROM_ADDR_L = addr + i * 2;
		ROM_DATA_L = *(buf + i);
		if (ROM_STATUS & bROM_ADDR_OK) {
			ROM_CTRL = ROM_CMD_WRITE;
		}
		if ((ROM_STATUS ^ bROM_ADDR_OK) > 0) {
			return i;
		}
	}

	SAFE_MOD = 0x55;
	SAFE_MOD = 0xAA;
	GLOBAL_CFG &= ~bDATA_WE;
	SAFE_MOD = 0;

	return i;
}

uint8_t ReadDataFlash(uint8_t addr, uint8_t len, uint8_t* buf) {
	uint8_t i;

	ROM_ADDR_H = DATA_FLASH_ADDR >> 8;
	addr <<= 1;
	for (i = 0; i < len; i++) {
		ROM_ADDR_L = addr + i * 2;
		ROM_CTRL = ROM_CMD_READ;
		*(buf + i) = ROM_DATA_L;
	}
	return i;
}