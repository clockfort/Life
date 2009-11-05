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
unsigned char next[2][WIDTH];
bitpack internalBoard[HEIGHT][WIDTH/8+1];

using namespace std;

int main(int argc, char** argv) {
    int generation;
    while(true){
        randomFill();
        generation=0;
        while(generation <= 80){
            cout << "Generation: " << generation++ << endl;
            printBoard();
            tick();
            usleep(50000);
        }
    }
    return (EXIT_SUCCESS);
}


char get(int height, int width){
    switch(width%8){
        case 0:
            return internalBoard[height][width/8].h;
        case 1:
            return internalBoard[height][width/8].g;
        case 2:
            return internalBoard[height][width/8].f;
        case 3:
            return internalBoard[height][width/8].e;
        case 4:
            return internalBoard[height][width/8].d;
        case 5:
            return internalBoard[height][width/8].c;
        case 6:
            return internalBoard[height][width/8].b;
        case 7:
            return internalBoard[height][width/8].a;
    }
}

void set(int height, int width, int newValue){

    switch(width%8){
        case 0:
            internalBoard[height][width/8].h=newValue;
            break;
        case 1:
            internalBoard[height][width/8].g=newValue;
            break;
        case 2:
            internalBoard[height][width/8].f=newValue;
            break;
        case 3:
            internalBoard[height][width/8].e=newValue;
            break;
        case 4:
            internalBoard[height][width/8].d=newValue;
            break;
        case 5:
            internalBoard[height][width/8].c=newValue;
            break;
        case 6:
            internalBoard[height][width/8].b=newValue;
            break;
        case 7:
            internalBoard[height][width/8].a=newValue;
    }
}

void tick(){
    int neighbors;
    for(int row=0; row<HEIGHT; row++){
        for(int col=0; col<WIDTH; col++){
            neighbors=0;
            if(row>0){
                if(get(row-1, col)==1)
                    ++neighbors;
                if(col>0){
                    if(get(row-1, col-1)==1)
                        ++neighbors;
                }
            }
            if(row<(HEIGHT-1)){
                if(get(row+1, col)==1)
                    ++neighbors;
                if(col<(WIDTH-1))
                    if(get(row+1, col+1)==1)
                        ++neighbors;
            }
            if(col>0){
                if(get(row, col-1)==1)
                    ++neighbors;
                if(row<(HEIGHT-1)){
                    if(get(row+1, col-1)==1)
                        ++neighbors;
                }
            }
            if(col<(WIDTH-1)){
                if(get(row, col+1)==1)
                    ++neighbors;
                if(row>0){
                    if(get(row-1, col+1)==1)
                        ++neighbors;
                }
            }

        if(get(row,col)==1)
            switch(neighbors){
                case 0://starvation
                case 1:
                    next[1][col]=0;
                    break;
                case 2:
                case 3:
                    next[1][col]=1;
                    break;
                case 4://overcrowding
                case 5:
                case 6:
                case 7:
                case 8:
                    next[1][col]=0;
            }
        else if(neighbors==3)//necromancy
            next[1][col]=1;
        else
            next[1][col]=0;
        }

        if(row==0){
            for(int col=0; col<WIDTH; col++)
                next[0][col]=next[1][col];
        }
        else if(row!=0 && row!=HEIGHT-1){
            //time to flush the buffer
            for(int col=0; col<WIDTH; col++){
                    set(row-1, col, next[0][col]);
                    next[0][col]=next[1][col];
            }
        }
        else if(row==HEIGHT-1){//we need to flush here too
            for(int col=0; col<WIDTH; ++col){
                set(row-1,col,next[0][col]);
                set(row, col, next[1][col]);
            }
        }
    }
}

void printBoard(){
    for(int i=0; i<HEIGHT; i++){
        for(int j=0; j<WIDTH; j++){
            if(get(i,j)==1)
                cout << (char) (get(i,j)+63) << " ";
            else
                cout << "  ";
        }
        cout << endl;
    }
}

void randomFill(){
    srand( time(NULL) );
    for(int i=0; i<HEIGHT; i++)
        for(int j=0; j<WIDTH; j++)
            set(i,j,rand()%2);
}

void gliderFill(){
    for(int i=0; i<HEIGHT; i++)
        for(int j=0; j<WIDTH; j++)
            set(i,j,0);
        
    set(0,1,1);
    set(1,2,1);
    set(2,0,1);
    set(2,1,1);
    set(2,2,1);

    set(0, 11, 1);
    set(1, 12, 1);
    set(2, 10, 1);
    set(2, 11, 1);
    set(2, 12, 1);
}
