/*******************************************************************************
File name: list.h
Compiler: Borland 5.5
Author: Christopher Elliott, 040 570 022
Course: CST 8152 - Compilers, Lab Section : 012
Date: 28/12/2015 (DD/MM/YYYY)
Professor: Sv. Ranev
Purpose: Implement the list data structure.
Function list: [
	l_create(), l_copy, l_add(), l_get(), l_getnext, l_hasnext, l_set(),
	l_remove(), l_size(), l_isempty(), l_pack(), l_reset(), l_reset_iterator(),
	l_destroy()
]
*******************************************************************************/
#ifndef LIST_H_
#define LIST_H_

#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <limits.h>

#define R_FAIL_1 -1
#define R_FAIL_0 0

typedef struct ListDescriptor {
	void* elmnts;
	int capacity;
	int elmnt_offset;
	int get_offset;
	short elmnt_sz;
	short inc_factor;
} List, *pList;

List* l_create(short init_capacity, short inc_factor, short elmnt_sz);
List* l_copy(List* src);
void* l_add(List* const pLD, const void* elmnt);
void* l_get(List* const pLD, const int index);
void* l_getnext(List* const pLD);
int l_hasnext(List* const pLD);
void* l_set(List* const pLD, const int index, const void* elmnt);
void* l_remove(List* const pLD, const int index, void* elmnt);
int l_size(List* const pLD);
int l_isempty(List* const pLD);
void* l_pack(List* const pLD);
int l_reset(List* const pLD);
int l_set_iterator(List* const pLD, int offset);
int l_reset_iterator(List* const pLD);
void l_destroy(List* const pLD);

#define INLINE
/*#undef INLINE*/

#ifdef INLINE
#define l_hasnext(pLD) (((List*)pLD) == NULL ? R_FAIL_0 : (((List*)pLD)->get_offset != ((List*)pLD)->elmnt_offset))
#define l_size(pLD) (((List*)pLD) == NULL ? R_FAIL_1 : ((List*)pLD)->elmnt_offset)
#define l_isempty(pLD) (((List*)pLD) == NULL ? R_FAIL_1 : (((List*)pLD)->elmnt_offset == 0))
#define l_get_offset(pLD) (((List*)pLD == NULL) ? R_FAIL_0 : ((List*)pLD)->get_offset)
#endif

#endif
