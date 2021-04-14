/**********************************************
File Name: something_cool.c
Author: Tyler Punch
Date: 9/20/2019

Purpose: This is the code from the hello_world.c
file which will be modified to perform 'something
cool'. My something cool is to both print a specifed
number of lines based on a command arguement and
each line is printed in a different color.
**********************************************/
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {

	int num_lines = atoi(argv[1]); // Get user input
	int color = 31;	// Initialize color variable

	for(int i = 0; i < num_lines; i++) {

		// This will print my message and change
		// the color every time.
		printf("\033[0;%dm", color);
		printf("ECE 471 First time using an RPi.\n");
		(color == 36 ? color = 31 : color++); 
	}
		return 0;
}
