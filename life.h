/* 
 * File:   main.h
 * Author: devnull
 *
 * Created on November 4, 2009, 11:27 AM
 */

#ifndef _LIFE_H
#define	_LIFE_H

void tick();
bool printBoard();
void randomFill();
void gliderFill();
char getBoard(int height, int width);
void setBoard(int height, int width, int newValue);
char getNext(int height, int width);
void setNext(int height, int width, int newValue);

#endif	/* _LIFE_H */

