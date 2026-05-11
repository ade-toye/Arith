/*
    *   Homework 2
    *   uarray2.c
    *   this program is an implementation of UArray2 Interface
    *   it's allows polymorphic encapsulation of our new 2D unboxed array
    *   Feb, 02 2026
    *   Updated: Feb, 03 2026
    *   Valens Dabagire [vdabag01] Toye Adebayo [aadeb02]
*/

/* Externals*/

#include <stdlib.h>
#include <assert.h>
#include "uarray.h"
#include "uarray2.h"

/* Implementation*/
#define T UArray2_T
struct T 
{
        int width, height, size;
        UArray_T storage;
};

/*
 * name:       UArray2_new
 * purpose:    Allocates and initializes a new 2D array of 
 *             the given dimensions.
 * parameters: width and height of the grid, and size in bytes of each element.
 * return:     A handle (pointer) to the new UArray2_T.
 * expect:     width and height are non-negative; size is positive. 
 *             Allocation of the descriptor and storage succeeds.
*/
T UArray2_new(int width, int height, int size)
{
        T uarray2 = malloc(sizeof(*uarray2));
        assert(uarray2 != NULL); 
        uarray2->width = width;
        uarray2->height = height;
        uarray2->size = size;
        uarray2->storage = UArray_new(width * height, size);
        return uarray2;
}

/*
 * name:       UArray2_free
 * purpose:    Deallocates the memory associated with the UArray2_T.
 * parameters: A pointer to the UArray2_T handle.
 * return:     none
 * expect:     uarray2 and *uarray2 are non-null. 
 * effect:     Sets *uarray2 to NULL to prevent dangling pointer access.
*/
void UArray2_free(T *uarray2)
{
        assert(uarray2 != NULL && *uarray2 != NULL);
        UArray_free(&(*uarray2)->storage);
        free(*uarray2);
        *uarray2 = NULL;
}

/*
 * name:       UArray2_width
 * purpose:    Returns the horizontal dimension (number of columns) 
 *             of the array.
 * parameters: The UArray2_T handle.
 * return:     Integer width of the array.
 * expect:     uarray2 is non-null.
*/
int UArray2_width(T uarray2)
{
        assert(uarray2 != NULL);
        return uarray2->width;
}

/*
 * name:       UArray2_height
 * purpose:    Returns the vertical dimension (number of rows) of the array.
 * parameters: The UArray2_T handle.
 * return:     Integer height of the array.
 * expect:     uarray2 is non-null.
*/
int UArray2_height(T uarray2)
{
        assert(uarray2 != NULL);
        return uarray2->height;
}

/*
 * name:       UArray2_size
 * purpose:    Returns the size of each element in the array in bytes.
 * parameters: The UArray2_T handle.
 * return:     Integer size of one element.
 * expect:     uarray2 is non-null.
*/
int UArray2_size(T uarray2)
{
        assert(uarray2 != NULL);
        return uarray2->size;
}

/*
 * name:       UArray2_at
 * purpose:    Provides access to the element at a specific column and row.
 * parameters: The UArray2_T handle and the (col, row) coordinates.
 * return:     A pointer to the element at the specified index.
 * expect:     uarray2 is non-null; col is in [0, width); row is 
 *             in [0, height).
*/
void *UArray2_at(T uarray2, int col, int row)
{
        assert(uarray2 != NULL);
        assert(col >= 0 && col < uarray2->width);
        assert(row >= 0 && row < uarray2->height);

        int index = row * uarray2->width + col;
        return UArray_at(uarray2->storage, index);
}

/*
 * name:       UArray2_map_row_major
 * purpose:    Visits every element in the array in row-major order.
 * parameters: The UArray2_T, an apply function, and a client data pointer.
 * return:     none
 * expect:     uarray2 and apply are non-null.
 * effect:     Calls apply for every element, iterating through columns 
 *             for each row.
*/
void UArray2_map_row_major(T uarray2, UArray2_apply *apply, void *cl)
{
        assert(uarray2 != NULL);
        for (int r = 0; r < uarray2->height; r++){
                for (int c = 0; c < uarray2->width; c++){
                        apply(c, r, uarray2,
                              UArray2_at(uarray2, c, r), cl);
                }
        }
}

/*
 * name:       UArray2_map_col_major
 * purpose:    Visits every element in the array in column-major order.
 * parameters: The UArray2_T, an apply function, and a client data pointer.
 * return:     none
 * expect:     uarray2 and apply are non-null.
 * effect:     Calls apply for every element, iterating through 
 *             rows for each column.
*/
void UArray2_map_col_major(T uarray2, UArray2_apply *apply, void *cl)
{
        assert(uarray2 != NULL);
        for (int c = 0; c < uarray2->width; c++){
                for (int r = 0; r < uarray2->height; r++){
                        apply(c, r, uarray2,
                              UArray2_at(uarray2, c, r), cl);
                }
        }
}

/* @val __ adetoye*/