///////////////////////////////////////////////////////////////////////////
// Main File: cache2Drows.c
// This File: cache2Drows.c
// Semester: CS 354 Spring 2020
//
// Author: Steven Hizmi
// Email: shizmi@wisc.edu
// CS Login shizmi
//
//////////////////////////////////////////////////////////////////////////

int arr2D[3000][500]; // array of 3000 rows and 500 col


int main(){
	for (int row = 0; row < 3000; row++){
		for(int col = 0; col < 500; col++){
			arr2D[row][col] = row + col;
		}
	}
}
