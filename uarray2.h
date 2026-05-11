/*
    *   Homework 2
    *   uarray2.h
    *   this program is a blueprint for our new 2D unboxed array
    *   Feb, 02 2026
    *   Valens Dabagire [vdabag01] Toye Adebayo [aadeb02]
*/


#ifndef UARRAY2_INCLUDED
#define UARRAY2_INCLUDED

#define T UArray2_T /* search and replace */
typedef struct T *T; /* pointer aliasing */

/* Functions */

/*
 * name:       UArray2_new
 * purpose:    Allocates and initializes a new 2D array of the 
 *             given dimensions.
 * parameters: width and height of the grid, and size in bytes 
 *             of each element.
 * return:     A handle (pointer) to the new UArray2_T.
 * expect:     width and height are non-negative; size is positive. 
 *             Allocation of the descriptor and storage succeeds.
*/
extern T    UArray2_new(int width, int height, int size);

/*
 * name:       UArray2_free
 * purpose:    Deallocates the memory associated with the UArray2_T.
 * parameters: A pointer to the UArray2_T handle.
 * return:     none
 * expect:     uarray2 and *uarray2 are non-null. 
 * effect:     Sets *uarray2 to NULL to prevent dangling pointer access.
*/
extern void UArray2_free(T *uarray2);

/*
 * name:       UArray2_width
 * purpose:    Returns the horizontal dimension 
 *             (number of columns) of the array.
 * parameters: The UArray2_T handle.
 * return:     Integer width of the array.
 * expect:     uarray2 is non-null.
*/
extern int  UArray2_width(T uarray2);

/*
 * name:       UArray2_height
 * purpose:    Returns the vertical dimension (number of rows) of the array.
 * parameters: The UArray2_T handle.
 * return:     Integer height of the array.
 * expect:     uarray2 is non-null.
*/
extern int  UArray2_height(T uarray2);

/*
 * name:       UArray2_size
 * purpose:    Returns the size of each element in the array in bytes.
 * parameters: The UArray2_T handle.
 * return:     Integer size of one element.
 * expect:     uarray2 is non-null.
*/
extern int  UArray2_size(T uarray2);

/*
 * name:       UArray2_at
 * purpose:    Provides access to the element at a specific column and row.
 * parameters: The UArray2_T handle and the (col, row) coordinates.
 * return:     A pointer to the element at the specified index.
 * expect:     uarray2 is non-null; col is in [0, width); row is in [0, height).
*/
extern void *UArray2_at(T uarray2, int col, int row);

/* 
 * name:       UArray2_apply
 * purpose:    A client-supplied function that defines an operation to be 
 *             performed on a single element in the 2D array.
 * parameters: 
 *             col, row: The current coordinates of the element being visited.
 *             uarray2:  The handle to the 2D array being mapped.
 *             elem:     A pointer to the actual data stored at (col, row).
 *             cl:       A pointer to client-specific data (the "closure").
 * expect:     The client must cast 'elem' to the correct pointer type 
 *             matching the array's contents before dereferencing.
 */
typedef void UArray2_apply(int col, int row, T uarray2, void *elem, void *cl);


/* Mapping Funcs */

/*
 * name:       UArray2_map_row_major
 * purpose:    Visits every element in the array in row-major order.
 * parameters: The UArray2_T, an apply function, and a client data pointer.
 * return:     none
 * expect:     uarray2 and apply are non-null.
 * effect:     Calls apply for every element, 
 *             iterating through columns for each row.
*/
extern void UArray2_map_row_major(T uarray2, UArray2_apply apply, void *cl);

/*
 * name:       UArray2_map_col_major
 * purpose:    Visits every element in the array in column-major order.
 * parameters: The UArray2_T, an apply function, and a client data pointer.
 * return:     none
 * expect:     uarray2 and apply are non-null.
 * effect:     Calls apply for every element, 
 *             iterating through rows for each column.
*/
extern void UArray2_map_col_major(T uarray2, UArray2_apply apply, void *cl);

#undef T
#endif

/* @val __ adetoye*/