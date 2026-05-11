/*
    *   Homework 3
    *   uarray2b.c
    *   this is an implementation of uarray2b, it's a blocked version of uarray2
    *   Feb, 16 2026
    *   Toye Adebayo [aadeb02], Valens Dabagire [vdabag01]
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include "uarray.h"
#include "uarray2.h"
#include "uarray2b.h"

#define T UArray2b_T


/*
 * name:        UArray2b_T
 * purpose:     Defines the structure for a blocked 2D array.
 * parameters:  width: the total number of columns in the array
 *              height: the total number of rows in the array
 *              size: the size in bytes of each element
 *              blocksize: the side length of each square block
 * return:      None. 
*/
struct T {
        int width;
        int height;
        int size;
        int blocksize;
        UArray2_T blocks; 
};

/*
 * name:       div_ceil
 * purpose:    Performs integer division and returns the ceiling of the result.
 *             This is used to determine how many blocks are needed to cover 
 *             a given dimension, including partial blocks at the edges.
 * parameters: n: the numerator (e.g., total width or height)
 *             d: the denominator (e.g., blocksize)
 * return:     An integer representing ceil(n / d).
 * expect:     d is greater than 0.
 * notes:      This is a static helper function local to uarray2b.c.
*/
static int div_ceil(int n, int d) 
{
        assert(d > 0);
        return (n + d - 1) / d;
}


/*
 * name:       UArray2b_new
 * purpose:    Allocates and initializes a new blocked 2D array.
 * parameters: width and height of the grid, size in bytes of each element,
 *             and the blocksize (side of the square block).
 * return:     A handle (pointer) to the new UArray2b_T.
 * expect:     width, height, and blocksize are positive; size is positive.
 *             Allocation succeeds.
*/
T UArray2b_new(int width, int height, int size, int blocksize)
{
        assert(blocksize >= 1);
        T array2b = malloc(sizeof(*array2b));
        assert(array2b != NULL);

        array2b->width = width;
        array2b->height = height;
        array2b->size = size;
        array2b->blocksize = blocksize;

        int b_width = div_ceil(width, blocksize);
        int b_height = div_ceil(height, blocksize);

        /* The outer shell is a UArray2 where each element is a 
        UArray_T block */
        array2b->blocks = UArray2_new(b_width, b_height, sizeof(UArray_T));

        for (int r = 0; r < b_height; r++) {
                for (int c = 0; c < b_width; c++) {
                        UArray_T *block =
                                UArray2_at(array2b->blocks, c, r);
                        *block = UArray_new(blocksize * blocksize, size);
                }
        }

        return array2b;
}

/*
 * name:       UArray2b_new_64K_block
 * purpose:    Allocates a new blocked 2D array with a default blocksize
 *             that allows one block to fit within roughly 64KB of RAM.
 * parameters: width and height of the grid, and size in bytes of each element.
 * return:     A handle to the new UArray2b_T.
 * expect:     width, height, and size are positive.
*/
T UArray2b_new_64K_block(int width, int height, int size)
{
        int blocksize = (int)sqrt(65536 / size);
        if (blocksize < 1) {
                blocksize = 1;
        }
        return UArray2b_new(width, height, size, blocksize);
}

/*
 * name:       UArray2b_free
 * purpose:    Deallocates memory for the blocked array and all internal 
 *             blocks.
 * parameters: A pointer to the UArray2b_T handle.
 * return:     none
 * expect:     array2b and *array2b are non-null.
*/
void UArray2b_free(T *array2b)
{
        assert(array2b != NULL && *array2b != NULL);
        int b_width = UArray2_width((*array2b)->blocks);
        int b_height = UArray2_height((*array2b)->blocks);

        for (int r = 0; r < b_height; r++) {
                for (int c = 0; c < b_width; c++) {
                        UArray_T *block =
                                UArray2_at((*array2b)->blocks, c, r);
                        UArray_free(block);
                }
        }
        UArray2_free(&(*array2b)->blocks);
        free(*array2b);
        *array2b = NULL;
}

/*
 * name:       UArray2b_width / height / size / blocksize
 * purpose:    Returns the respective attribute of the blocked array.
 * parameters: The UArray2b_T handle.
 * return:     The requested integer attribute.
 * expect:     array2b is non-null.
*/
int UArray2b_width(T array2b)    
{ 
        assert(array2b); 
        return array2b->width; 
}

int UArray2b_height(T array2b)   
{ 
        assert(array2b); 
        return array2b->height; 
}

int UArray2b_size(T array2b)     
{ 
        assert(array2b); 
        return array2b->size; 
}

int UArray2b_blocksize(T array2b)
{ 
        assert(array2b); 
        return array2b->blocksize; 
}

/*
 * name:       UArray2b_at
 * purpose:    Provides access to the element at a specific column and row.
 * parameters: The UArray2b_T handle and the (col, row) coordinates.
 * return:     A pointer to the element.
 * expect:     array2b non-null; coordinates within [0, width) and [0, height).
*/
void *UArray2b_at(T array2b, int column, int row)
{
        assert(array2b != NULL);
        assert(column >= 0 && column < array2b->width);
        assert(row >= 0 && row < array2b->height);

        int bs = array2b->blocksize;
        UArray_T *block =
                UArray2_at(array2b->blocks, column / bs, row / bs);
    
        // Calculate the index within the block using modulo to find 
        // the position within the block
        int index = bs * (column % bs) + (row % bs);
        return UArray_at(*block, index);
}

/*
 * name:       UArray2b_map
 * purpose:    Visits every cell in the array, block by block.
 * parameters: The UArray2b_T handle, an apply function, and a closure pointer.
 * return:     none
 * expect:     array2b and apply are non-null.
 * effect:     Calls apply for every valid cell in the array. Unused cells in
 *             padding blocks are not visited.
*/
void UArray2b_map(
        T array2b, 
        void apply(
                int col, 
                int row, 
                T array2b, 
                void *elem, 
                void *cl
        ), 
        void *cl)
{
        assert(array2b != NULL);
        int b_width = UArray2_width(array2b->blocks);
        int b_height = UArray2_height(array2b->blocks);
        int bs = array2b->blocksize;

        for (int br = 0; br < b_height; br++) {
                for (int bc = 0; bc < b_width; bc++) {
                        UArray_T *block =
                                UArray2_at(array2b->blocks, bc, br);
            
                        for (int i = 0; i < bs; i++) {       
                                for (int j = 0; j < bs; j++) {   
                                        int col = bc * bs + i;
                                        int row = br * bs + j;

                                        if (col < array2b->width &&
                                            row < array2b->height) {
                                                apply(col, row, array2b, 
                                                     UArray_at(*block,
                                                               bs * i + j),
                                                     cl);
                                        }
                                }
                        }
                }
        }
}

#undef T


/* @val __ adetoye*/