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
/*============================ MACROFIED FUNCTIONS ===========================*/
#define list_entry(ptr, type, member)                               \
    ((type *)((uintptr_t)(ptr) - offsetof(type, member)))

#define list_for_each_entry(pos, head, member)                      \
    for (pos = list_entry((head)->ptNext, typeof(*pos), member);    \
         &pos->member != (head);                                    \
         pos = list_entry(pos->member.ptNext, typeof(*pos), member))

#define list_for_each_entry_safe(pos, n, head, member)                   \
    for (pos = list_entry((head)->ptNext, typeof(*pos), member),         \
         n   = list_entry(pos->member.ptNext, typeof(*n), member);       \
         &pos->member != (head);                                         \
         pos = n, n = list_entry(n->member.ptNext, typeof(*n), member))
/*============================ TYPES =========================================*/
typedef struct _emb_list_t
{
    struct _emb_list_t *ptNext;
    struct _emb_list_t *ptPrev;
} emb_list_t;

/*============================ PROTOTYPES ====================================*/
static inline 
void _emb_list_add(emb_list_t *ptNode, emb_list_t *ptHead, emb_list_t *ptNext);

/*============================ IMPLEMENTATION ================================*/
static inline 
void emb_list_init(emb_list_t *ptList)
{
    ptList->ptNext = ptList;
    ptList->ptPrev = ptList;
}

static inline 
void emb_list_add_head(emb_list_t *ptNode, emb_list_t *ptHead)
{
    _emb_list_add(ptNode, ptHead, ptHead->ptNext);
}

static inline 
void emb_list_add_tail(emb_list_t *ptNode, emb_list_t *ptHead)
{
    _emb_list_add(ptNode, ptHead->ptPrev, ptHead);
}

static inline 
void emb_list_del(emb_list_t *ptNode)
{
    ptNode->ptPrev->ptNext = ptNode->ptNext;
    ptNode->ptNext->ptPrev = ptNode->ptPrev;
}

static inline 
void emb_list_move_tail(emb_list_t *ptNode, emb_list_t *ptHead)
{
    emb_list_del(ptNode);
    emb_list_add_tail(ptNode, ptHead);
}

static inline 
void emb_list_move_head(emb_list_t *ptNode, emb_list_t *ptHead)
{
    emb_list_del(ptNode);
    emb_list_add_head(ptNode, ptHead);
}

static inline 
void emb_list_clean(emb_list_t *ptHead)
{
    emb_list_init(ptHead);
}

static inline 
bool emb_list_is_empty(emb_list_t *ptHead)
{
    bool bIsEmpty = (ptHead->ptNext == ptHead);
    return bIsEmpty;
}

static inline 
bool emb_list_is_last(emb_list_t *ptNode, emb_list_t *ptHead)
{
    bool bIsLast = (ptNode->ptNext == ptHead);
    return bIsLast;
}

static inline 
void _emb_list_add(emb_list_t *ptNode, emb_list_t *ptHead, emb_list_t *ptNext)
{
    ptHead->ptNext = ptNode;
    ptNode->ptPrev = ptHead;
    ptNode->ptNext = ptNext;
    ptNext->ptPrev = ptNode;
}
/*=============================== END ========================================*/
#ifdef   __cplusplus
}
#endif
#endif /* __EMB_LIST_H__ */
