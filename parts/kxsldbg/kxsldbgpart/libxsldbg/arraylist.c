
/***************************************************************************
                          arraylist.c  -  define array implementation of a list
                             -------------------
    begin                : Sat Nov 10 2001
    copyright            : (C) 2001 by Keith Isdale
    email                : k_isdale@tpg.com.au
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "arraylist.h"
#include <libxslt/xsltutils.h>

#ifndef NULL
#define NULL 0
#endif

/**
 * arrayListNew:
 * @initialSize: The initial size of list
 * @deleteFunction: The function to call to free items in the list
 *
 * Create a new list with a size of @initialSize
 *
 * Returns Non-null on success,
 *         NULL otherwise
 */
arrayListPtr
arrayListNew(int initialSize, freeItemFunc deleteFunction)
{
    arrayListPtr list = NULL;

    if (initialSize <= 0) {
        xsltGenericError(xsltGenericErrorContext,
                         "arrayListNew invalid initialSize %d\n",
                         initialSize);
    } else
        list = (arrayListPtr) xmlMalloc(sizeof(arrayList));

    if (list) {
        list->data = (void *) xmlMalloc(sizeof(void *) * initialSize);
        list->deleteFunction = deleteFunction;
        list->count = 0;
        list->size = initialSize;
    }

    return list;
}


/**
 * arrayListFree:
 * @list: A valid list
 *
 * Free memory assocated with array list, if the array list 
 *   has a valid deleteFunction then content with be freed with 
 *    using that deleteFunction
 */
void
arrayListFree(arrayListPtr list)
{
    if (!list)
        return;

    arrayListEmpty(list);
    xmlFree(list->data);
    xmlFree(list);
}


/**
 * arrayListEmpty:
 * @list: A valid list
 *
 * Empties the list of its content
 *
 * Returns 1 on success,
 *         0 otherwise
 */
int
arrayListEmpty(arrayListPtr list)
{
    int index, result = 0;

    if (list) {
        if (list->deleteFunction) {
            for (index = 0; index < list->count; index++) {
                if (list->data[index])
                    (*list->deleteFunction) (list->data[index]);
            }
            result = 1;
            list->count = 0;
        }
    }
    return result;
}


/**
 * arrayListSize:
 * @list: A valid list
 *
 * Return The maximum number elements this list can contain
 *
 * Returns The maximum number elements this list can contain
 */
int
arrayListSize(arrayListPtr list)
{
    int result = 0;

    if (list)
        result = list->size;

    return result;
}


/**
 * arrayListCount:
 * @list: A valid list
 *
 * Return the count of number items in list
 *
 * Returns The count of number items in list
 */
int
arrayListCount(arrayListPtr list)
{
    int result = 0;

    if (list)
        result = list->count;

    return result;
}


/** 
 * arrayListAdd:
 * @list: A valid list
 * @item: A valid list
 *
 * Add @item to @list
 *
 * Returns 1 if able to add @item to end of @list,
 *         0 otherwise
 */
int
arrayListAdd(arrayListPtr list, void *item)
{
    int result = 0;

    if (list && item) {
        if (list->count + 1 > list->size) {
            /* grow the size of data */
            void **temp;
            int newSize, index;

            if (list->size < DOUBLE_SIZE_MAX_ITEM)
                newSize = list->size * 2;
            else
                newSize = (int) (list->size * 1.5);
            temp = (void *) xmlMalloc(sizeof(void *) * newSize);
            for (index = 0; index < list->count; index++) {
                temp[index] = list->data[index];
            }
            xmlFree(list->data);
            list->data = temp;
            list->size = newSize;
        }
        list->data[list->count++] = item;
        result = 1;
    }
    return result;
}


/**
 * arrayListDelete:
 * @list: A valid list
 * @position: 0 =< @position < arrayListCount(@list)
 *
 * Delete item at position @position from @list
 *
 * Returns 1 if able to delete element in @list at position @position,
 *         0 otherwise 
 */
int
arrayListDelete(arrayListPtr list, int position)
{
    int result = 0, index;

    if (list && (list->count > 0) && (position >= 0) &&
        (position < list->count) && list->data[position]) {
        if (list->deleteFunction)
            (*list->deleteFunction) (list->data[position]);

        /* shuffle all elements upwards */
        for (index = position; index < (list->count - 1); index++) {
            list->data[index] = list->data[index + 1];
        }
        list->count--;
        result = 1;
    }
    return result;
}



/**
 * arrayListGet:
 * @list: A valid list
 * @position: 0 =< @position < arrayListCount(@list)
 *
 * Get item at position @position from @list
 *
 * Returns Non-null if able to retrieve element in @list at position @position,
 *         NULL otherwise
 */
void *
arrayListGet(arrayListPtr list, int position)
{
    void *result = NULL;

    if (list && (position >= 0) && (position < list->count)) {
        result = list->data[position];
    }
    return result;
}
