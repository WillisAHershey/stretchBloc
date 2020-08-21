//Willis A. Hershey

//This file defines a type, stretchBloc, which represents an unsigned integer of effectively no maximum size, as well as a library of functions to 
//perform mathematical operations on and between them

#ifndef STRETCH_BLOC
#define STRETCH_BLOC

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <limits.h>
#include <string.h>
#include <time.h>

//longtype can be redefined as any unsigned integer type without side-effect as long as longtype_max is also properly set
#define LONGTYPE uint_fast32_t
#define LONGTYPE_MAX UINT_FAST32_MAX

//STRETCHBLOC_SUCCESS and STRETCHBLOC_FAILURE are the standard return for all of the int type functions
#define STRETCHBLOC_SUCCESS 0
#define STRETCHBLOC_FAILURE -1

#define SIZE_T_MAX (((size_t)0)-1) //size_ts are tricky in for loops when you count down (beacuse of their unsignedness), so I saved myself some trouble here
#define LONGBYTES sizeof(LONGTYPE)
#define LONGBITS (LONGBYTES*8)

#define PARTPRINT PRIXFAST32
#define WHOLEPRINT "0*"PRIXFAST32

typedef struct{
  LONGTYPE *data;
  size_t numLongs;
}stretchBloc;

char* stretchBlocToString(stretchBloc*,int); //Returns string representing value in given base. Memory for string is malloced and must be freed.

//These functions all return STRETCHBLOC_SUCCESS or STRETCHBLOC_FAILURE except for the boolean functions, which return 1 or 0 as described

//PURE INITIALIZATION FUNCTIONS
int stretchBlocInit(stretchBloc*); //Initializes new stretchBloc to zero in the given location
int copyStretchBloc(stretchBloc*,const stretchBloc*); //Initializes new stretchBloc in given location to value of given stretchBloc
int newStretchBloc(stretchBloc*,LONGTYPE); //Initializes new stretchBloc in given location with value of given longtype
int stringToStretchBloc(stretchBloc*,char*); //Initializes new stretchBloc in given locations with value from null terminated (numerical c-standard) string
int randomStretchBloc(stretchBloc*,size_t); //Initializes new stretchBloc in given location with input number of randomly generated longtypes
//DESTRUCTOR FUNCTIONS
int deleteStretchBloc(stretchBloc*); //Frees the memory allocated by this library for any valid stretchBloc
//HEXADECIMAL PRINTING FUNCTIONS
int printStretchBloc(const stretchBloc*); //prints value of stretchBloc to stdout in hexadecimal
int printlnStretchBloc(const stretchBloc*); //prints value of stretchBloc to stdout with newline
int printDashedStretchBloc(const stretchBloc*); //prints value of stretchBloc to stdout with dashes between longs
int printlnDashedStretchBloc(const stretchBloc*); //prints value of stretchBloc to stdout with dashes between longs and a newline
char* sprintStretchBloc(const stretchBloc*);
//BOOLEAN FUNCTIONS
_Bool stretchBlocTest(const stretchBloc*); //returns 1 if nonzero value and 0 otherwise (Functions as boolean test for value)
int stretchBlocCompare(const stretchBloc*,const stretchBloc*); //Returns negative value if first is less than second, positive if greater, and zero if equal
//MATHEMATICAL INITIALIZATION FUNCTIONS
int stretchBlocPlus(stretchBloc*,const stretchBloc*,const stretchBloc*); //Initializes new stretchBloc with value of sum of input values
int stretchBlocMinus(stretchBloc*,const stretchBloc*,const stretchBloc*); //Initializes new stretchBloc with value of difference of input values, or zero if negative
int stretchBlocTimes(stretchBloc*,const stretchBloc*,const stretchBloc*); //Initializes new stretchBloc with value of product of input values
int stretchBlocDividedBy(stretchBloc*,const stretchBloc*,const stretchBloc*); //Initializes new stretchBloc with value of quotient of input values*
int stretchBlocRemainder(stretchBloc*,const stretchBloc*,const stretchBloc*); //Initializes new stretchBloc with value of remainder of division*
int stretchBlocDivideRemainder(stretchBloc*,stretchBloc*,const stretchBloc*,const stretchBloc*);
int stretchBlocLeftShiftOne(stretchBloc*,const stretchBloc*); //Initializes new stretchBloc with value of input left shifted one place
int stretchBlocRightShiftOne(stretchBloc*,const stretchBloc*); //Initializes new stretchBloc with value of input logically right shifted one place
int stretchBlocLeftShift(stretchBloc*,const stretchBloc*,size_t); //Initializes new stretchBloc with value of input left shifted input places
int stretchBlocRightShift(stretchBloc*,const stretchBloc*,size_t); //Initializes new stretchBloc with value of input right shifted logically input places
int stretchBlocAnd(stretchBloc*,const stretchBloc*,const stretchBloc*); //Initializes new stretchBloc with value of bitwise and of inputs
int stretchBlocOr(stretchBloc*,const stretchBloc*,const stretchBloc*); //Initializes new stretchBloc with value of bitwise or of inputs
int stretchBlocXor(stretchBloc*,const stretchBloc*,const stretchBloc*); //Initializes new stretchBloc with value of bitwise xor of inputs
int stretchBlocPower(stretchBloc*,const stretchBloc*,size_t); //Initializes new stretchBloc with value of input to the power of longtype input
int stretchBlocPOWER(stretchBloc*,const stretchBloc*,const stretchBloc*); //Initializes new stretchBloc with value of input to the power of stretchBloc input
//* dividedBy and remainder also accept an extra pointer where, if not null, the remainder or divisor (respectively) will be stored, as it is computed as a byproduct
//MUTATOR FUNCTIONS
int stretchBlocPlusPlus(stretchBloc*); //increments value of input by one
int stretchBlocMinusMinus(stretchBloc*); //decrements value of input by one
int stretchBlocPlusEquals(stretchBloc*,const stretchBloc*); //increments value of first by value of second
int stretchBlocMinusEquals(stretchBloc*,const stretchBloc*); //decrements value of first by value of second, will set to zero if difference is negative
int stretchBlocTimesEquals(stretchBloc*,const stretchBloc*); //sets value of first to the product of the two values
int stretchBlocDividedByEquals(stretchBloc*,const stretchBloc*,stretchBloc*); //sets value of first to the quotient of the first and second
int stretchBlocRemainderEquals(stretchBloc*,const stretchBloc*,stretchBloc*); //sets value of first to the remainder of division of the fisrt and second
int stretchBlocLeftShiftEqualsOne(stretchBloc*); //shifts the value left by one
int stretchBlocRightShiftEqualsOne(stretchBloc*); //shifts the value right logically by one
int stretchBlocLeftShiftEquals(stretchBloc*,size_t); //shifts the value left by input
int stretchBlocRightShiftEquals(stretchBloc*,size_t); //shifts the value right logically by the input
int stretchBlocAndEquals(stretchBloc*,const stretchBloc*); //sets the value of the first to the bitwise and of the two
int stretchBlocOrEquals(stretchBloc*,const stretchBloc*); //sets the value of the first to the bitwise or of the two
int stretchBlocXorEquals(stretchBloc*,const stretchBloc*); //sets the value of thefirst to the bitwise xor of the two
int stretchBlocPowerEquals(stretchBloc*,size_t); //sets the value of the first to the first to the power of the second
int stretchBlocPOWEREquals(stretchBloc*,const stretchBloc*); //sets the value of the first to the first to the power of the second

#endif
