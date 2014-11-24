/*CS 460 Fall 2014 Project 1 Part 2
 *Author: Lowell Olson
 *Development time 48 hours
 */

#include <stdio.h>
#include <stdlib.h>
#include "syn.h"
#include "SetLimits.h"

int main(int argc, char *argv[]) {
    SetLimits();
    if (argc < 2) {
        printf("format: proj1 <filename>\n");
        exit(1);
    }
    start(argv[1]);
    return 0;
}
