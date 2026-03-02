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

#ifndef __EMB_LV_UTILS_H__
#define __EMB_LV_UTILS_H__

/*============================ INCLUDES ======================================*/
#include <stdbool.h>

#if defined(_WIN64)
#   include "lvgl/lvgl.h"
#   include "emb_lv_conf.h"
#else
#   include "lvgl.h"
#   include "lv_port_disp_template.h"
#   include "lv_port_indev_template.h"
#   if __LV_USE_FILE_SYSTEM__
#      include "lv_port_fs_template.h"
#   endif
#   ifdef __EMBEDDED_RAPID_DEVELOPMENT__
#       include "emb_utils.h"
#   elif defined(__EMB_USE_MID_LVGL__)
#       include "RTE_Components.h"
#   else
#       include "emb_lv_conf.h"
#   endif
#endif

#include "emb_lv_language.h"
#if defined(__EMB_USE_MID_LVGL__)
#   include "emb_lv_scene_declaration.h"
#endif

#ifdef   __cplusplus
extern "C" {
#endif
/*============================ MACROS ========================================*/

/*----------------------------------------------------------------------------*
 * Environment                                                                *
 *----------------------------------------------------------------------------*/
#define EMB_LV_PTR(x)    (void *)(uintptr_t)(x)

 /**
 * @brief lvgl indev macros
 */
#if defined(_WIN64)
#   define LV_INDEV_TOUCHPAD     mouse_indev
#   define LV_INDEV_KEYPAD       kb_indev
#   define LV_INDEV_ENCODER      enc_indev
extern lv_indev_t *mouse_indev;
extern lv_indev_t *kb_indev;
extern lv_indev_t *enc_indev;
#else
#   if __LV_USE_KEYPAD_INDEV__
#       define LV_INDEV_KEYPAD   indev_keypad
        extern lv_indev_t * indev_keypad;
#   endif
#   if __LV_USE_TOUCHPAD_INDEV__
#       define LV_INDEV_TOUCHPAD indev_touchpad
        extern lv_indev_t * indev_touchpad;
#   endif
#   if __LV_USE_ENCODER_INDEV__
#       define LV_INDEV_ENCODER  indev_encoder
        extern lv_indev_t * indev_encoder;
#   endif
#endif

#if defined(_WIN64)
#   define EMB_LOG_INFO(module, fmt, ...)                                      \
        do {                                                                   \
            printf("[%s][%s] " fmt "\n", #module, __FUNCTION__, ##__VA_ARGS__);\
        } while(0)
#   define EMB_LOG_WARNING                   EMB_LOG_INFO
#   define EMB_LOG_ERROR                     EMB_LOG_INFO
#else
#   ifndef EMB_LOG_INFO
#       define EMB_LOG_INFO(module, fmt, ...)
#   endif
#   ifndef EMB_LOG_WARNING
#       define EMB_LOG_WARNING(module, fmt, ...)
#   endif
#   ifndef EMB_LOG_ERROR
#       define EMB_LOG_ERROR(module, fmt, ...)
#   endif
#endif

#if defined(_WIN64)
#   undef assert
#   define assert(__COND)                             \
    do{                                               \
        if (!(__COND)) {                              \
            EMB_LOG_ERROR(                            \
                ASSERT,                               \
                "Assertion failed: parameter error"   \
            );                                        \
            while(1);                                 \
        }                                             \
    }while(0)
#else
#   ifndef assert
#       define assert(__COND)            
#   endif
#endif

#ifndef EMB_INVOKE_RT_VOID
#   define EMB_INVOKE_RT_VOID(__FUNC_PTR, ...)                           \
            do {                                                         \
                if (NULL != (__FUNC_PTR)) (*(__FUNC_PTR))(__VA_ARGS__);  \
                else {                                                   \
                    EMB_LOG_ERROR(                                       \
                        USER,                                            \
                        "function pointer is NULL, cannot invoke"        \
                    );                                                   \
                }                                                        \
            } while (0)
#endif

/*----------------------------------------------------------------------------*
 * Misc                                                                       *
 *----------------------------------------------------------------------------*/
/**
 * @brief lvgl colour macros
 */
#define rgb(r, g, b)        lv_color_make(r, g, b)
#if LV_COLOR_DEPTH == 32
#   define rgba(r, g, b, a) ((lv_color_t){.ch = {.red = (r), .green = (g), .blue = (b), .alpha = (a)}})
#   define argb(a, r, g, b) ((lv_color_t){.ch = {.alpha = (a), .red = (r), .green = (g), .blue = (b)}})
#endif

/*----------------------------------------------------------------------------*
 * Scene                                                                      *
 *----------------------------------------------------------------------------*/
#define EMB_SCENE_DECLARATION(WHICESCENE)                      \
        extern lv_scene_t *g_ptScene##WHICESCENE;                            
#define EMB_SCENE_QUOTE(WHICESCENE)                            \
        g_ptScene##WHICESCENE             

#define EMB_LV_ALL_SCENE_INIT()                                \
        do{                                                    \
            __EMB_LV_ALL_SCENE_INIT();                         \
        } while (0)


/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ PROTOTYPES ====================================*/
/*============================ IMPLEMENTATION ================================*/
/*============================ END ===========================================*/
#ifdef   __cplusplus
}
#endif
#endif /* __EMB_LV_UTILS_H__ */
