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
#include "emb_lv_scene.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
#define emb_lv_scene_current_scene_get()                        \
    list_entry(tSceneManage.tSceneList.ptNext, emb_lv_scene_t, tListNode)
#define emb_lv_scene_previous_scene_get()                       \
    list_entry(tSceneManage.tSceneList.ptNext->ptNext, emb_lv_scene_t, tListNode)

#define emb_lv_scene_extend_group_free(_ptScene)                \
    do {                                                        \
        lv_group_del(_ptScene->ptSceneEX[0]->ptGroup);          \
    } while (0)    
#define emb_lv_scene_extend_timer_free(_ptScene)                \
    do {                                                        \
        lv_timer_del(_ptScene->ptSceneEX[0]->ptTimer);          \
    } while (0)

#if __LV_SCENE_DEFAULT_INDEV__   == 0
#   define EMB_LV_SCENE_DEFAULT_INDEV  LV_INDEV_TOUCHPAD
#elif __LV_SCENE_DEFAULT_INDEV__ == 1
#   define EMB_LV_SCENE_DEFAULT_INDEV  LV_INDEV_KEYPAD
#elif __LV_SCENE_DEFAULT_INDEV__ == 2
#   define EMB_LV_SCENE_DEFAULT_INDEV  LV_INDEV_ENCODER
#endif

#define emb_lv_scene_indev_group_set(_ptGroup)                  \
    do {                                                        \
        if((_ptGroup) != NULL) {                                \
            lv_indev_set_group(EMB_LV_SCENE_DEFAULT_INDEV,      \
                               (_ptGroup));                     \
            lv_indev_wait_release(EMB_LV_SCENE_DEFAULT_INDEV);  \
        }                                                       \
    } while (0)

/*============================ TYPES =========================================*/
typedef struct {
    emb_list_t                 tSceneList;  /* Scene list                        */
    emb_lv_scene_switch_anim_t tSwitchAnim; /* Scene switch animation parameters */
} emb_lv_scene_manage_t;
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ LOCAL VARIABLES ===============================*/
static emb_lv_scene_manage_t tSceneManage;

/*============================ PROTOTYPES ====================================*/
static void emb_lv_scene_extend_handle(emb_lv_scene_t *ptScene);
static void emb_lv_scene_extend_free(emb_lv_scene_t *ptScene);

/*============================ IMPLEMENTATION ================================*/

/*******************************************************************************
* @brief Initialize the scene management module
*******************************************************************************/
void emb_lv_scene_manage_init(void)
{
    memset(&tSceneManage, 0, sizeof(emb_lv_scene_manage_t));
    emb_list_init(&tSceneManage.tSceneList);
    EMB_LOG_INFO(
        USER,
        "initialized successfully"
    );
}

/*******************************************************************************
 * @brief Set the scene switch animation parameters
 * @param ptAnim Pointer to the animation parameters
*******************************************************************************/
void emb_lv_scene_switch_anim_set(emb_lv_scene_switch_anim_t *ptAnim)
{
    emb_assert(ptAnim != NULL);
    tSceneManage.tSwitchAnim = *ptAnim;
    EMB_LOG_INFO(
        USER,
        "set scene switch animation success, type: %d, time: %d ms, delay: %d ms",
         tSceneManage.tSwitchAnim.u4AnimType,
         tSceneManage.tSwitchAnim.u14AnimTime,
         tSceneManage.tSwitchAnim.u14AnimDelay
    );
}

/*******************************************************************************
 * @brief Create a new scene
 * @param u8SceneID Scene ID
 * @param ptCfg Pointer to the scene configuration
 * @return Pointer to the created scene, or NULL on failure
*******************************************************************************/
emb_lv_scene_t* emb_lv_scene_create(uint8_t u8SceneID, emb_lv_scene_cfg_t *ptCfg)
{
    emb_assert(ptCfg != NULL);

    /*	step1,	initialize the scene configuration */
    emb_lv_scene_t *ptScene  = lv_mem_alloc(sizeof(emb_lv_scene_t));
    lv_obj_t       *ptScreen = lv_obj_create(NULL);
    emb_assert(ptScene != NULL && ptScreen != NULL);
    memset(ptScene, 0, sizeof(emb_lv_scene_t));
    ptScene->ptScreen    = ptScreen;
    ptScene->u8SceneID   = u8SceneID;
    ptScene->tCfg        = *ptCfg;
    ptScene->bHasExtData = false;

    return ptScene;
}

