#include <Arduino.h>
#include "neo/neo.h"
#include "led.h"
#include <stdbool.h>
#include "../configuration.h"
// ===================================================================================
// Color section
// ============================================================================
static const int DATA_FLASH_LIGHT_ADDR = 2;
static const int DATA_FLASH_BRIGHTNESS_ADDR = 4;
static int MAX_MODES = NUM_LED_CONFIGURATION -1;
static enum led_keyboard_mode_t led_mode_s = LED_LOOP;
static int color_hue_s[3] = {0, 0, 0}; // hue value: 0..191 color map
static int color_rgb[3][3] = {{0,0,0}, {0,0,0}, {0,0,0}}; // hue value: 0..191 color map
static int curretn_key_s = -1;         // current press key
int h1 = 0;
bool t2 = true;
bool rainbow = false;
int brightnes = 255;
int step = 20;
int lmode = 1;

void led_set_color_hue(uint8_t led0, uint8_t led1, uint8_t led2)
{
  color_hue_s[0] = led0;
  color_hue_s[1] = led1;
  color_hue_s[2] = led2;
}

void led_set_mode(enum led_keyboard_mode_t mode)
{
  led_mode_s = mode;
  switch (mode)
  {
  case LED_LOOP:
    color_hue_s[0] = NEO_RED;
    color_hue_s[1] = NEO_YEL;
    color_hue_s[2] = NEO_GREEN;
    break;
  }
}
void load_rgbtq(){
    uint8_t buf[2] = {0}; // Initialize buffer
    if (ReadDataFlash(DATA_FLASH_LIGHT_ADDR, 2, buf) == 2) {
        int profile = buf[1]; // Extract profile from the buffer
        lmode = profile;
    }

    uint8_t buf2[2] = {0};
    if (ReadDataFlash(DATA_FLASH_BRIGHTNESS_ADDR, 2, buf2) == 2) {
        int profile2 = buf2[1]; // Extract profile from the buffer
        brightnes = profile2;
    }
}

// if in loop mode, change color to pressed key
void led_presskey(int key)
{
  curretn_key_s = key;
}

void save_brightness(){
  uint8_t buf[2] = {DATA_FLASH_BRIGHTNESS_ADDR, (uint8_t)brightnes}; // Combine address and profile
    if (WriteDataFlash(DATA_FLASH_BRIGHTNESS_ADDR, buf, 2) == 2) { // Assuming your WriteDataFlash function takes offset as the first argument.
        // Successfully wrote to data flash
    } else {
        // Handle data flash write error 
    }
}
void decrease_brightness(){
  int new = brightnes -step;
  brightnes  = max(0,new);
  save_brightness();
}

void increase_brightness(){
  int new = brightnes + step;
  brightnes  = min(255,new);
  save_brightness();
}


void loop_modes(){  
    lmode++;;
    if(lmode > MAX_MODES){
      lmode = 1;
    }
    uint8_t buf[2] = {DATA_FLASH_LIGHT_ADDR, (uint8_t)lmode}; // Combine address and profile
    if (WriteDataFlash(DATA_FLASH_LIGHT_ADDR, buf, 2) == 2) { // Assuming your WriteDataFlash function takes offset as the first argument.
        // Successfully wrote to data flash
    } else {
        // Handle data flash write error 
    }

}


void led_rainbow(int speed){
  for (int i = 0; i < 3; i++)
    {
      color_hue_s[i] += speed;
      if (color_hue_s[i] > 191)
      {
        color_hue_s[i] = 0;
      }
    }
}

void led_solid(){
  for (int i = 0; i < 3; i++)
    {
      color_rgb[i][0] = led_configurations[0].led[lmode].r;
      color_rgb[i][1] =led_configurations[0].led[lmode].g;
      color_rgb[i][2] =led_configurations[0].led[lmode].b;
    }
}
void led_button(int speed){
  for (int i = 0; i < 3; i++)
    {

      color_rgb[i][0]=color_rgb[i][0]-speed;
      color_rgb[i][1]=color_rgb[i][1]-speed;
      color_rgb[i][2]=color_rgb[i][2]-speed;
      for(int y = 0; y<3 ;y++){
        if(color_rgb[i][y]<0){
          color_rgb[i][y] = 0;
        }
      }
    }
}
void led_breathe(int speed){
  if(t2){
    h1 = h1 + speed;
  }else{
    h1 = h1 - speed;
  }
  if(h1 >= 255){
    t2 = false;
  }
  if(h1 <= 0){
    t2 = true;
  }


  for (int i = 0; i < 3; i++)
    {
      

      color_rgb[i][0] = max(led_configurations[0].led[lmode].r -h1,0);
      color_rgb[i][1] =max(led_configurations[0].led[lmode].g -h1,0);
      color_rgb[i][2] =max(led_configurations[0].led[lmode].b -h1,0);
    }
}

void ApplyBrightnessRGB(int led, int r, int g, int b, int targetBrightness) {
  targetBrightness = 255 - targetBrightness;
  r = max(0,r-targetBrightness);
  g = max(0,g-targetBrightness);
  b = max(0,b-targetBrightness);

  NEO_writeColor(led, r, g, b);
}
void ApplyBrightnessHUE(int pixel, int h,int br){
  br = 255-br;
  uint8_t phase = h >> 6;
  uint8_t step  = (h & 63) ;
  uint8_t nstep = (63) - step;
  nstep = max(0 ,nstep-br);
  step = max(0,step-br);
  switch(phase) {
    case 0:   NEO_writeColor(pixel, nstep,  step,     0); break;
    case 1:   NEO_writeColor(pixel,     0, nstep,  step); break;
    case 2:   NEO_writeColor(pixel,  step,     0, nstep); break;
    default:  break;
  }
 
}


void led_update()
{
  if (led_mode_s == LED_LOOP)
  {
    switch (led_configurations[0].led[lmode].type)
    {
    case RAINBOW:
          if(rainbow){
            color_hue_s[0] = NEO_RED;
            color_hue_s[1] = NEO_YEL;
            color_hue_s[2] = NEO_GREEN;
            rainbow = true;
          }
      
      led_rainbow(led_configurations[0].led[lmode].speed);
    break;
    case SOLID:
      h1 = 0;
      rainbow = false;
      led_solid();
    case BREATHING:
      rainbow = false;
      led_breathe(led_configurations[0].led[lmode].speed);
    case BUTTONPRESS:
      rainbow = false;
      led_button(led_configurations[0].led[lmode].speed);
    break;
    };
  }
  for (int led = 0; led < 3; led++)
  {
    if (curretn_key_s == led)
    {
      if(led_configurations[0].led[lmode].type == BUTTONPRESS && led_mode_s != LED_FIX){
          color_rgb[led][0] = led_configurations[0].led[lmode].r;
          color_rgb[led][1] = led_configurations[0].led[lmode].g;
          color_rgb[led][2] = led_configurations[0].led[lmode].b;
      }else{
          ApplyBrightnessRGB(led, 255, 255, 255 , brightnes);
      }
      
    }
    else
    {
      if(led_mode_s != LED_FIX){
          if(led_configurations[0].led[lmode].type == RAINBOW){
                  ApplyBrightnessHUE(led, color_hue_s[led],brightnes);
          }else{
            
            ApplyBrightnessRGB(led, color_rgb[led][0], color_rgb[led][1], color_rgb[led][2],brightnes );
          }
      }
      else{
            ApplyBrightnessHUE(led, color_hue_s[led],255);
      }
      
      
    }
  }

  NEO_update();
}

