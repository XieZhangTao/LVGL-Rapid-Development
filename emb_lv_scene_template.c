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
#if defined(_WIN64)
#include "emb_lvgl/emb_lvgl.h"
#else 
#include "emb_lvgl.h"
#endif

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
typedef struct{

} ui_component_t;

/*============================ GLOBAL VARIABLES ==============================*/
lv_scene_t *g_ptScene%Instance% = NULL;

/*============================ LOCAL VARIABLES ===============================*/
static ui_component_t tUI;

/*============================ PROTOTYPES ====================================*/
/*============================ IMPLEMENTATION ================================*/
static void emb_lv_scene_ui_create(void)
{
    lv_obj_t *ptScreen = g_ptScene%Instance%->ptScreen;
  
    /*--------------------- insert your ui code here --------------------*/
    /**
     * @note All components should be directly or indirectly mounted on
     * the ptScreen.
     */
   
    /*--------------------- END -----------------------------------------*/
}

static void emb_lv_before_scene_create(void)
{
    /*----------------- Add operations before creating the UI ------------*/
    /**
     * @note You don't need to create components here. Just add operations
     * that need to be executed before creating the UI, such as initializing
     * variables.
     */
}

static void emb_lv_scene_update(void)
{
    /*----------------- Add operations when updating the UI ------------*/
    /**
     * @note This function will be automatically called once after the 
     *       execution of emb_lv_scene_ui_create.
     */
}

static void emb_lv_after_scene_delete(void)
{
    /*----------------- Add operations when releasing the UI ------------*/
    /**
     * @note You don't need to delete the components bound to the
     * screen here. They will run automatically in the background.
     */

}

void emb_lv_scene%Instance%_init(void)
{
    lv_scene_cfg_t tCFG;
    tCFG.pfBeforeScene      = emb_lv_before_scene_create;
    tCFG.pfSceneCreate      = emb_lv_scene_ui_create;
    tCFG.pfSceneUpdate      = emb_lv_scene_update;
    tCFG.pfAfterSceneDelete = emb_lv_after_scene_delete;
#if 0
    g_ptScene%Instance% = emb_lv_scene_create(%Instance%, &tCFG);
#else
    lv_scene_ext_t *ptSceneEX = lv_mem_alloc(sizeof(lv_scene_ext_t));
    /*------------------ Initialize additional data -------------------*/
    /**
     * @note Do not allocate space for additional data here.
     *  Define yours at emb_lv_scene_ui_create Additional data
     *  structure.
     */
    ptSceneEX->ptGroup    = NULL;
    ptSceneEX->ptTimer    = NULL;
    /*------------------ END ------------------------------------------*/
    g_ptScene%Instance% = emb_lv_scene_create_with_extend(%Instance%, &tCFG, ptSceneEX);
#endif
} 
/*============================ END ===========================================*/