/*******************************************************************************
 * @brief Create a new scene with extension data
 * @param u8SceneID Scene ID
 * @param ptCfg Pointer to the scene configuration
 * @param ptSceneEX Pointer to the scene extension data
 * @return Pointer to the created scene, or NULL on failure
*******************************************************************************/
emb_lv_scene_t *emb_lv_scene_create_with_extend(uint8_t u8SceneID, 
                                                emb_lv_scene_cfg_t *ptCfg, 
                                                emb_lv_scene_ext_t *ptSceneEX)
{
    emb_assert(ptCfg != NULL && ptSceneEX != NULL);

    /*	step1,	create the scene */
    emb_lv_scene_t *ptScene   = lv_mem_alloc(sizeof(emb_lv_scene_t) + sizeof(emb_lv_scene_ext_t *));
    lv_obj_t       *ptScreen  = lv_obj_create(NULL);
    emb_assert(ptScene   != NULL && ptScreen != NULL);
    memset(ptScene, 0, sizeof(emb_lv_scene_t) + sizeof(emb_lv_scene_ext_t *));
    ptScene->ptScreen     = ptScreen;
    ptScene->u8SceneID    = u8SceneID;
    ptScene->tCfg         = *ptCfg;
    ptScene->ptSceneEX[0] = ptSceneEX;
    ptScene->bHasExtData  = true;

    return ptScene;
}

/*******************************************************************************
 * @brief Append a scene to the scene list
 * @param ptScene Pointer to the scene to append
*******************************************************************************/
void emb_lv_scene_list_append(emb_lv_scene_t *ptScene)
{
    emb_assert(ptScene != NULL);

    /*	step1,	add the scene to the list */
    emb_lv_scene_t *ptThis = ptScene;
    emb_list_node_insert_head(&(ptThis->tListNode), &tSceneManage.tSceneList);
}

/*******************************************************************************
 * @brief Remove a scene from the scene list
 * @param ptScene Pointer to the scene to remove
*******************************************************************************/
void emb_lv_scene_list_remove(emb_lv_scene_t *ptScene)
{
    emb_assert(ptScene != NULL);

    /*	step1,	remove the scene from the list */
    emb_lv_scene_t *ptThis = ptScene;
    emb_list_node_delete(&(ptThis->tListNode));
}

/*******************************************************************************
 * @brief Load the current scene
*******************************************************************************/
void emb_lv_scene_current_scene_load(void)
{
    /*	step1,	check the scene list is empty */
    if(emb_list_is_empty(&tSceneManage.tSceneList)) {
        EMB_LOG_WARNING(
            USER,
            "no scene to load"
        );
        return;
    }
    
    /*	step2,	get the current scene */
    emb_lv_scene_t *ptScene = emb_lv_scene_current_scene_get();

    /*	step3,	perform the scene switch operation */
    if(ptScene->ptScreen == NULL) {
        ptScene->ptScreen = lv_obj_create(NULL);
    }
    EMB_INVOKE_RT_VOID(ptScene->tCfg.pfBeforeScene);
    EMB_INVOKE_RT_VOID(ptScene->tCfg.pfSceneCreate);
    EMB_INVOKE_RT_VOID(ptScene->tCfg.pfSceneUpdate);
    lv_scr_load_anim(
        ptScene->ptScreen,
        (lv_scr_load_anim_t)tSceneManage.tSwitchAnim.u4AnimType,
        tSceneManage.tSwitchAnim.u14AnimTime,
        tSceneManage.tSwitchAnim.u14AnimDelay,
        false
    );
    emb_lv_scene_current_scene_mode_set((emb_lv_scene_mode_t)ptScene->bIsEdit);

    /*	step4,	handle the current scene extension data */
    if(!ptScene->bHasExtData) return;
    emb_lv_scene_indev_group_set(ptScene->ptSceneEX[0]->ptGroup);
}

