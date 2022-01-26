////////////////////////////////////////////////////////////////////////
// Main File: cache2Dcols.c
// This File: cache2Dcols.c
// Semester: CS 354 Spring 2020
//
// Author: Steven Hizmi
// Email: shizmi@wisc.edu
// CS Login: shizmi
//
////////////////////////////////////////////////////////////////////////


int arr2D[3000][500]; // array of 3000 rows and 500 col


int main(){
	for (int col = 0; col < 500; col++){
		for(int row = 0; row < 3000; row++){
			arr2D[row][col] = col + row;
		}
	}
}
