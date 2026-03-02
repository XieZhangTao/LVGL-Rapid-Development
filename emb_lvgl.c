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
#define EMB_LVGL_KEY_MODE_COUNT    2U
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ LOCAL VARIABLES ===============================*/
static const uint16_t **s_pphwLvglKeyValue;
static const uint16_t  *s_phwKeyValueMap;
static       uint16_t   s_hwLvglKeyCount;
/*============================ PROTOTYPES ====================================*/
#ifndef _WIN64
    extern 
    bool emb_read_key_value(uint16_t *pchKeyValue);
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

#ifndef _WIN64
void emb_lvgl_set_key_map(const uint16_t **pphwLvglKeyMap, 
                          const uint16_t  *phwKeyValueMap,
                                uint16_t   hwLvglKeyCount)
{
    if((NULL == pphwLvglKeyMap) || (NULL == phwKeyValueMap) || (0U == hwLvglKeyCount))
    {
        return;
    }
    s_pphwLvglKeyValue  = pphwLvglKeyMap;
    s_phwKeyValueMap    = phwKeyValueMap;
    s_hwLvglKeyCount    = hwLvglKeyCount;
}

void emb_lv_keypad_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data)
{
    static uint16_t hwLastKeyValue = 0;
    uint16_t hwKeyValue = 0;
   
    if(false == emb_read_key_value(&hwKeyValue))
    {
        data->state = LV_INDEV_STATE_REL;
    }
    else 
    {
        bool bMapped = false;

        if((NULL != s_pphwLvglKeyValue) && (NULL != s_phwKeyValueMap) && (0U != s_hwLvglKeyCount))
        {
            emb_lv_scene_mode_t tMode = emb_lv_scene_get_current_scene_mode();
            uint16_t hwKeyIndex = 0U;
            uint16_t hwModeIndex = (uint16_t)tMode;
            bool bFound = false;

            if(hwModeIndex >= EMB_LVGL_KEY_MODE_COUNT)
            {
                hwModeIndex = 0U;
            }

            for(hwKeyIndex = 0U; hwKeyIndex < s_hwLvglKeyCount; hwKeyIndex++)
            {
                if(s_phwKeyValueMap[hwKeyIndex] == hwKeyValue)
                {
                    bFound = true;
                    break;
                }
            }

            if(bFound && (NULL != s_pphwLvglKeyValue[hwModeIndex]))
            {
                hwKeyValue = s_pphwLvglKeyValue[hwModeIndex][hwKeyIndex];
                bMapped = true;
            }
        }

        if(bMapped)
        {
            data->state = LV_INDEV_STATE_PR;
            hwLastKeyValue = hwKeyValue;
        }
        else
        {
            data->state = LV_INDEV_STATE_REL;
        }
    }
    data->key = hwLastKeyValue;
}
#endif
/*============================ END ===========================================*/