/*******************************************************************************
 * @brief Reload the current scene
*******************************************************************************/
void emb_lv_scene_current_scene_reload(void)
{
    /*	step1,	check the scene list is empty */
    if(emb_list_is_empty(&tSceneManage.tSceneList)) {
        EMB_LOG_WARNING(
            USER,
            "no scene to reload"
        );
        return;
    }

    /*	step2,	get the current scene */
    emb_lv_scene_t *ptScene = emb_lv_scene_current_scene_get();

    /*	step3,	handle the current scene before reload */
    EMB_INVOKE_RT_VOID(ptScene->tCfg.pfAfterSceneDelete);
    do{
        if(!ptScene->bHasExtData) break;
        if(ptScene->ptSceneEX[0]->ptGroup != NULL && ptScene->bAutoFocus)
        {
            lv_group_t *ptGroup    = ptScene->ptSceneEX[0]->ptGroup;
            lv_obj_t   *ptFocusObj = lv_group_get_focused(ptGroup);
            emb_assert(ptFocusObj != NULL); // focus object must be valid
            uint8_t chFocusTier = 0;
            while(ptFocusObj != ptScene->ptScreen)
            {
                ptScene->chFocusIndex[chFocusTier++] = lv_obj_get_index(ptFocusObj);
                ptFocusObj = lv_obj_get_parent(ptFocusObj);
                if(chFocusTier >= sizeof(ptScene->chFocusIndex)) {
                    EMB_LOG_WARNING(
                        USER,
                        "focus tier exceed the maximum supported"
                    );
                    break;
                }
            }
            ptScene->u8FocusTier = chFocusTier;
            ptScene->bNeedFocus  = true;
        }
        else {
            ptScene->bNeedFocus = false;
        }

        /*	record the working state */
        if(ptScene->ptSceneEX[0]->ptTimer != NULL)
        {
            bool bIsTimerPaused = ptScene->ptSceneEX[0]->ptTimer->paused;
            ptScene->bIsWorking = !bIsTimerPaused;
        }
        else {
            ptScene->bIsWorking = false;
        }

        emb_lv_scene_extend_free(ptScene);
    } while(0);

    /*	step4,	perform the scene reload operation */
    ptScene->ptScreen = lv_obj_create(NULL);
    EMB_INVOKE_RT_VOID(ptScene->tCfg.pfBeforeScene);
    EMB_INVOKE_RT_VOID(ptScene->tCfg.pfSceneCreate);
    EMB_INVOKE_RT_VOID(ptScene->tCfg.pfSceneUpdate);
    lv_scr_load_anim(
        ptScene->ptScreen,
        (lv_scr_load_anim_t)tSceneManage.tSwitchAnim.u4AnimType,
        tSceneManage.tSwitchAnim.u14AnimTime,
        tSceneManage.tSwitchAnim.u14AnimDelay,
        true
    );
    emb_lv_scene_current_scene_mode_set((emb_lv_scene_mode_t)ptScene->bIsEdit);

    /*	step5,	handle the current scene extension data */
    if(!ptScene->bHasExtData) return;
    emb_lv_scene_indev_group_set(ptScene->ptSceneEX[0]->ptGroup);
    emb_lv_scene_extend_handle(ptScene);
}

