#ifndef ENS_DEFINES_H
#define ENS_DEFINES_H

#define ENS_DIM 2


#define ENS_NX 20
#define ENS_NY 20

/*
 *
 */

#if ENS_DIM == 3

#define ENS_NZ 100
#define ENS_N ENS_NX*ENS_NY*ENS_NZ

#else

#define ENS_N ENS_NX*ENS_NY

#endif

/*
 *
 */

#define ENS_PERIODIC_X
#define ENS_PERIODIC_Y



/*
 *
 */

#if ENS_DIM == 2 && defined (ENS_PERIODIC_Z)
#undef ENS_PERIODIC_Z
#endif

#define quickPrint(expr) \
    std::cout << expr << std::endl

#define IGNIS_X 0
#define IGNIS_Y 1
#define IGNIS_Z 2

#endif // DEFINES_H
