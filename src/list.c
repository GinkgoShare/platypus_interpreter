/*******************************************************************************
File name: list.h
Compiler: Borland 5.5
Author: Christopher Elliott, 040 570 022
Course: CST 8152 - Compilers, Lab Section : 012
Date: 29/12/2015 (DD/MM/YYYY)
Professor: Sv. Ranev
Purpose: Implement the list data structure.
Function list: [
	l_create(), l_copy, l_add(), l_get(), l_getnext, l_hasnext, l_set(),
	l_remove(), l_size(), l_isempty(), l_pack(), l_reset(), l_reset_iterator(),
	l_destroy()
]
*******************************************************************************/
#include "list.h"

#define DEBUG
#undef DEBUG

/*******************************************************************************
Purpose: Initialize a ListDescriptor structure on the program heap. 
Author: Christopher JW Elliott, 040 570 022
History/Versions: Version 0.0.1 29/12/2015
Called functions: [ calloc(), malloc(), free() ]
Parameters: init_capacity is a short type with an inclusive range of 0 to 
			SHRT_MAX, inc_factor is a short type with an inclusive range of 0 
			to SHRT_MAX
Return value: returns the address of the defined ListDescriptor
Algorithm: initialize the ListDescriptor structure, initialize embedded array
		   to a capacity defined by the argument init_capacity, and set the list 
		   increment factor
*******************************************************************************/
List* l_create(short init_capacity, short inc_factor, short elmnt_sz) {
	List* pList;
	if (init_capacity < 0 || elmnt_sz <= 0 || inc_factor <= 0) return NULL;

	pList = (List*)calloc(1, sizeof(List));
	if (!pList) return NULL;

	/* set capacity and allocate memory */
	pList->capacity = (int)(elmnt_sz * init_capacity);
	pList->elmnts = malloc(pList->capacity);
	if (!pList->elmnts) {
		free(pList);
		return NULL;
	}

	pList->inc_factor = inc_factor;
	pList->elmnt_sz = elmnt_sz;
	return pList;
}
/*******************************************************************************
Purpose: Copies and existing list to a new. 
Author: Christopher JW Elliott, 040 570 022
History/Versions: Version 0.0.1 29/12/2015
Called functions: [ calloc(), free(), memcpy() ]
Parameters: src is the list to be copied
Return value: returns the address of a new ListDescriptor structure on the heap
Algorithm: initialize a ListDescriptor structure using the existing list's 
		   members
*******************************************************************************/
List* l_copy(List* src) {
	List* pList;
	pList = (List*)calloc( 1, sizeof(List));
	if (!pList) return NULL;

	/* set capacity and allocate memory */
	pList->capacity = src->capacity;
	pList->elmnts = malloc(pList->capacity);
	if (!pList->elmnts) {
		free(pList);
		return NULL;
	}
	memcpy(pList->elmnts, src->elmnts, src->capacity);
	pList->elmnt_offset = src->elmnt_offset;
	pList->inc_factor = src->inc_factor;
	pList->elmnt_sz = src->elmnt_sz;
	return pList;
}
/*******************************************************************************
Purpose: Append a new item to the list. 
Author: Christopher JW Elliott, 040 570 022
History/Versions: Version 0.0.1 29/12/2015
Called functions: [ realloc(), memcpy() ]
Parameters: pLD is the ListDescriptor that is being appended to, elmnt is the 
			new element to append to the list.
Return value: returns the address of the ListDescriptor
Algorithm: Check if the list is at its maximum capacity, check if the list is at
		   its current capacity, if so, reallocate with additional memory
		   storage, add element to the end of the list
*******************************************************************************/
void* l_add(List* const pLD, const void* elmnt) {
	int new_capacity;
	void* pLoc; void* dest_addr;
	if (!pLD && pLD->capacity == INT_MAX) return NULL;

	if((pLD->elmnt_offset * pLD->elmnt_sz) == pLD->capacity) {
		new_capacity = pLD->capacity + pLD->inc_factor * pLD->elmnt_sz;
		if (new_capacity < 0) return NULL;
		pLoc = realloc(pLD->elmnts, new_capacity);
		if (pLoc == NULL) return NULL;
		pLD->capacity = new_capacity;
		pLD->elmnts = pLoc;
	}

	dest_addr = (char*)pLD->elmnts + (pLD->elmnt_offset * pLD->elmnt_sz);
	memcpy(dest_addr, elmnt, pLD->elmnt_sz);
	pLD->elmnt_offset++;
	return pLD;
}
/*******************************************************************************
Purpose: Get the address of an element. 
Author: Christopher JW Elliott, 040 570 022
History/Versions: Version 0.0.1 29/12/2015
Parameters: pLD is the ListDescriptor that is being appended to, index is the 
			index of the element to return.
Return value: returns the address at the specified index.
*******************************************************************************/
void* l_get(List* const pLD, const int index) {
	if (!pLD || index < 0 || index >= pLD->elmnt_offset) return NULL;
	return (char*)pLD->elmnts + index * pLD->elmnt_sz;
}
/*******************************************************************************
Purpose: Iterates over the list. Proceeds with each individual call. 
Author: Christopher JW Elliott, 040 570 022
History/Versions: Version 0.0.1 29/12/2015
Parameters: pLD is the ListDescriptor that is being used to get
Return value: returns the address of the get_offset index.
*******************************************************************************/
void* l_getnext(List* const pLD) {
	void* elmnt;
	if (!pLD) return NULL;
	elmnt = (char*)pLD->elmnts + pLD->get_offset++ * pLD->elmnt_sz;
	return elmnt;
}
/*******************************************************************************
Purpose: Reports if this list has another element for l_getnext(). 
Author: Christopher JW Elliott, 040 570 022
History/Versions: Version 0.0.1 29/12/2015
Parameters: pLD is the ListDescriptor that is queried
Return value: returns 1 for true and 0 for false
*******************************************************************************/
#ifndef INLINE
int l_hasnext(List* const pLD) {
	return (!pLD) ? R_FAIL_0 : (pLD->get_offset != pLD->elmnt_offset);
}
#endif
/*******************************************************************************
Purpose: Set's an already specified index with a new element. 
Author: Christopher JW Elliott, 040 570 022
History/Versions: Version 0.0.1 29/12/2015
Called functions: [ memcpy() ]
Parameters: pLD is the ListDescriptor that is being appended to, index is the 
			index to set the element to, elmnt is the new item to store.
Return value: returns the address of the list descriptor.
Algorithm: Make sure the specified index is within a valid range, store element
*******************************************************************************/
void* l_set(List* const pLD, const int index, const void* elmnt) {
	void *dest_addr;
	if (!pLD || index < 0 || index >= pLD->elmnt_offset) return NULL;
	dest_addr = (char*)pLD->elmnts + index * pLD->elmnt_sz;
	memcpy(dest_addr, elmnt, pLD->elmnt_sz);
	return pLD;
}
/*******************************************************************************
Purpose: Removes an element from the list and shifts remaining elements. 
Author: Christopher JW Elliott, 040 570 022
History/Versions: Version 0.0.1 29/12/2015
Called functions: [ memcpy() ]
Parameters: pLD is the ListDescriptor that is being appended to, index is the 
			index to set the element to, elmnt is the new item to store.
Return value: the adjusted size of the list
Algorithm: Make sure the specified index is within a valid range, shift the
		   following elements to the removed index position
*******************************************************************************/
void* l_remove(List* const pLD, const int index, void* elmnt) {
	if (!pLD || index < 0 || index >= pLD->elmnt_offset) return NULL;
	pLD->elmnt_offset--;
	/* copy element */
	memcpy(elmnt, (char*)pLD->elmnts + index * pLD->elmnt_sz, pLD->elmnt_sz);
	/* shift remaining elements */
	memcpy((char*)pLD->elmnts + index * pLD->elmnt_sz, (char*)pLD->elmnts + (index+1) * pLD->elmnt_sz, (pLD->elmnt_offset - index) * pLD->elmnt_sz);
	return elmnt;
}
/*******************************************************************************
Purpose: Returns the current size of the list. 
Author: Christopher JW Elliott, 040 570 022
History/Versions: Version 0.0.1 29/12/2015
Parameters: pLD is the ListDescriptor to find the size of
Return value: the current size of the list
*******************************************************************************/
#ifndef INLINE
int l_size(List* const pLD) {
	return pLD == NULL ? R_FAIL_1 : pLD->elmnt_offset;
}
#endif
/*******************************************************************************
Purpose: Returns true if the list is currently empty. 
Author: Christopher JW Elliott, 040 570 022
History/Versions: Version 0.0.1 29/12/2015
Parameters: pLD is the ListDescriptor to query
Return value: true if the list is currently empty or null, false if otherwise
*******************************************************************************/
#ifndef INLINE
int l_isempty(List* const pLD) {
	return pLD == NULL ? R_FAIL_1 : (pLD->elmnt_offset == 0);
}
#endif
/*******************************************************************************
Purpose: resize the list to its current size plus one
Author: Christopher JW Elliott
History/Versions: Version 0.0.1 29/12/2015
Called functions: [ realloc() ]
Parameters: the address of the list
Return value: returns the constant NULL for a failure or the address of the
			  address of the list when successful
Algorithm: set a temporary variable to reallocate new memory, try to reallocate
		   to a new capacity, if reallocation fails then return NULL, otherwise
		   set capacity member and end of list to 0, return
*******************************************************************************/
void* l_pack(List* const pLD) {
	void* pLoc;
	int new_capacity;
	if (pLD == NULL || pLD->capacity == INT_MAX) return NULL;

	if (pLD->capacity == ((pLD->elmnt_offset+1) * pLD->elmnt_sz)) return pLD; /* list is already packed */
	new_capacity = (pLD->elmnt_offset+1) * pLD->elmnt_sz;
	if (new_capacity < 0) new_capacity = INT_MAX;
	pLoc = realloc(pLD->elmnts, new_capacity);
	if (pLoc == NULL) { return NULL; }
	pLD->capacity = new_capacity;
	pLD->elmnts = pLoc;
	return pLD;
}
/*******************************************************************************
Purpose: Gets the list get_offset member value.
Author: Christopher JW Elliott
History/Versions: Version 0.0.1 15/09/2015
Parameters: the address of the buffer to be reset, the offset value
Return value: returns an int with a value of 0 on failure and 1 for success
*******************************************************************************/
#ifndef INLINE
int l_get_offset(List* const pLD) {
	return (pLD == NULL) ? R_FAIL_0 : pLD->get_offset;
}
#endif
/*******************************************************************************
Purpose: Reset list to its starting positions so data can be overridden
Author: Christopher JW Elliott
History/Versions: Version 0.0.1 15/09/2015
Parameters: the address of the buffer to be reset
Return value: returns an int with a value of 0 on failure and 1 for success
*******************************************************************************/
int l_reset(List* const pLD) {
	if (pLD == NULL) return R_FAIL_0;
	pLD->elmnt_offset = 0;
	pLD->get_offset = 0;
	return 1;
}
/*******************************************************************************
Purpose: Reset list to its starting get_offset position so data can be
		 enumerated
Author: Christopher JW Elliott
History/Versions: Version 0.0.1 15/09/2015
Parameters: the address of the buffer to be reset
Return value: returns an int with a value of 0 on failure and 1 for success
*******************************************************************************/
int l_reset_iterator(List* const pLD) {
	if (pLD == NULL) return R_FAIL_0;
	pLD->get_offset = 0;
	return 1;
}
/*******************************************************************************
Purpose: Set list to get_offset position defined by offset parameter.
Author: Christopher JW Elliott
History/Versions: Version 0.0.1 15/09/2015
Parameters: the address of the buffer to be reset, the offset value
Return value: returns an int with a value of 0 on failure and 1 for success
*******************************************************************************/
int l_set_iterator(List* const pLD, int offset) {
	if (pLD == NULL) return R_FAIL_0;
	pLD->get_offset = offset;
	return 1;
}
/*******************************************************************************
Purpose: Free the list's allocated memory
Author: Christopher JW Elliott
History/Versions: Version 0.0.1 29/12/2015
Called functions: [ free() ]
Parameters: the list to free
*******************************************************************************/
void l_destroy(List* const pLD) {
	if (pLD) {
		if (pLD->elmnts != NULL) free(pLD->elmnts);
		free(pLD); 
	}
}