/*******************************************************************************
 * @brief Load the current scene and delete the previous one
 * @note This function will also handle the focus object and working
 *       state of the previous scene.
*******************************************************************************/
void emb_lv_scene_current_scene_load_with_del_previous(void)
{
    /*	step1,	handle the previous scene */
    emb_lv_scene_t *ptPreviousScene = emb_lv_scene_previous_scene_get();
    do{
        if(&(ptPreviousScene->tListNode) == &tSceneManage.tSceneList) {
            EMB_LOG_WARNING(
                USER,
                "no previous scene to delete"
            );
            return;
        }

        /*	step1.1,   invoke scene delete callback */
        EMB_INVOKE_RT_VOID(ptPreviousScene->tCfg.pfAfterSceneDelete);

        /*	step1.2,   record the extend data state */
        if(!ptPreviousScene->bHasExtData) break;
        if(ptPreviousScene->ptSceneEX[0]->ptGroup != NULL && ptPreviousScene->bAutoFocus)
        {
            lv_group_t *ptGroup    = ptPreviousScene->ptSceneEX[0]->ptGroup;
            lv_obj_t   *ptFocusObj = lv_group_get_focused(ptGroup);
            emb_assert(ptFocusObj != NULL); // focus object must be valid
            uint8_t chFocusTier = 0;
            while(ptFocusObj != ptPreviousScene->ptScreen) 
            {
                ptPreviousScene->chFocusIndex[chFocusTier++] = lv_obj_get_index(ptFocusObj);
                ptFocusObj = lv_obj_get_parent(ptFocusObj);
                if(chFocusTier >= sizeof(ptPreviousScene->chFocusIndex)) {
                    EMB_LOG_WARNING(
                        USER,
                        "focus tier exceed the maximum supported"
                    );
                    break;
                }
            }
            ptPreviousScene->u8FocusTier = chFocusTier;
            ptPreviousScene->bNeedFocus = true;
        }
        else {
            ptPreviousScene->bNeedFocus = false;
        }

        /*	record the working state */
        if(ptPreviousScene->ptSceneEX[0]->ptTimer != NULL)
        {
            bool bIsTimerPaused = ptPreviousScene->ptSceneEX[0]->ptTimer->paused;
            ptPreviousScene->bIsWorking = !bIsTimerPaused;
        } 
        else {
            ptPreviousScene->bIsWorking = false;
        }

        /*	step1.3,   free the previous scene extension data */
        emb_lv_scene_extend_free(ptPreviousScene);

    }while(0);

    /*	step2,	get the current scene */
    emb_lv_scene_t *ptScene = emb_lv_scene_current_scene_get();

    /*	step3,	perform the scene switch operation */
    ptScene->ptScreen = lv_obj_create(NULL);
    EMB_INVOKE_RT_VOID(ptScene->tCfg.pfBeforeScene);
    EMB_INVOKE_RT_VOID(ptScene->tCfg.pfSceneCreate);
    EMB_INVOKE_RT_VOID(ptScene->tCfg.pfSceneUpdate);
    lv_scr_load_anim(
        ptScene->ptScreen,
        (lv_scr_load_anim_t)tSceneManage.tSwitchAnim.u4AnimType,
        tSceneManage.tSwitchAnim.u14AnimTime,
        tSceneManage.tSwitchAnim.u14AnimDelay,
        true
    );
    ptPreviousScene->ptScreen = NULL; 

    /*	step4,	handle the current scene extension data */
    if(!ptScene->bHasExtData) return;
    emb_lv_scene_indev_group_set(ptScene->ptSceneEX[0]->ptGroup);
}

/*******************************************************************************
 * @brief Return to the previous scene
 * @note This function will release the current scene, then create
 *       and load the previous scene, and finally restore the
 *       previous scene extension state.
*******************************************************************************/
void emb_lv_scene_previous_scene_back(void)
{
    /*	step1,	get the previous scene */
    emb_lv_scene_t *ptPreviousScene = emb_lv_scene_previous_scene_get();
    if(&(ptPreviousScene->tListNode) == &tSceneManage.tSceneList) {
        EMB_LOG_WARNING(
            USER,
            "no previous scene to return to"
        );
        return;
    }

    /*	step2,	handle the current scene */
    emb_lv_scene_t *ptCurrentScene = emb_lv_scene_current_scene_get();
    EMB_INVOKE_RT_VOID(ptCurrentScene->tCfg.pfAfterSceneDelete);
    emb_lv_scene_extend_free(ptCurrentScene);
    emb_lv_scene_list_remove(ptCurrentScene);

    /*	step3,	load the previous scene */
    ptPreviousScene->ptScreen = lv_obj_create(NULL);
    EMB_INVOKE_RT_VOID(ptPreviousScene->tCfg.pfBeforeScene);
    EMB_INVOKE_RT_VOID(ptPreviousScene->tCfg.pfSceneCreate);
    EMB_INVOKE_RT_VOID(ptPreviousScene->tCfg.pfSceneUpdate);
    lv_scr_load_anim(
        ptPreviousScene->ptScreen,
        (lv_scr_load_anim_t)tSceneManage.tSwitchAnim.u4AnimType,
        tSceneManage.tSwitchAnim.u14AnimTime,
        tSceneManage.tSwitchAnim.u14AnimDelay,
        true
    );
    ptCurrentScene->ptScreen = NULL;

    /*	step4,	handle the previous scene extension data */
    if(!ptPreviousScene->bHasExtData) return;
    emb_lv_scene_indev_group_set(ptPreviousScene->ptSceneEX[0]->ptGroup);
    emb_lv_scene_extend_handle(ptPreviousScene);
}

/*******************************************************************************
 * @brief Restore to the previous scene
 * @note This function will release the current scene and only load
 *       the existing previous scene screen.
 *       It will not recreate scene UI callbacks and will not
 *       restore previous scene extension state.
 ******************************************************************************/
