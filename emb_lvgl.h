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
void emb_lv_keypad_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data);
#endif
/*============================ END ===========================================*/
#ifdef   __cplusplus
}
#endif
#endif /* __EMB_LVGL_H__ */
