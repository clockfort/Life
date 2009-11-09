/* 
 * File:   main.h
 * Author: devnull
 *
 * Created on November 4, 2009, 11:27 AM
 */

#ifndef _LIFE_H
#define	_LIFE_H

void tick();
void printBoard();
void randomFill();
void gliderFill();
char getBoard(int height, int width);
void setBoard(int height, int width, int newValue);
char getNext(int height, int width);
void setNext(int height, int width, int newValue);

struct bitpack
{
unsigned char a: 1;
unsigned char b: 1;
unsigned char c : 1;
unsigned char d : 1;
unsigned char e : 1;
unsigned char f : 1;
unsigned char g : 1;
unsigned char h : 1;
};

#endif	/* _LIFE_H */

