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

#ifndef _WIN64
extern
/**
 * @brief Configure the key mapping table (fixed 2 rows, variable columns).
 *
 * @details
 * - pphwLvglKeyMap is a double pointer for "mode -> LVGL key code table".
 * - The first dimension is the scene mode index
 *   (current implementation uses 2 fixed rows, usually 2 modes).
 * - The second dimension is the key index (range: 0 ~ hwLvglKeyCount - 1).
 * - phwKeyValueMap maps raw hardware key values to the key index:
 *   when a hardware key value hwKeyValue is read, the code searches
 *   phwKeyValueMap sequentially, and the matched position i is used as
 *   the column index of pphwLvglKeyMap[mode][i].
 *
 * @param pphwLvglKeyMap  Double-pointer mapping table, must not be NULL;
 *                        each row pointer should also be valid.
 * @param phwKeyValueMap  Hardware key lookup table, must not be NULL,
 *                        with length hwLvglKeyCount.
 * @param hwLvglKeyCount  Number of valid columns per row (also the element
 *                        count of phwKeyValueMap), must be greater than 0.
 *
 * @note
 * - This function only stores pointers and does not copy data.
 *   Ensure input arrays remain valid during use.
 * - It is recommended to call this before input reading starts
 *   (for example, during system initialization).
 */
void emb_lvgl_set_key_map(const uint16_t **pphwLvglKeyMap, 
                          const uint16_t  *phwKeyValueMap,
                                uint16_t   hwLvglKeyCount);

extern
/**
 * @brief LVGL keypad input callback.
 *
 * @details
 * - Calls emb_read_key_value() to read the raw hardware key value.
 * - If no key is pressed: sets data->state = LV_INDEV_STATE_REL and
 *   keeps the previous key value.
 * - If a key is pressed: sets data->state = LV_INDEV_STATE_PR, then
 *   searches phwKeyValueMap for index i and outputs
 *   pphwLvglKeyMap[mode][i].
 * - The resulting key code is written to data->key for LVGL focus
 *   navigation and event processing.
 *
 * @param indev_drv LVGL input device driver pointer
 *                  (not directly used in current implementation).
 * @param data      LVGL input data structure; this function updates
 *                  its state and key fields.
 */
void emb_lv_keypad_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data);
#endif
/*============================ END ===========================================*/
#ifdef   __cplusplus
}
#endif
#endif /* __EMB_LVGL_H__ */
