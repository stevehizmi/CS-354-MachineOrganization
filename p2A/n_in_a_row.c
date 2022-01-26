///////////////////////////////////////////////////////////////////////////////
//
// Copyright 2020 Jim Skrentny
// Posting or sharing this file is prohibited, including any changes/additions.
//
////////////////////////////////////////////////////////////////////////////////
// Main File:        n_in_a_row.c
// This File:        n_in_a_row.c
// Other Files:      none
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
// Persons:          Identify persons by name, relationship to you, and email.
//                   Describe in detail the the ideas and help they provided.
//
// Online sources:   Avoid web searches to solve your problems, but if you do
//                   search, be sure to include Web URLs and description of
//                   of any information you find.
////////////////////////////////////////////////////////////////////////////////
   
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
     
char *COMMA = ",";  
     
/* COMPLETED:       
 * Retrieves from the first line of the input file,
 * the size of the board (number of rows and columns).
 * 
 * fp: file pointer for input file
 * size: pointer to size
 */
void get_dimensions(FILE *fp, int *size) {      
    char *line = NULL;
    size_t len = 0;
    if (getline(&line, &len, fp) == -1) {
        printf("Error in reading the file.\n");
        exit(1);
    }

    char *token = NULL;
    token = strtok(line, COMMA);
    *size = atoi(token);
}       
 
   
  
/* TODO:
 * Returns 1 if and only if the board is in a valid state.
 * Otherwise returns 0.
 * 
 * board: heap allocated 2D board
 * size: number of rows and columns
 */
int n_in_a_row(int **board, int size) {

    // variable declarations
    int xwins = 0;
    int owins = 0;
    int count; // sums up the values -1 for 0 and 1 for x
    int xcount = 0;
    int ocount = 0;

    // checking rows
    for (int i = 0; i < size; i++) {
        count = 0;
        for (int j = 0; j < size; j++) {
            int x_or_o = *(*(board + i) + j);
            if (x_or_o == 1) count += 1;
            else if (x_or_o == 2) count -= 1;
        }
        if (count == 1 * size) xwins += 1;
        else if (count == -1 * size) owins += 1;
    }

    // checking columns
    for (int i = 0; i < size; i++) {
        count = 0;
        for (int j = 0; j < size; j++) {
            int x_or_o = *(*(board + j) + i);
            if (x_or_o == 1) {
                count += 1;
                xcount++;
            } else if (x_or_o == 2) {
                count -= 1;
                ocount++;
            }
        }
        if (count == 1 * size) xwins += 1;
        else if (count == -1 * size) owins += 1;
    }

    // checking left->right diagonal
    count = 0;
    for (int i = 0; i < size; i++) {
        int x_or_o = *(*(board + i) + i);
        if (x_or_o == 1) count += 1;
        else if (x_or_o == 2) count -= 1;
    }
    if (count == 1 * size) xwins += 1;
    else if (count == -1 * size) owins += 1;

    // checking right->left diagonal
    count = 0;
    for (int i = 0; i < size; i++) {
        int x_or_o = *(*(board + i) + (size - i));
        if (x_or_o == 1) count += 1;
        else if (x_or_o == 2) count -= 1;
    }
    if (count == 1 * size) xwins += 1;
    else if (count == -1 * size) owins += 1;

    // checking for valid number of wins
    if (xwins + owins > 1) return 0;

    // checking for valid number of x and o's
    int xo_diff = xcount - ocount;
    if (xo_diff > 1 || xo_diff < 0) return 0;

    return 1;
}
  
 
   
/* PARTIALLY COMPLETED:
 * This program prints Valid if the input file contains
 * a game board with either 1 or no winners; and where
 * there is at most 1 more X than O.
 * 

 * argc: CLA count
 * argv: CLA value
 */
int main(int argc, char *argv[]) {              
     
    //TODO: Check if number of command-line arguments is correct.
	if(!(argc >= 2)){
		printf( "invalid amount of arguments");
		exit(1);
	}

    //Open the file and check if it opened successfully.
    FILE *fp = fopen(*(argv + 1), "r");
    if (fp == NULL) {
        printf("Can't open file for reading.\n");
        exit(1);
    }

    //Declare local variables.
    int size;
    int valid;
    int **m;

    //TODO: Call get_dimensions to retrieve the board size.
    get_dimensions(fp,&size);

    //TODO: Dynamically allocate a 2D array of dimensions retrieved above.
    m = malloc(sizeof(int*)*size);
    for(int i =0; i <size; i++)
	    *(m+i) = malloc(sizeof(int)*size);

    //Read the file line by line.
    //Tokenize each line wrt comma to store the values in your 2D array.
    char *line = NULL;
    size_t len = 0;
    char *token = NULL;
    for (int i = 0; i < size; i++) {

        if (getline(&line, &len, fp) == -1) {
            printf("Error while reading the file.\n");
            exit(1);
        }

        token = strtok(line, COMMA);
        for (int j = 0; j < size; j++) {
            //TODO: Complete the line of code below
            //to initialize your 2D array.
            /* ADD ARRAY ACCESS CODE HERE */ *(*(m+i)+j) = atoi(token);
            token = strtok(NULL, COMMA);
        }
    }

    //TODO: Call the function n_in_a_row and print the appropriate
    //output depending on the function's return value.
    valid = n_in_a_row(m,size);
    if(valid == 1){
        printf("valid board");
    }
    else{
        printf("invalid board");
    }
    //TODO: Free all dynamically allocated memory.
    for(int i = 0; i <size; i++)
	    free(*(m+i));
    free(m);
    //Close the file.
    if (fclose(fp) != 0) {
        printf("Error while closing the file.\n");
        exit(1);
    } 
     
    return 0;       
}       
