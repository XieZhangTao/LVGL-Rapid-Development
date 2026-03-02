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

#ifndef __EMB_LV_LANGUAGE_H__
#define __EMB_LV_LANGUAGE_H__

/*============================ INCLUDES ======================================*/

#ifdef   __cplusplus
extern "C" {
#endif
/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
typedef enum{
    EMB_LV_LANGUAGE_CN = 0, /* Simplified Chinese   */
    EMB_LV_LANGUAGE_TC = 1, /* Traditional Chinese  */
    EMB_LV_LANGUAGE_EN = 2, /* English              */
    EMB_LV_LANGUAGE_JP = 3, /* Japanese             */
    EMB_LV_LANGUAGE_RU = 4  /* Russian              */
} emb_lv_language_t; 

/*============================ GLOBAL VARIABLES ==============================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ PROTOTYPES ====================================*/
/**
 * @brief Set the current language
 * @param tLanguage The language to set
 */
extern
void emb_lv_language_set(emb_lv_language_t eLanguage);

/**
 * @brief Get the current language
 * @return The current language
 */
extern
emb_lv_language_t emb_lv_language_get(void);
/*============================ IMPLEMENTATION ================================*/
/*============================ LOCAL IMPLEMENTATION ==========================*/
/*============================ END ===========================================*/
#ifdef   __cplusplus
}
#endif
#endif /* __EMB_LV_LANGUAGE_H__ */
