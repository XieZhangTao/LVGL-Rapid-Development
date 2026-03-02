#ifndef __EMB_LV_CONF_H__
#define __EMB_LV_CONF_H__

// <<< Use Configuration Wizard in Context Menu >>>

// <q>lvgl use file system
#ifndef __LV_USE_FILE_SYSTEM__
#   define __LV_USE_FILE_SYSTEM__          0
#endif

// <h>lvgl indev
    // <q>lvgl use keypad indev
    #ifndef __LV_USE_KEYPAD_INDEV__
    #   define __LV_USE_KEYPAD_INDEV__     1
    #endif
    // <q>lvgl use touchpad indev
    #ifndef __LV_USE_TOUCHPAD_INDEV__
    #   define __LV_USE_TOUCHPAD_INDEV__   1
    #endif
    // <q>lvgl use encoder indev
    #ifndef __LV_USE_ENCODER_INDEV__
    #   define __LV_USE_ENCODER_INDEV__    0
    #endif
    // <o>lvgl default scene indev
    //     <0=>LV_INDEV_TOUCHPAD
    //     <1=>LV_INDEV_KEYPAD
    //     <2=>LV_INDEV_ENCODER
    #ifndef __LV_SCENE_DEFAULT_INDEV__
    #   define __LV_SCENE_DEFAULT_INDEV__  1
    #endif
    // <q>lvgl keypad indev helper functions
    #ifndef __LV_USE_KEYPAD_HELPER__
    #   define __LV_USE_KEYPAD_HELPER__    0
    #endif
    // <q>lvgl encoder indev helper functions
    #ifndef __LV_USE_ENCODER_HELPER__
    #   define __LV_USE_ENCODER_HELPER__   0
    #endif
// </h>

// <<< end of configuration section >>>

#endif /* __EMB_LV_CONF_H__ */
