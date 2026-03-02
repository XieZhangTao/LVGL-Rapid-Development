/****************************************************************************
*  Copyright 2025 xzt (Email:genuine2001@163.com)                           *
*                                                                           *
*  Licensed under the Apache License, Version 2.0 (the "License");          *
*  you may not use this file except in compliance with the License.         *
*  You may obtain a copy of the License at                                  *
*                                                                           *
*     http://www.apache.org/licenses/LICENSE-2.0                            *
*                                                                           *
*
*  Unless required by applicable law or agreed to in writing, software      *
*  distributed under the License is distributed on an "AS IS" BASIS,        *
*  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. *
*  See the License for the specific language governing permissions and      *
*  limitations under the License.                                           *
*                                                                           *
****************************************************************************/

/*============================ INCLUDES ======================================*/
#include "emb_lvgl.h"
/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ GLOBAL VARIABLES ==============================*/
#ifdef _WIN64
#   define _WIN_STATIC  const
#else
#   define _WIN_STATIC  static const
#endif
/*============================ LOCAL VARIABLES ===============================*/
/*============================ PROTOTYPES ====================================*/
#ifndef _WIN64
    extern 
    uint8_t emb_read_key_value(void);
#endif
/*============================ IMPLEMENTATION ================================*/
void emb_lvgl_init(void)
{
#ifndef _WIN64
    lv_init();
    lv_port_disp_init(); 
    lv_port_indev_init();
#   if __LV_USE_FILE_SYSTEM__
    lv_port_fs_init();
#   endif
#endif
    emb_lv_scene_manage_init();
#ifdef __EMB_USE_MID_LVGL__
    EMB_LV_ALL_SCENE_INIT();
#endif
}

_WIN_STATIC uint8_t chLvglKeyValue[][5] = {
    {LV_KEY_PREV, LV_KEY_NEXT, LV_KEY_ENTER, LV_KEY_ESC, LV_KEY_HOME},
    {LV_KEY_DOWN, LV_KEY_UP  , LV_KEY_ENTER, LV_KEY_ESC, LV_KEY_HOME},
};
#ifndef _WIN64
void emb_lv_keypad_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data)
{
    static uint8_t chLastKeyValue = 0;
    emb_lv_scene_mode_t tMode = emb_lv_scene_get_current_scene_mode();
    uint8_t chKeyValue = emb_read_key_value();
    if( 0 == chKeyValue )
    {
        data->state = LV_INDEV_STATE_REL;
    }
    else 
    {
        data->state = LV_INDEV_STATE_PR;
        switch(chKeyValue)
        {
            case 1: chKeyValue = chLvglKeyValue[tMode][0]; break;
            case 2: chKeyValue = chLvglKeyValue[tMode][1]; break;
            case 3: chKeyValue = chLvglKeyValue[tMode][2]; break;
            case 4: chKeyValue = chLvglKeyValue[tMode][3]; break;
            case 5: chKeyValue = chLvglKeyValue[tMode][4]; break;
        }
        chLastKeyValue = chKeyValue;
    }
    data->key = chLastKeyValue;
}
#endif
/*============================ END ===========================================*/
