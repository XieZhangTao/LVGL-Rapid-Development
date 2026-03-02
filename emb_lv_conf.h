/****************************************************************************
*  Copyright 2025 xzt (Email:genuine2001@163.com)                           *
*                                                                           *
*  Licensed under the Apache License, Version 2.0 (the "License");          *
*  you may not use this file except in compliance with the License.         *
*  You may obtain a copy of the License at                                  *
*                                                                           *
*     http://www.apache.org/licenses/LICENSE-2.0                            *
*                                                                           *
*  Unless required by applicable law or agreed to in writing, software      *
*  distributed under the License is distributed on an "AS IS" BASIS,        *
*  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. *
*  See the License for the specific language governing permissions and      *
*  limitations under the License.                                           *
*                                                                           *
****************************************************************************/

#ifndef __EMB_LV_CONF_H__
#define __EMB_LV_CONF_H__

// <<< Use Configuration Wizard in Context Menu >>>

// <q>lvgl use file system
#ifndef __LV_USE_FILE_SYSTEM__
#   define __LV_USE_FILE_SYSTEM__          1
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
    #   define __LV_USE_ENCODER_INDEV__    1
    #endif
    // <o>lvgl default scene indev
    //     <0=>LV_INDEV_TOUCHPAD
    //     <1=>LV_INDEV_KEYPAD
    //     <2=>LV_INDEV_ENCODER
    #ifndef __LV_SCENE_DEFAULT_INDEV__
    #   define __LV_SCENE_DEFAULT_INDEV__  1
    #endif
// </h>

// <<< end of configuration section >>>

#endif /* __EMB_LV_CONF_H__ */
