///////////////////////////////////////////////////////////////////////////////
//
// Copyright 2019-2020 Jim Skrentny
// Posting or sharing this file is prohibited, including any changes/additions.
//
///////////////////////////////////////////////////////////////////////////////
 
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdio.h>
#include <string.h>
#include "heapAlloc.h"
 
/*
 * This structure serves as the header for each allocated and free block.
 * It also serves as the footer for each free block but only containing size.
 */
typedef struct blockHeader {           
    int size_status;
    /*
    * Size of the block is always a multiple of 8.
    * Size is stored in all block headers and free block footers.
    *
    * Status is stored only in headers using the two least significant bits.
    *   Bit0 => least significant bit, last bit
    *   Bit0 == 0 => free block
    *   Bit0 == 1 => allocated block
    *
    *   Bit1 => second last bit 
    *   Bit1 == 0 => previous block is free
    *   Bit1 == 1 => previous block is allocated
    * 
    * End Mark: 
    *  The end of the available memory is indicated using a size_status of 1.
    * 
    * Examples:
    * 
    * 1. Allocated block of size 24 bytes:
    *    Header:
    *      If the previous block is allocated, size_status should be 27
    *      If the previous block is free, size_status should be 25
    * 
    * 2. Free block of size 24 bytes:
    *    Header:
    *      If the previous block is allocated, size_status should be 26
    *      If the previous block is free, size_status should be 24
    *    Footer:
    *      size_status should be 24
    */
} blockHeader;         

/* Global variable - DO NOT CHANGE. It should always point to the first block,
 * i.e., the block at the lowest address.
 */
blockHeader *heapStart = NULL;     

/* Size of heap allocation padded to round to nearest page size.
 */
int allocsize;

/*
 * Additional global variables may be added as needed below
 */
blockHeader *prev_alloc = NULL; // stores previously allocated block
blockHeader *end = NULL; // end of heap
 
/* 
 * Function for allocating 'size' bytes of heap memory.
 * Argument size: requested size for the payload
 * Returns address of allocated block on success.
 * Returns NULL on failure.
 * This function should:
 * - Check size - Return NULL if not positive or if larger than heap space.
 * - Determine block size rounding up to a multiple of 8 and possibly adding padding as a result.
 * - Use NEXT-FIT PLACEMENT POLICY to chose a free block
 * - Use SPLITTING to divide the chosen free block into two if it is too large.
 * - Update header(s) and footer as needed.
 * Tips: Be careful with pointer arithmetic and scale factors.
 */
void* allocHeap(int size) {     
    //TODO: Your code goes in here.
   // if size is less than 1, return NULL
   if (size < 1){
   	return NULL;
   }
   // add padding for block header
   int start_size = size + sizeof(blockHeader);
   
   // if block size is not a multiple of 8, then add padding
   // to ensure double word addressibility
   if(start_size % 8 != 0){
   	start_size = start_size + (8 - (start_size % 8)); 
   }

   // if heap is not initialized return NULL
   if (heapStart == NULL){
   	return NULL;
   }
   blockHeader *newBlock = NULL;

   // check if first allocation
   if (prev_alloc == NULL){
   	prev_alloc = heapStart;
	end = (blockHeader*)((void*)heapStart + heapStart->size_status - heapStart->size_status % 8);
   }
   blockHeader *current_block = prev_alloc;
   
   // find where to allocate next block based on next_fit placement policy
   while((current_block->size_status - (current_block->size_status % 8)) < start_size || (current_block->size_status % 2) != 0){
   	
	current_block = (blockHeader*)((void*)current_block + current_block->size_status - current_block->size_status % 8);
	
	// check if loop has made it through every block in heap
	if(current_block == prev_alloc){
		return NULL;
	}
	
	// check to see if loop has made it to end of heap, if so, start from beginning
	if(current_block->size_status == 1){
		
		current_block = heapStart;
		
		if(current_block == prev_alloc){
			return NULL;
		}
	}
   
   }
	
   int curr_block_size = current_block->size_status - current_block->size_status%8;
   int p_bit = current_block->size_status%8;
   
   // if current block size is odd then return null as it was allocated incorrectly
   if(curr_block_size < start_size || current_block->size_status%2 == 1){
   	return NULL;
   }
   // for splitting
   if(start_size < curr_block_size){
   	newBlock = (blockHeader*)((void*)current_block + start_size);
	newBlock ->size_status = curr_block_size + 2 - start_size;
   }
   // updates footer
   blockHeader *footer = (blockHeader*)((char*)current_block + curr_block_size - sizeof(blockHeader));
   if(start_size == curr_block_size){
   	footer = NULL;
   }
   else {
   	footer->size_status = curr_block_size - start_size;
	}
	
   // update current block
   current_block->size_status = start_size + p_bit + 1;
   // update previously allocated block with current block
   prev_alloc = current_block;

   

   // return pointer to payload
   return ((void*)current_block + sizeof(blockHeader));
} 
 