void emb_lv_scene_previous_scene_restore(void)
{
    emb_lv_scene_t *ptPreviousScene = emb_lv_scene_previous_scene_get();
    if(&(ptPreviousScene->tListNode) == &tSceneManage.tSceneList) {
        EMB_LOG_WARNING(
            USER,
            "no previous scene to return to"
        );
        return;
    }

    /*	step2,	handle the current scene */
    emb_lv_scene_t *ptCurrentScene = emb_lv_scene_current_scene_get();
    EMB_INVOKE_RT_VOID(ptCurrentScene->tCfg.pfAfterSceneDelete);
    emb_lv_scene_extend_free(ptCurrentScene);
    emb_lv_scene_list_remove(ptCurrentScene);

    /*	step3,	load the previous scene */
    lv_scr_load_anim(
        ptPreviousScene->ptScreen,
        (lv_scr_load_anim_t)tSceneManage.tSwitchAnim.u4AnimType,
        tSceneManage.tSwitchAnim.u14AnimTime,
        tSceneManage.tSwitchAnim.u14AnimDelay,
        true
    );
    ptCurrentScene->ptScreen = NULL;
}

/*******************************************************************************
 * @brief Get the current scene
 * @return Pointer to the current scene, or NULL if no scene is available
*******************************************************************************/
emb_lv_scene_t* emb_lv_scene_current_scene_struct_get(void)
{
    if(emb_list_is_empty(&tSceneManage.tSceneList)) {
        return NULL;
    }
    return emb_lv_scene_current_scene_get();
}

/*******************************************************************************
 * @brief Get the current scene ID
 * @return Current scene ID, or -1 if no scene is available
*******************************************************************************/
int32_t emb_lv_scene_current_scene_id_get(void)
{
    if(emb_list_is_empty(&tSceneManage.tSceneList)) {
        return -1;
    }
    emb_lv_scene_t *ptScene = emb_lv_scene_current_scene_get();
    return ptScene->u8SceneID;
}

/*******************************************************************************
 * @brief Set the current scene mode
 * @param tMode Scene mode to set (navigation or edit)
*******************************************************************************/
void emb_lv_scene_current_scene_mode_set(emb_lv_scene_mode_t tMode)
{
    emb_lv_scene_t *ptScene = emb_lv_scene_current_scene_get();
    emb_assert(ptScene != NULL); /* should never be NULL */
    ptScene->bIsEdit = tMode;
}

/*******************************************************************************
 * @brief Get the current scene mode
 * @return Current scene mode (navigation or edit)
*******************************************************************************/
emb_lv_scene_mode_t emb_lv_scene_current_scene_mode_get(void)
{
    emb_lv_scene_t *ptScene = emb_lv_scene_current_scene_get();
    emb_assert(ptScene != NULL); /* should never be NULL */
    return (emb_lv_scene_mode_t)ptScene->bIsEdit;
}

/*============================ LOCAL IMPLEMENTATION ==========================*/
static void emb_lv_scene_extend_handle(emb_lv_scene_t *ptScene)
{
    emb_lv_scene_t *ptThis = ptScene;
    /*	step1,	handle the focus settings */
    if(ptThis->bAutoFocus && ptThis->bNeedFocus && ptThis->u8FocusTier > 0)
    {
        lv_obj_t *ptFocusObj = ptThis->ptScreen;
        for(int32_t i = (int32_t)ptThis->u8FocusTier - 1; i >= 0; i--)
        {
            ptFocusObj = lv_obj_get_child(ptFocusObj, ptThis->chFocusIndex[i]);
        }
        lv_group_focus_obj(ptFocusObj);
    }

    /*	step2,	handle the working state */
    if(ptThis->ptSceneEX[0]->ptTimer)
    {
        ptThis->bIsWorking ? lv_timer_resume(ptThis->ptSceneEX[0]->ptTimer) 
                           : lv_timer_pause (ptThis->ptSceneEX[0]->ptTimer) ;
    }
}

static void emb_lv_scene_extend_free(emb_lv_scene_t *ptScene)
{
    emb_assert(ptScene != NULL);

    emb_lv_scene_t *ptThis = ptScene;
    /*	step1,	check the scene extension data */
    do{
        if(!ptThis->bHasExtData) return; 
        emb_lv_scene_extend_group_free(ptThis);
        emb_lv_scene_extend_timer_free(ptThis);
    } while(0);
}
/*============================ END ===========================================*/