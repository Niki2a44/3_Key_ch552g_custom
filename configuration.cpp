#include "configuration.h"
#include "src/userUsbHidKeyboardMouse/USBHIDKeyboardMouse.h"

const keyboard_configuration_t configurations[NUM_CONFIGURATION] = {
    {
        .button = { // Configuration copy paste keyboard
            [BTN_1] = {
                .type = BUTTON_SEQUENCE,
                .function.sequence = {
                    .sequence = {KEY_LEFT_CTRL, 'z'}, // ctxxxxxxxxxxxxxrl + z
                    .length = 2,            // Lengh of sequence
                    .delay = 0             // no delay
                }
            },
            [BTN_2] = {
                .type = BUTTON_SEQUENCE,
                .function.sequence = {
                    .sequence = {'c'}, // ctrl + v
                    .length = 1,            // Lengh of sequence
                    .delay = 0             // no delay
                }
            },
            [BTN_3] = {
                .type = BUTTON_SEQUENCE,
                .function.sequence = {
                    .sequence = {'x'}, // ctrl + c
                    .length = 1,            // Lengh of sequence
                    .delay = 0             // no delay
                }
            },
            [ENC_CW] = { // scroll up
                .type = BUTTON_SEQUENCE,
                .function.sequence = {
                    .sequence = {KEY_LEFT_CTRL, ','}, // ctxxxxxxxxxxxxxrl + z
                    .length = 2,            // Lengh of sequence
                    .delay = 0             // no delay
                }
            },
            [ENC_CCW] = {   // scroll down
                 .type = BUTTON_SEQUENCE,
                .function.sequence = {
                    .sequence = {KEY_LEFT_CTRL, '.'}, // ctxxxxxxxxxxxxxrl + z
                    .length = 2,            // Lengh of sequence
                    .delay = 0             // no delay
                }
            },
            [BTN_ENC] = {
                .type = BUTTON_FUNCTION,
                .function.functionPointer = keyboard_press_enc,
            },
        }
    },
    
    {   // Configuration game
        .button = { // 
            [BTN_1] = {
                .type = BUTTON_SEQUENCE,
                .function.sequence = {
                    .sequence = {KEY_LEFT_CTRL, 'v'}, // multi a
                    .length = 2,            // Lengh of sequence
                    .delay = 0             // no delay
                }
            },
            [BTN_2] = {
                .type = BUTTON_SEQUENCE,
                .function.sequence = {
                    .sequence = {KEY_LEFT_CTRL, 'c'}, // multi b
                    .length = 2,            // Lengh of sequence
                    .delay = 0             // no delay
                }
            },
            [BTN_3] = {
                .type = BUTTON_SEQUENCE,
                .function.sequence = {
                    .sequence = {KEY_LEFT_CTRL, 'x'}, // multi b
                    .length = 2,            // Lengh of sequence
                    .delay = 0             // no delay
                }
            },
            [ENC_CW] = {
                .type = BUTTON_MOUSE,
                .function.mouse = {
                    .mouse_event_sequence = {
                        {
                            .type = SCROLL_UP,
                            .value = 1
                        }
                    },
                    .length = 1,
                    .delay = 0,
                    .keypress = KEY_LEFT_SHIFT // Valore della pressione del tasto del mouse
                }
            },
            [ENC_CCW] = {
                .type = BUTTON_MOUSE,
                .function.mouse = {
                    .mouse_event_sequence = {
                        {
                            .type = SCROLL_DOWN,
                            .value = 1
                        }
                    },
                    .length = 1,
                    .delay = 0,
                    .keypress = KEY_LEFT_SHIFT// Valore della pressione del tasto del mouse
                }
            },
            [BTN_ENC] = {
                .type = BUTTON_FUNCTION,
                .function.functionPointer = keyboard_press_enc,
            },
        }
    },
    {   // Configuration photoshop
        .button = { // Configuration copy paste keyboard
            [BTN_1] = {
                .type = BUTTON_SEQUENCE,
                .function.sequence = {
                    .sequence = { 'S','l','a','v','a',' ','U','k','r','a','i','n','i','!'}, // ctxxxxxxxxxxxxxrl + z
                    .length = 14,            // Lengh of sequence
                    .delay = 2             // no delay
                }
            },
            [BTN_2] = {
                .type = BUTTON_SEQUENCE,
                .function.sequence = {
                    .sequence = {'a'}, // ctrl + v
                    .length = 1,            // Lengh of sequence
                    .delay = 0             // no delay
                }
            },
            [BTN_3] = {
                .type = BUTTON_SEQUENCE,
                .function.sequence = {
                    .sequence = {'g'}, // ctrl + c
                    .length = 1,            // Lengh of sequence
                    .delay = 0             // no delay
                }
            },
            [ENC_CW] = { // scroll up
                .type = BUTTON_FUNCTION,
                .function.functionPointer = button_brightness_up,
            },
            [ENC_CCW] = {   // scroll down
                 .type = BUTTON_FUNCTION,
                .function.functionPointer = button_brightness_down,
            },
            [BTN_ENC] = {
                .type = BUTTON_FUNCTION,
                .function.functionPointer = keyboard_press_enc,
            },
        }
    },
    
    {   //Menu configuration
        .button = { // Configurtion copy paste keyboard
            [BTN_1] = {
                .type = BUTTON_FUNCTION,
                .function.functionPointer = button_brightness_down,
            },
            [BTN_2] = {
                .type = BUTTON_FUNCTION,
                .function.functionPointer = button_loop_led,
            },
            [BTN_3] = {
                .type = BUTTON_FUNCTION,
                .function.functionPointer = button_brightness_up,
            },
            [ENC_CW] = {
                .type = BUTTON_FUNCTION,
                .function.functionPointer = button_menu_up,
            },
            [ENC_CCW] = {
                .type = BUTTON_FUNCTION,
                .function.functionPointer = button_menu_down,
            },
            [BTN_ENC] = {
                .type = BUTTON_FUNCTION,
                .function.functionPointer = keyboard_press_enc,
            },
        }
    },
};

const led_configuration_t led_configurations[NUM_LED_CONFIGURATION] = {
    {
        .led = { // Configurtion copy paste keyboard
            [1] = {
              .type = RAINBOW,
              .speed = 2,
            },
            [2] = {
              .type = SOLID,
              .r = 255 ,
              .g = 0,
              .b = 0,
            },
            
            [3] = {
              .type = BUTTONPRESS,
              .speed = 2,
              .r = 255,
              .g = 0,
              .b = 0,
            },
            [4] = {
              .type = SOLID,
              .r = 0 ,
              .g = 255,
              .b = 255,
            },
            [5] = {
              .type = BREATHING,
              .speed = 1,
              .r = 255 ,
              .g = 0,
              .b = 0,
            },
            }

    },

};

