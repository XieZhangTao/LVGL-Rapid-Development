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
} emb_lv_scene_switch_anim_t;

typedef struct{ 
    void (*pfBeforeScene)(void);      /* Callback function before scene create     */
    void (*pfSceneCreate)(void);      /* Scene create callback function            */
    void (*pfSceneUpdate)(void);      /* Scene update callback function            */
    void (*pfAfterSceneDelete)(void); /* Scene delete callback function      
                                         Note : For the components bound to Screen,
                                         you don't need to delete them here. The 
                                         background will handle them automatically */
} emb_lv_scene_cfg_t;
    
typedef struct{
    lv_group_t     *ptGroup           ; /* Scene group objects */
    lv_timer_t     *ptTimer           ; /* Scene timer object  */
} emb_lv_scene_ext_t;

typedef struct {
    lv_obj_t            *ptScreen         ; /* Scene screen object      */
    emb_lv_scene_cfg_t   tCfg             ; /* Scene configuration      */
    emb_list_t           tListNode        ; /* Scene list node          */ 
    uint8_t              chFocusIndex  [4]; /* Focus object index       */
    uint32_t             u8SceneID    : 8 ; /* Scene ID                 */
    uint32_t             u8FocusTier  : 8 ; /* Focus tier index         */
    uint32_t             bIsWorking   : 1 ; /* Is working               */
    uint32_t             bAutoFocus   : 1 ; /* Auto focus on scene load */
    uint32_t             bNeedFocus   : 1 ; /* Needs to be focused      */
    uint32_t             bIsEdit      : 1 ; /* Is in edit mode          */
    uint32_t             bHasExtData  : 1 ; /* Has extension data       */
    uint32_t                          : 11; /* Reserved bits            */
    emb_lv_scene_ext_t  *ptSceneEX[]      ; /* Scene extension data     */
} emb_lv_scene_t;
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
void emb_lv_scene_switch_anim_set(emb_lv_scene_switch_anim_t *ptAnim);

/**
 * @brief Create a new scene
 * @param chSceneID Scene ID
 * @param ptCfg Pointer to the scene configuration
 * @return Pointer to the created scene, or NULL on failure
 */
extern
emb_lv_scene_t* emb_lv_scene_create(uint8_t chSceneID, emb_lv_scene_cfg_t *ptCfg);

/**
 * @brief Create a new scene with extension data
 * @param chSceneID Scene ID
 * @param ptCfg Pointer to the scene configuration
 * @param ptSceneEX Pointer to the scene extension data
 * @return Pointer to the created scene, or NULL on failure
 */
extern
emb_lv_scene_t *emb_lv_scene_create_with_extend(uint8_t u8SceneID, 
                                                emb_lv_scene_cfg_t *ptCfg, 
                                                emb_lv_scene_ext_t *ptSceneEX);

/**
 * @brief Append a scene to the scene list
 * @param ptScene Pointer to the scene to append
 */
extern
void emb_lv_scene_list_append(emb_lv_scene_t *ptScene);

/**
 * @brief Remove a scene from the scene list
 * @param ptScene Pointer to the scene to remove
 */
extern
void emb_lv_scene_list_remove(emb_lv_scene_t *ptScene);

/**
 * @brief Load the current scene
 */
extern
void emb_lv_scene_current_scene_load(void);

/**
 * @brief Reload the current scene
 */
extern
void emb_lv_scene_current_scene_reload(void);

/**
 * @brief Load the current scene and delete the previous one
 */
extern
void emb_lv_scene_current_scene_load_with_del_previous(void);

/**
 * @brief Go back to the previous scene
 * @note This function will release the current scene, then create
 *       and load the previous scene, and finally restore the
 *       previous scene extension state.
 */
extern
void emb_lv_scene_previous_scene_back(void);

/**
 * @brief Restore to the previous scene
 * @note This function will release the current scene and only load
 *       the existing previous scene screen.
 *       It will not recreate scene UI callbacks and will not
 *       restore previous scene extension state.
 */
extern
void emb_lv_scene_previous_scene_restore(void);

/**
 * @brief Get the current scene
 * @return Pointer to the current scene, or NULL if no scene is available
 */
extern 
emb_lv_scene_t* emb_lv_scene_current_scene_struct_get(void);

/**
 * @brief Get the current scene ID
 * @return Current scene ID, or -1 if no scene is available
 */
extern
int32_t emb_lv_scene_current_scene_id_get(void);

/**
 * @brief Set the current scene mode
 * @param tMode Scene mode to set (navigation or edit)
 */
extern
void emb_lv_scene_current_scene_mode_set(emb_lv_scene_mode_t tMode);

/**
 * @brief Get the current scene mode
 * @return Current scene mode (navigation or edit)
 */
extern
emb_lv_scene_mode_t emb_lv_scene_current_scene_mode_get(void);

/*============================ LOCAL IMPLEMENTATION ==========================*/
/**
 * @brief Set the auto focus state of a scene
 * @param ptScene Pointer to the scene
 * @param bAutoFocus Auto focus state to set (true to enable, false to disable)
 */
static inline
void emb_lv_scene_auto_focus_set(emb_lv_scene_t *ptScene, bool bAutoFocus)
{
    emb_assert(ptScene != NULL);
    ptScene->bAutoFocus = bAutoFocus;
}

/**
 * @brief Get the auto focus state of a scene
 * @param ptScene Pointer to the scene
 * @return Auto focus state (true if enabled, false if disabled)
 */
static inline
bool emb_lv_scene_auto_focus_get(emb_lv_scene_t *ptScene)
{
    emb_assert(ptScene != NULL);
    return ptScene->bAutoFocus;
}
/*============================ END ===========================================*/
#ifdef   __cplusplus
}
#endif
#endif /* __EMB_LV_SCENE_H__ */
