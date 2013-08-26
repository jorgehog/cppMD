#ifndef ENS_DEFINES_H
#define ENS_DEFINES_H

#define ENS_DIM 2

#define ENS_NX 100
#define ENS_NY 100

#if ENS_DIM == 3

#define ENS_NZ 100
#define ENS_N ENS_NX*ENS_NY*ENS_NZ

#else

#define ENS_N ENS_NX*ENS_NY

#define ENS_PERIODIC_X
#define ENS_PERIODIC_Y


#endif

#endif // DEFINES_H
