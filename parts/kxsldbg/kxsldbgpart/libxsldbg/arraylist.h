
/**************************************************************************
                          arraylist.h  -  declare the functions for 
                                        implementation of the array list
                             -------------------
    begin                : Sat Nov 10 2001
    copyright            : (C) 2001 by Keith Isdale
    email                : k_isdale@tpg.com.au
 **************************************************************************/

/**************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 **************************************************************************/

#ifndef ARRAYLIST_H
#define ARRAYLIST_H

#ifdef USE_KDE_DOCS

/**
 * Provide a fast easy to use array list. Support the basic functions of add
 *  delete, empty, count, free
 *
 * @short Array list support
 *
 * @author Keith Isdale <k_isdale@tpg.com.au> 
 */
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifdef USE_GNOME_DOCS

#else
#ifdef USE_KDE_DOCS

#endif
#endif


    typedef void (*freeItemFunc) (void *item);
    /* A dynamic structure behave like a list */
    typedef struct _arrayList arrayList;
    typedef arrayList *arrayListPtr;
    struct _arrayList {
        int size, count;
        void **data;
        freeItemFunc deleteFunction;
    };

/* what size of the list do we stop automatic doubling of capacity
    if array list size growth is needed */
#define DOUBLE_SIZE_MAX_ITEM 10



#ifdef USE_GNOME_DOCS

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
#else
#ifdef USE_KDE_DOCS

/**
 * Create a new list with a size of @p initialSize
 *
 * @returns Non-null on success,
 *          NULL otherwise
 *
 * @param initial The initial size of list
 * @param deleteFunction the Function to call to free items in the list
 */
#endif
#endif
    arrayListPtr arrayListNew(int initialSize,
                              freeItemFunc deleteFunction);



#ifdef USE_GNOME_DOCS

/**
 * arrayListFree:
 * @list: A valid list
 *
 * Free memory assocated with array list, if the array list 
 *   has a valid deleteFunction then content with be freed with 
 *    using that deleteFunction
 */
#else
#ifdef USE_KDE_DOCS

/**
 * Free memory assocated with array list, if the array list 
 *   has a valid deleteFunction then content with be freed with 
 *    using that deleteFunction
 *
 * @param list A valid list
 */
#endif
#endif
    void arrayListFree(arrayListPtr list);



#ifdef USE_GNOME_DOCS

/**
 * arrayListEmpty:
 * @list: A valid list
 *
 * Empties the list of its content
 *
 * Returns 1 on success,
 *         0 otherwise
 */
#else
#ifdef USE_KDE_DOCS

/**
 * Empties the list of its content
 *
 * @returns 1 on success,
 *          0 otherwise
 *
 * @param list A valid list
 */
#endif
#endif
    int arrayListEmpty(arrayListPtr list);



#ifdef USE_GNOME_DOCS

/**
 * arrayListSize:
 * @list: A valid list
 *
 * Return The maximum number elements this list can contain
 *
 * Returns The maximum number elements this list can contain
 */
#else
#ifdef USE_KDE_DOCS

/**
 * Return the maximum number elements this list can contain
 *
 * @returns The maximum number elements this list can contain
 *
 * @param list A valid list
 */
#endif
#endif
    int arrayListSize(arrayListPtr list);



#ifdef USE_GNOME_DOCS

/**
 * arrayListCount:
 * @list: A valid list
 *
 * Return the count of number items in list
 *
 * Returns The count of number items in list
 */
#else
#ifdef USE_KDE_DOCS

/**
 * Return the count of number items in list
 * @returns The count of number items in list
 *
 * @param list A valid list
 */
#endif
#endif

    int arrayListCount(arrayListPtr list);



#ifdef USE_GNOME_DOCS

/**
 * arrayListAdd:
 * @list: A valid list
 * @item:A valid item
 *
 * Add @item to @list
 *
 * Returns 1 if able to add @item to end of @list,
 *         0 otherwise
 */
#else
#ifdef USE_KDE_DOCS

/**
 * Add @p item to @p list
 *
 * @returns 1 if able to add @p item to end of @p list,
 *          0 otherwise
 *
 * @param list A valid list
 * @param item A valid item
 */
#endif
#endif
    int arrayListAdd(arrayListPtr list, void *item);



#ifdef USE_GNOME_DOCS

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
#else
#ifdef USE_KDE_DOCS

/**
 * @returns 1 if able to delete element in @p list at position @p position,
 *          0 otherwise 
 *
 * @param list A valid list
 * @param position  0 =< @p position < arrayListCount(@p list)
 */
#endif
#endif
    int arrayListDelete(arrayListPtr list, int position);



#ifdef USE_GNOME_DOCS

/**
 * arrayListGet:
 * @list: A valid list
 * @position: 0 =< @position < arrayListCount(@list)
 *
 * Get item at position @position from @list
 *
 * Returns Non-null if able to retrieve element in @list at position
 *            @position,
 *         NULL otherwise
 */
#else
#ifdef USE_KDE_DOCS

/**
 * @returns Non-null if able to retrieve element in @p list at position
 *          @p position,
 *         NULL otherwise
 *
 * @param list A valid list
 * @param position  0 =< @p position < arrayListCount(@p list)
 */
#endif
#endif
    void *arrayListGet(arrayListPtr list, int position);


#ifdef __cplusplus
}
#endif
#endif