/* 
 * Function for freeing up a previously allocated block.
 * Argument ptr: address of the block to be freed up.
 * Returns 0 on success.
 * Returns -1 on failure.
 * This function should:
 * - Return -1 if ptr is NULL.
 * - Return -1 if ptr is not a multiple of 8.
 * - Return -1 if ptr is outside of the heap space.
 * - Return -1 if ptr block is already freed.
 * - USE IMMEDIATE COALESCING if one or both of the adjacent neighbors are free.
 * - Update header(s) and footer as needed.
 */                    
int freeHeap(void *ptr) {    
    //TODO: Your code goes in here.
  	
    // if ptr is null return -1
    if(ptr == NULL){
    	return -1;
    }
    // check if ptr is a multiple of 8
    if((int)&ptr % 8 != 0){
    	return -1;
    }
    // check if ptr is outside of heap space
    if(ptr < (void*)heapStart || ptr > (void*)end){
    	return -1;
    }
    // check if it has already been freed
    if(((blockHeader*)((void*)ptr-sizeof(blockHeader)))->size_status%2 != 1){
    	 return -1;
    }

    blockHeader *current_block = (blockHeader*)(ptr-sizeof(blockHeader));
    int curr_block_size = current_block->size_status - current_block->size_status % 8;

    // check backside by checking p-bit to see if prev block is alloc'd
    if (current_block->size_status - (current_block->size_status%2) == 0){
    	blockHeader *prev_foot = (blockHeader*)((void*)current_block - sizeof(blockHeader));
	blockHeader *prev_block = (blockHeader*)((void*)current_block - prev_foot->size_status);
	curr_block_size += prev_foot->size_status;
	current_block = prev_block;
	current_block->size_status = curr_block_size + prev_block->size_status % 8;
	current_block = current_block - current_block->size_status % 2;
    }

    blockHeader *next_block = (blockHeader*)((void*) current_block + curr_block_size);
    // check front by checking p-bit of next block
    if(next_block->size_status % 2 == 0){
    	curr_block_size = curr_block_size + next_block->size_status - next_block->size_status % 8;
	current_block->size_status = curr_block_size + current_block->size_status % 8;
	current_block->size_status = current_block->size_status - current_block->size_status % 2;

    }

    // change header of curr block to represent freedom
    current_block->size_status = current_block->size_status - 1;

    //footer
    blockHeader *footer = (blockHeader*)((char*) current_block + curr_block_size - sizeof(blockHeader));
    footer -> size_status = curr_block_size;

    // change p bit of next block
    blockHeader *next = (blockHeader*)((void*)current_block + current_block->size_status - current_block->size_status % 8);
    if(next->size_status - (next->size_status % 2) != 0){
    	next->size_status = next->size_status - 2;
    }
    // change prev allocated if needed	
    if(current_block == prev_alloc){
    	prev_alloc = current_block;
    }

    return 0;
} 
 
/*

 * Function used to initialize the memory allocator.
 * Intended to be called ONLY once by a program.
 * Argument sizeOfRegion: the size of the heap space to be allocated.
 * Returns 0 on success.
 * Returns -1 on failure.
 */                    
