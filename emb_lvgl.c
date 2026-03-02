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
#define EMB_BTN_PRESSED   0
#define EMB_BTN_RELEASED  1

/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ LOCAL VARIABLES ===============================*/
#if __LV_USE_KEYPAD_HELPER__
    static const uint16_t **s_pphwLvglKeyValue;
    static const uint16_t  *s_phwKeyValueMap;
    static       uint16_t   s_hwKeyValueCount;
#endif

#if __LV_USE_ENCODER_HELPER__
    static uint32_t s_wEncoderEventCode;
    static int32_t s_i32EncoderDiff;
#endif
/*============================ PROTOTYPES ====================================*/
#if __LV_USE_KEYPAD_HELPER__
    extern 
    bool emb_btn_event_get(uint16_t *phwValue);
#endif

#if __LV_USE_ENCODER_HELPER__
    static void emb_lv_encoder_init(void);
    static void emb_lv_encoder_timer_cb(lv_timer_t *ptTimer);
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

#if __LV_USE_ENCODER_HELPER__
    emb_lv_encoder_init();
#endif
}

#if __LV_USE_KEYPAD_HELPER__
/*******************************************************************************
 * @brief Set the key map for LVGL
 * @param pphwLvglKeyValueMap Pointer to the LVGL key value map
 * @param phwKeyValueMap Pointer to the hardware key value map
 * @param hwKeyValueCount Number of key values in the map
 ******************************************************************************/
void emb_lvgl_set_key_map(const uint16_t **pphwLvglKeyValueMap, 
                          const uint16_t  *phwKeyValueMap,
                                uint16_t   hwKeyValueCount)
{
    emb_assert(NULL != pphwLvglKeyValueMap);
    emb_assert(NULL != phwKeyValueMap);
    emb_assert(0U   != hwKeyValueCount);

    s_pphwLvglKeyValue  = pphwLvglKeyValueMap;
    s_phwKeyValueMap    = phwKeyValueMap;
    s_hwKeyValueCount   = hwKeyValueCount;
}

/*******************************************************************************
 * @brief Read the keypad state and update the LVGL input data
 * @param indev_drv Pointer to the LVGL input device driver
 * @param data Pointer to the LVGL input data structure to update
 ******************************************************************************/
void emb_lv_keypad_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data)
{
    static uint16_t hwLastKeyValue = 0;
    static uint16_t hwLastKeyState = LV_INDEV_STATE_REL;

    uint16_t hwKeyValue = 0;
    if(!emb_btn_event_get(&hwKeyValue)) 
    {
        data->key   = hwLastKeyValue;
        data->state = hwLastKeyState;
        return;
    }

    do{
        uint16_t hwKeyState = (hwKeyValue & 0x00FFU);
        if(EMB_BTN_PRESSED == hwKeyState)
        {
            hwLastKeyState = LV_INDEV_STATE_PR;
        }
        else if(EMB_BTN_RELEASED == hwKeyState)
        {
            hwLastKeyState = LV_INDEV_STATE_REL;
        }
        data->state = hwLastKeyState;
    }while(0);

    do{
        bool bFound   = false;
        uint16_t hwKeyIndex = 0U;
        emb_lv_scene_mode_t tMode = emb_lv_scene_current_scene_mode_get();

        for(hwKeyIndex = 0U; hwKeyIndex < s_hwKeyValueCount; hwKeyIndex++)
        {
            if(s_phwKeyValueMap[hwKeyIndex] == hwKeyValue)
            {
                bFound = true;
                break;
            }
        }

        if(bFound && (NULL != s_pphwLvglKeyValue[tMode]))
        {
            hwKeyValue = s_pphwLvglKeyValue[tMode][hwKeyIndex];
            hwLastKeyValue = hwKeyValue;
        }
        data->key = hwLastKeyValue;
    }while(0);
}
#endif

#if __LV_USE_ENCODER_HELPER__
/*******************************************************************************
 * @brief Update the encoder difference value
 * @param i32Diff The difference value to add to the current encoder state
 ******************************************************************************/
void emb_lv_encoder_update(int32_t i32Diff)
{
    s_i32EncoderDiff += i32Diff;
}

/*******************************************************************************
 * @brief Get the custom event code for encoder events
 * @return The event code registered for encoder events
 ******************************************************************************/
uint32_t emb_lv_encoder_event_code_get(void)
{
    return s_wEncoderEventCode;
}
#endif
/*============================ LOCAL IMPLEMENTATION ==========================*/
#if __LV_USE_ENCODER_HELPER__
static void emb_lv_encoder_init(void)
{
    lv_timer_t *ptTimer = lv_timer_create(emb_lv_encoder_timer_cb, 50, NULL);
    s_wEncoderEventCode = lv_event_register_id();
}

static int32_t emb_lv_encoder_read(void)
{
    int32_t i32Diff = s_i32EncoderDiff;
    s_i32EncoderDiff = 0;
    return i32Diff;
}

static void emb_lv_encoder_timer_cb(lv_timer_t *ptTimer)
{
    int32_t i32Diff = emb_lv_encoder_read();
    if(i32Diff != 0)
    {
        lv_group_t *ptGroup = LV_INDEV_KEYPAD->group;
        lv_event_send(lv_group_get_focused(ptGroup), s_wEncoderEventCode, EMB_LV_PTR(i32Diff));
    }
}
#endif
/*============================ END ===========================================*/
