/**********************************************
Author: Tyler Punch
Date: 9/20/2019

Purpose: This is the hello_world.c file provided by
the instructor. For part 5 of Homework 2 I am required to
modify this file to produce 15 lines of a unique message
of my choosing. 
**********************************************/
#include <stdio.h>

int main(int argc, char **argv) {

	for(int i = 0; i < 15; i++) {
		// Hello World! was updated to my message.
		printf("ECE 471 First time using an RPi.\n");
	}
		return 0;
}
