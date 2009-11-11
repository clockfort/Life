/* 
 * File:   main.cpp
 * Author: Chris Lockfort, devnull@csh.rit.edu
 *
 * Quick little Game of Life implementation that I made when I got bored
 * in an introductory-level networking class.
 *
 * Its meant to be rather memory effecient, as you can see through the use of
 * bit-packing the gameboard and a buffer rather than implementing a second
 * copy of the entire matrix, each of which allows for _significant_ reduction
 * in memory footprint.
 *
 * I wanted to make it memory efficient so it could be used later on a
 * microprocessor, possibly for my own projects or a CSH project called
 * "Big Infosys", which is a giant LED matrix.
 *
 * todo:
 *  - Clean up code
 *  - Possibly add "wrapping" support to the game board.
 *  - Decide on a microcontroller setup and program this more specifically
 * 
 * Created on November 4, 2009, 10:55 AM
 */

#include <stdlib.h>
#include <iostream>
#include "life.h"

const int HEIGHT=20;
const int WIDTH=30;
unsigned char next[2][WIDTH/8+1];
unsigned char internalBoard[HEIGHT][WIDTH/8+1];

using namespace std;

int main(int argc, char** argv) {
    int generation;
    bool notEmpty;
    while(true){
        randomFill();
        generation=0;
        notEmpty=true;
        while(generation <= 80 && notEmpty){
            cout << "Generation: " << generation++ << endl;
            notEmpty = printBoard();
            tick();
            usleep(50000);
        }
    }
    return (EXIT_SUCCESS);
}


char getBoard(int height, int width){
    return (internalBoard[height][width/8] >> width%8) & 1;
}


void setBoard(int height, int width, int newValue){
    if(newValue==1)
        internalBoard[height][width/8]=internalBoard[height][width/8] | (1<<(width%8));
    else
        internalBoard[height][width/8]=internalBoard[height][width/8] & ~(1<<(width%8));
}


char getNext(int height, int width){
    return (next[height][width/8] >> width%8) & 1;
}


void setNext(int height, int width, int newValue){
     if(newValue==1)
        next[height][width/8]=next[height][width/8] | (1<<(width%8));
    else
        next[height][width/8]=next[height][width/8] & ~(1<<(width%8));
}


void tick(){
    int neighbors;
    for(int row=0; row<HEIGHT; row++){
        for(int col=0; col<WIDTH; col++){
            neighbors=0;
            if(row>0){
                if(getBoard(row-1, col)==1)
                    ++neighbors;
                if(col>0){
                    if(getBoard(row-1, col-1)==1)
                        ++neighbors;
                }
            }
            if(row<(HEIGHT-1)){
                if(getBoard(row+1, col)==1)
                    ++neighbors;
                if(col<(WIDTH-1))
                    if(getBoard(row+1, col+1)==1)
                        ++neighbors;
            }
            if(col>0){
                if(getBoard(row, col-1)==1)
                    ++neighbors;
                if(row<(HEIGHT-1)){
                    if(getBoard(row+1, col-1)==1)
                        ++neighbors;
                }
            }
            if(col<(WIDTH-1)){
                if(getBoard(row, col+1)==1)
                    ++neighbors;
                if(row>0){
                    if(getBoard(row-1, col+1)==1)
                        ++neighbors;
                }
            }

        if(getBoard(row,col)==1)
            switch(neighbors){
                case 0://starvation
                case 1:
                    setNext(1,col,0);
                    break;
                case 2:
                case 3:
                    setNext(1,col,1);
                    break;
                case 4://overcrowding
                case 5:
                case 6:
                case 7:
                case 8:
                    setNext(1,col,0);
            }
        else if(neighbors==3)//necromancy
            setNext(1,col,1);
        else
            setNext(1,col,0);
        }

        if(row==0){
            for(int col=0; col<WIDTH; col++)
                setNext(0, col, getNext(1, col));
        }
        else if(row!=0 && row!=HEIGHT-1){
            //time to flush the buffer
            for(int col=0; col<WIDTH; col++){
                    setBoard(row-1, col, getNext(0,col));
                    setNext(0,col,getNext(1,col));
            }
        }
        else if(row==HEIGHT-1){//we need to flush here too
            for(int col=0; col<WIDTH; ++col){
                setBoard(row-1,col,getNext(0,col));
                setBoard(row, col, getNext(1,col));
            }
        }
    }
}

bool printBoard(){
    bool notEmpty=false;
    for(int i=0; i<HEIGHT; i++){
        for(int j=0; j<WIDTH; j++){
            if(getBoard(i,j)==1){
                cout << (char) (getBoard(i,j)+63) << " ";
                notEmpty=true;
            }
            else
                cout << "  ";
        }
        cout << endl;
    }
    return notEmpty;
}

void randomFill(){
    srand( time(NULL) );
    for(int i=0; i<HEIGHT; i++)
        for(int j=0; j<WIDTH; j++)
            setBoard(i,j,rand()%2);
}

void gliderFill(){
    for(int i=0; i<HEIGHT; i++)
        for(int j=0; j<WIDTH; j++)
            setBoard(i,j,0);
        
    setBoard(0,1,1);
    setBoard(1,2,1);
    setBoard(2,0,1);
    setBoard(2,1,1);
    setBoard(2,2,1);

    setBoard(0, 11, 1);
    setBoard(1, 12, 1);
    setBoard(2, 10, 1);
    setBoard(2, 11, 1);
    setBoard(2, 12, 1);
}