int initHeap(int sizeOfRegion) {    
 
    static int allocated_once = 0; //prevent multiple initHeap calls
 
    int pagesize;  // page size
    int padsize;   // size of padding when heap size not a multiple of page size
    void* mmap_ptr; // pointer to memory mapped area
    int fd;

    blockHeader* endMark;
  
    if (0 != allocated_once) {
        fprintf(stderr, 
        "Error:mem.c: InitHeap has allocated space during a previous call\n");
        return -1;
    }
    if (sizeOfRegion <= 0) {
        fprintf(stderr, "Error:mem.c: Requested block size is not positive\n");
        return -1;
    }

    // Get the pagesize
    pagesize = getpagesize();

    // Calculate padsize as the padding required to round up sizeOfRegion 
    // to a multiple of pagesize
    padsize = sizeOfRegion % pagesize;
    padsize = (pagesize - padsize) % pagesize;

    allocsize = sizeOfRegion + padsize;

    // Using mmap to allocate memory
    fd = open("/dev/zero", O_RDWR);
    if (-1 == fd) {
        fprintf(stderr, "Error:mem.c: Cannot open /dev/zero\n");
        return -1;
    }
    mmap_ptr = mmap(NULL, allocsize, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
    if (MAP_FAILED == mmap_ptr) {
        fprintf(stderr, "Error:mem.c: mmap cannot allocate space\n");
        allocated_once = 0;
        return -1;
    }
  
    allocated_once = 1;

    // for double word alignment and end mark
    allocsize -= 8;

    // Initially there is only one big free block in the heap.
    // Skip first 4 bytes for double word alignment requirement.
    heapStart = (blockHeader*) mmap_ptr + 1;

    // Set the end mark
    endMark = (blockHeader*)((void*)heapStart + allocsize);
    endMark->size_status = 1;

    // Set size in header
    heapStart->size_status = allocsize;

    // Set p-bit as allocated in header
    // note a-bit left at 0 for free
    heapStart->size_status += 2;

    // Set the footer
    blockHeader *footer = (blockHeader*) ((void*)heapStart + allocsize - 4);
    footer->size_status = allocsize;
  
    return 0;
} 
                  
/* 
 * Function to be used for DEBUGGING to help you visualize your heap structure.
 * Prints out a list of all the blocks including this information:
 * No.      : serial number of the block 
 * Status   : free/used (allocated)
 * Prev     : status of previous block free/used (allocated)
 * t_Begin  : address of the first byte in the block (where the header starts) 
 * t_End    : address of the last byte in the block 
 * t_Size   : size of the block as stored in the block header
 */                     
void dumpMem() {     
 
    int counter;
    char status[5];
    char p_status[5];
    char *t_begin = NULL;
    char *t_end   = NULL;
    int t_size;

    blockHeader *current = heapStart;
    counter = 1;

    int used_size = 0;
    int free_size = 0;
    int is_used   = -1;

    fprintf(stdout, "************************************Block list***\
                    ********************************\n");
    fprintf(stdout, "No.\tStatus\tPrev\tt_Begin\t\tt_End\t\tt_Size\n");
    fprintf(stdout, "-------------------------------------------------\
                    --------------------------------\n");
  
    while (current->size_status != 1) {
        t_begin = (char*)current;
        t_size = current->size_status;
    
        if (t_size & 1) {
            // LSB = 1 => used block
            strcpy(status, "used");
            is_used = 1;
            t_size = t_size - 1;
        } else {
            strcpy(status, "Free");
            is_used = 0;
        }

        if (t_size & 2) {
            strcpy(p_status, "used");
            t_size = t_size - 2;
        } else {
            strcpy(p_status, "Free");
        }

        if (is_used) 
            used_size += t_size;
        else 
            free_size += t_size;

        t_end = t_begin + t_size - 1;
    
        fprintf(stdout, "%d\t%s\t%s\t0x%08lx\t0x%08lx\t%d\n", counter, status, 
        p_status, (unsigned long int)t_begin, (unsigned long int)t_end, t_size);
    
        current = (blockHeader*)((char*)current + t_size);
        counter = counter + 1;
    }

    fprintf(stdout, "---------------------------------------------------\
                    ------------------------------\n");
    fprintf(stdout, "***************************************************\
                    ******************************\n");
    fprintf(stdout, "Total used size = %d\n", used_size);
    fprintf(stdout, "Total free size = %d\n", free_size);
    fprintf(stdout, "Total size = %d\n", used_size + free_size);
    fprintf(stdout, "***************************************************\
                    ******************************\n");
    fflush(stdout);

    return;  
} 
