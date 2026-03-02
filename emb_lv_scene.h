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

#ifndef __EMB_LV_SCENE_H__
#define __EMB_LV_SCENE_H__

/*============================ INCLUDES ======================================*/
#include "emb_lv_utils.h"
#if defined(__EMBEDDED_RAPID_DEVELOPMENT__)
#   include "emb_list.h"
#else
#   include "emb_lv_list.h"
#endif

#ifdef   __cplusplus
extern "C" {
#endif
/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
typedef enum{
    EMB_LV_SCENE_NAVIGATION = 0,
    EMB_LV_SCENE_EDIT       = 1,
} emb_lv_scene_mode_t;

typedef struct { 
    uint32_t       u4AnimType   : 4 ; /* Animation type                  */
    uint32_t       u14AnimTime  : 14; /* Animation time in milliseconds  */
    uint32_t       u14AnimDelay : 14; /* Animation delay in milliseconds */
} lv_scene_switch_anim_t;

typedef struct{ 
    void (*pfBeforeScene)(void);      /* Callback function before scene create     */
    void (*pfSceneCreate)(void);      /* Scene create callback function            */
    void (*pfSceneUpdate)(void);      /* Scene update callback function            */
    void (*pfAfterSceneDelete)(void); /* Scene delete callback function      
                                         Note : For the components bound to Screen,
                                         you don't need to delete them here. The 
                                         background will handle them automatically */
} lv_scene_cfg_t;
    
typedef struct{
    lv_group_t     *ptGroup           ; /* Scene group objects */
    lv_timer_t     *ptTimer           ; /* Scene timer object  */
} lv_scene_ext_t;

typedef struct {
    lv_obj_t        *ptScreen         ; /* Scene screen object  */
    lv_scene_cfg_t   tCfg             ; /* Scene configuration  */
    emb_list_t       tListNode        ; /* Scene list node      */ 
    uint8_t          chFocusIndex  [4]; /* Focus object index   */
    uint32_t         u8SceneID    : 8 ; /* Scene ID             */
    uint32_t         u8FocusTier  : 8 ; /* Focus tier index     */
    uint32_t         bIsWorking   : 1 ; /* Is working           */
    uint32_t         bNeedFocus   : 1 ; /* Needs to be focused  */
    uint32_t         bIsEdit      : 1 ; /* Is in edit mode      */
    uint32_t         bHasExtData  : 1 ; /* Has extension data   */
    uint32_t                      : 12; /* Reserved bits        */
    lv_scene_ext_t  *ptSceneEX[]      ; /* Scene extension data */
} lv_scene_t;
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ PROTOTYPES ====================================*/
/*============================ IMPLEMENTATION ================================*/
/**
 * @brief Initialize the scene management module
 */
extern
void emb_lv_scene_manage_init(void);

/**
 * @brief Set the scene switch animation parameters
 * @param ptAnim Pointer to the animation parameters
 */
extern
void emb_lv_scene_set_switch_anim(lv_scene_switch_anim_t *ptAnim);

/**
 * @brief Create a new scene
 * @param chSceneID Scene ID
 * @param ptCfg Pointer to the scene configuration
 * @return Pointer to the created scene, or NULL on failure
 */
extern
lv_scene_t* emb_lv_scene_create(uint8_t chSceneID, lv_scene_cfg_t *ptCfg);

/**
 * @brief Create a new scene with extension data
 * @param chSceneID Scene ID
 * @param ptCfg Pointer to the scene configuration
 * @param ptSceneEX Pointer to the scene extension data
 * @return Pointer to the created scene, or NULL on failure
 */
extern
lv_scene_t *emb_lv_scene_create_with_extend(uint8_t u8SceneID, 
                                            lv_scene_cfg_t *ptCfg, 
                                            lv_scene_ext_t *ptSceneEX);

/**
 * @brief Append a scene to the scene list
 * @param ptScene Pointer to the scene to append
 */
extern
void emb_lv_scene_append_to_list(lv_scene_t *ptScene);

/**
 * @brief Remove a scene from the scene list
 * @param ptScene Pointer to the scene to remove
 */
extern
void emb_lv_scene_remove_from_list(lv_scene_t *ptScene);

/**
 * @brief Load the current scene
 */
extern
void emb_lv_scene_load_current_scene(void);

/**
 * @brief Load the current scene and delete the previous one
 */
extern
void emb_lv_scene_load_current_scene_with_del_previous(void);

/**
 * @brief Go back to the previous scene
 * @note This function will free the current scene.
 */
extern
void emb_lv_scene_back_previous(void);

/**
 * @brief Get the current scene
 * @return Pointer to the current scene, or NULL if no scene is available
 */
extern 
lv_scene_t* emb_lv_scene_get_current_scene_struct(void);

/**
 * @brief Get the current scene ID
 * @return Current scene ID, or -1 if no scene is available
 */
extern
int32_t emb_lv_scene_get_current_scene_id(void);

extern
void emb_lv_scene_set_current_scene_mode(emb_lv_scene_mode_t tMode);

extern
emb_lv_scene_mode_t emb_lv_scene_get_current_scene_mode(void);
/*============================ END ===========================================*/
#ifdef   __cplusplus
}
#endif
#endif /* __EMB_LV_SCENE_H__ */
