#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <semaphore.h>
#include <ncurses.h>

#define max_descanso 5
#define OCCUPED 1
#define FREE 2