/****************************************************************************
*  Copyright 2025 xzt (Email:genuine2001@163.com)                           *
*                                                                           *
*  Licensed under the Apache License, Version 2.0 (the "License");          *
*  you may not use this file except in compliance with the License.         *
*  You may obtain a copy of the License at                                  *
*                                                                           *
*     http://www.apache.org/licenses/LICENSE-2.0                            *
*                                                                           *
*  Unless required by applicable law or agreed to in writing, software      *
*  distributed under the License is distributed on an "AS IS" BASIS,        *
*  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. *
*  See the License for the specific language governing permissions and      *
*  limitations under the License.                                           *
*                                                                           *
****************************************************************************/

#ifndef __EMB_LIST_H__
#define __EMB_LIST_H__

/*============================ INCLUDES ======================================*/
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef   __cplusplus
extern "C" {
#endif
/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
#define list_entry(ptr, type, member)                               \
    ((type *)((uintptr_t)(ptr) - offsetof(type, member)))

#define list_for_each_entry(pos, list, member)                      \
    for (pos = list_entry((list)->ptNext, typeof(*pos), member);    \
         &pos->member != (list);                                    \
         pos = list_entry(pos->member.ptNext, typeof(*pos), member))
/*============================ TYPES =========================================*/
typedef struct _emb_list_t
{
    struct _emb_list_t *ptNext;
    struct _emb_list_t *ptPrev;
} emb_list_t;

/*============================ PROTOTYPES ====================================*/
static inline
void _emb_list_add(emb_list_t *ptNode, emb_list_t *ptPrev, emb_list_t *ptNext);

/*============================ IMPLEMENTATION ================================*/
/*============================ LOCAL IMPLEMENTATION ==========================*/
static inline 
void emb_list_init(emb_list_t *ptList)
{
    ptList->ptNext = ptList;
    ptList->ptPrev = ptList;
}

static inline 
void emb_list_node_insert_head(emb_list_t *ptNode, emb_list_t *ptList)
{
    _emb_list_add(ptNode, ptList, ptList->ptNext);
}

static inline 
void emb_list_node_insert_tail(emb_list_t *ptNode, emb_list_t *ptList)
{
    _emb_list_add(ptNode, ptList->ptPrev, ptList);
}

static inline 
void emb_list_node_delete(emb_list_t *ptNode)
{
    ptNode->ptPrev->ptNext = ptNode->ptNext;
    ptNode->ptNext->ptPrev = ptNode->ptPrev;
    ptNode->ptNext = ptNode;
    ptNode->ptPrev = ptNode;
}

static inline
void emb_list_node_move_to_tail(emb_list_t *ptNode, emb_list_t *ptList)
{
    emb_list_node_delete(ptNode);
    emb_list_node_insert_tail(ptNode, ptList);
}

static inline
void emb_list_node_move_to_head(emb_list_t *ptNode, emb_list_t *ptList)
{
    emb_list_node_delete(ptNode);
    emb_list_node_insert_head(ptNode, ptList);
}

static inline 
void emb_list_clean(emb_list_t *ptList)
{
    emb_list_init(ptList);
}

static inline 
bool emb_list_is_empty(const emb_list_t *ptList)
{
    bool bIsEmpty = (ptList->ptNext == ptList);
    return bIsEmpty;
}

static inline 
bool emb_list_node_is_last(const emb_list_t *ptNode, const emb_list_t *ptList)
{
    bool bIsLast = (ptNode->ptNext == ptList);
    return bIsLast;
}

static inline
void _emb_list_add(emb_list_t *ptNode, emb_list_t *ptPrev, emb_list_t *ptNext)
{
    ptPrev->ptNext = ptNode;
    ptNode->ptPrev = ptPrev;
    ptNode->ptNext = ptNext;
    ptNext->ptPrev = ptNode;
}
/*=============================== END ========================================*/
#ifdef   __cplusplus
}
#endif
#endif /* __EMB_LIST_H__ */
