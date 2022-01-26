///////////////////////////////////////////////////////////////////////////////
//
// Copyright 2020 Jim Skrentny
// Posting or sharing this file is prohibited, including any changes/additions.
//
////////////////////////////////////////////////////////////////////////////////
// Main File:        makeMagic.c
// This File:        makeMagic.c
// Other Files:      None
// Semester:         CS 354 Spring 2020
//
// Author:           Steven Hizmi
// Email:            shizmi@wisc.edu
// CS Login:         shizmi
//
/////////////////////////// OTHER SOURCES OF HELP //////////////////////////////
//                   Fully acknowledge and credit all sources of help,
//                   other than Instructors and TAs.
//
// Persons:         None
//
// Online sources:   https://www.geeksforgeeks.org/magic-square/ 
//                   used descriptions of magic square algorthim on top to help
//                   in writing generateMSquare method
////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure that represents a magic square
typedef struct {
    int size;     // dimension of the square
    int **msquare; // pointer to heap allocated magic square
} MSquare;


 /* getSize() function returns dimensions of the magic square only 
 * if it is >= 3 and an odd number
 *
 * @return value (int) of magic square size if conditions are met
 */
int getSize() {
    
    int size;
    // prompt user for size of board
    printf("Enter magic square's size (odd integer >=3)\n");
    // read user input and store in variable 'size'
    scanf("%i", &size);
    
    // if size is not an odd number exit 
    if((size%2) != 1){
        printf("Size must be odd.");
        exit(1);
    }
    // if size is not >= to 3 then exit
    if(size < 3){
        printf("Size must be >= 3.");
        exit(1);
    }
    return size;
}


 /* generateMSquare(int n) generates a 2d array of size n filled with numbers and returns a 
 *  MSquare structure pointer
 *
 * @param int n: dimension of magic square
 * @return MSquare: returns a pointer to a MSquare Structure
 */
MSquare *generateMSquare(int n) {
    int size = n;
    
    // check if malloc return null
    if(malloc(sizeof(MSquare)) == NULL){
        printf("initialize MSquare struct error");
        exit(1);
    }
    
    MSquare *square = malloc(sizeof(MSquare));
    
    // initialize magic square board
    int **board;
    
    // check if malloc returns null for 1d allocation
    if(malloc(sizeof(int*)) == NULL){
        printf("1D allocation failed");
        exit(1);
    }
    
    // check if malloc returns null for 2d allocation
    for(int i = 0; i<size; i++){
        if(malloc(sizeof(int) * size) == NULL){
            printf("2D allocation failed");
            exit(1);
        }
        *(board + i) = malloc(sizeof(int) * size);
    }
    
    int i = size/2; // middle row
    int j = size-1; // last col
    
    for(int counter = 0; counter < size*size;){
       if (i==-1 && j==n) // wrap around
        { 
            j = n-2; 
            i = 0; 
        } 
        else
        { 
            // wrap around right side if needed
            if (j == n) 
                j = 0; 
  
            // wrap around top if needed
            if (i < 0) 
                i=n-1; 
        } 
        if ( (*(*(board + i)+j)) ) // if number is already there keep going
        { 
            j -= 2; 
            i++; 
            continue; 
        } 
        else
            // assign number
            (*(*(board + i)+j)) = counter++; 
  
        j++; i--;
    }
    square -> msquare = board;
    return *square;
}


 /* outputMSquare writes the magicSquare to a file provided as a parameter
 *
 * @param *msquare: is the structs contents that will be written to the file
 * @param *filename: name of file to write to, overwrites or creates new file
 *
 */
void outputMSquare(MSquare *msquare, char *filename) {
    
    if (fopen(filename, "w") == NULL){
   	printf("error in opening file for writing");
	exit(1);
    }	    
    //open file for writing
    fopen(filename, "w");
    for(int i = 0; i < msquare-> size; i++) {
        
        for(int j = 0; j < msquare-> size; j++) {
            
            // print 2D array contents in file 
            fprintf( filename, msquare->*(*(msquare + i)+ j) );
            if(j == msquare-> size){
                fprintf(filename, "\n");
         }
      }
   if(fclose(filename) == NULL){
  	printf("error in closing file");
	exit(1);
   }
       	fclose(filename);
}

/* main method that generates user input for creating the magic square
 *
 * @param argc: number of command line arguments
 * @param argv: each command line argument is added to an array
 * @return 0 if no error
 */
int main(int argc, char *argv[]) {
    if (malloc(sizeof(MSquare)) == NULL){
        printf("generating square failed");
        exit(1);
    }
    MSquare *square = malloc(sizeof(MSquare));
    
    // Check input arguments to get output filename
    FILE *filename = *(argv + 2);
    
    // Get magin square's size from user
    square -> size = getSize();
    
    // Generate the magic square
    square -> msquare = generateMSquare(square-> size);
    
    // Output the magic square to file
    outputMSquare(&square, &filename);
    
    return 0;
}
   
