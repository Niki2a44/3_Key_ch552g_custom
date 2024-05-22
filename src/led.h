#pragma once
// Key colors (hue value: 0..191)
#define NEO_RED 0    // red
#define NEO_YEL 32   // yellow
#define NEO_GREEN 64 // green
#define NEO_CYAN 96  // cyan
#define NEO_BLUE 128 // blue
#define NEO_MAG 160  // magenta
#define NEO_WHITE 191  // white
#define NEO_BRIGHT_KEYS 2

#define NUM_LED_CONFIGURATION 6

enum led_keyboard_mode_t
{
  LED_LOOP,
  LED_FIX,
  LED_BLINK
};

typedef enum 
{
  RAINBOW, // send button_sequence_t tyoe
  SOLID,    // send button_mouse_t type
  BREATHING,    // send button_mouse_t type
  BUTTONPRESS,    // send button_mouse_t type
  
}cycle_type;

typedef struct 
{
    cycle_type type;
    uint8_t speed; // length of the array
    uint8_t r; // ms delay between event
    uint8_t g; // keypress to send with the mouse event
    uint8_t b; // keypress to send with the mouse event
    
}led_modes;


typedef struct 
{
   led_modes led[NUM_LED_CONFIGURATION];
}led_configuration_t;


// change led mode
void led_set_mode(enum led_keyboard_mode_t mode);

// set led color in FIX mode
void led_set_color_hue(uint8_t led0, uint8_t led1, uint8_t led2);

// update led task
void led_update();

//if in loop mode, change color to pressed key
void led_presskey(int key);
void load_rgbtq();
void increase_brightness();
void decrease_brightness();
void loop_modes();
void ApplyBrightnessHUE(int pixel, int h,int br);