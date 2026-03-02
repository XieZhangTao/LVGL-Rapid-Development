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

#ifndef __EMB_LVGL_H__
#define __EMB_LVGL_H__

/*============================ INCLUDES ======================================*/
#include "emb_lv_scene.h"

#ifdef   __cplusplus
extern "C" {
#endif
/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ PROTOTYPES ====================================*/
/*============================ IMPLEMENTATION ================================*/
extern
void emb_lvgl_init(void);

#if __LV_USE_KEYPAD_HELPER__
/**
 * @brief Set the key map for LVGL
 * @param pphwLvglKeyValueMap Pointer to the LVGL key value map
 * @param phwKeyValueMap Pointer to the hardware key value map
 * @param hwKeyValueCount Number of key values in the map
 */
extern
void emb_lvgl_set_key_map(const uint16_t **pphwLvglKeyValueMap, 
                          const uint16_t  *phwKeyValueMap,
                                uint16_t   hwKeyValueCount);

/**
 * @brief Read the keypad state and update the LVGL input data
 * @param indev_drv Pointer to the LVGL input device driver
 * @param data Pointer to the LVGL input data structure to update
 */
extern
void emb_lv_keypad_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data);
#endif

#if __LV_USE_ENCODER_HELPER__
/**
 * @brief Update the encoder difference value
 * @param i32Diff The difference value to add to the current encoder state
 */
extern
void emb_lv_encoder_update(int32_t i32Diff);

/**
 * @brief Get the custom event code for encoder events
 * @return The event code registered for encoder events
 */
extern
uint32_t emb_lv_encoder_event_code_get(void);

#endif
/*============================ LOCAL IMPLEMENTATION ==========================*/
/*============================ END ===========================================*/
#ifdef   __cplusplus
}
#endif
#endif /* __EMB_LVGL_H__ */
