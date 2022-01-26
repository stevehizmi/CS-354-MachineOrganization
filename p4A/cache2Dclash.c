/////////////////////////////////////////////////////////////////////////////
// Main File: cache2Dclash.c
// This File: cache2Dclash.c
// Semester: CS 354 Spring 2020
//
// Author: Steven Hizmi
// Email: shizmi@wisc.edu
// CS Login: shizmi
////////////////////////////////////////////////////////////////////////////

int arr2D[128][8]; // array of 128 rows and 8 col

int main(){
	for(int iteration = 0; iteration < 100; iteration++){
		for (int row = 0; row < 128; row += 64){
			for(int col = 0; col < 8; col++){
				arr2D[row][col] = iteration + row + col;
			}	
		}
	}
}
