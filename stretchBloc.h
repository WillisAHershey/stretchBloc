//Willis Hershey wrote this whole damned thing and it is broken and beautiful. Someone please give him a job.

//This file defines a structure stretchBloc_t, which represents an unsigned integer of effectively no maximum size, as well as a library of functions to 
//perform mathematical operations on and between them

#ifndef __STRETCHBLOC_ //pragma once
#define __STRETCHBLOC_

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <malloc.h>
#include <string.h>
#include <time.h>

//longtype can be redefined as any unsigned integer type without side-effect as long as longtype_max is also properly set
#define longtype unsigned long
#define longtype_max ULONG_MAX

//STRETCHBLOC_SUCCESS and STRETCHBLOC_FAILURE are the standard return for all of the int type functions
#define STRETCHBLOC_SUCCESS 1
#define STRETCHBLOC_FAILURE 0

#define size_t_max ~((size_t)0) //size_ts are tricky in for loops when you count down (beacuse of their unsignedness), so I saved myself some trouble here
#define longbytes sizeof(longtype)
#define longbits (longbytes*8)

#if longtype_max==0xffffffffffffffff //The following macros allow the stretchBlocs to be written into strings correctly
	#define wholePrint "016lX"
	#define partPrint "lX"
#elif longtype_max==0xffffffff
	#if longtype_max==ULONG_MAX
		#define wholePrint "08lX"
		#define partPrint "lX"
	#else
		#define wholePrint "08X" //Case longs are larger than ints, but you want to use unsigned int as longtype anyway
		#define partPrint "X"
	#endif
#elif longtype_max==0xffff //longtype can be ushort or uint_8 if you want, but this code will likely lose efficiency
	#define wholePrint "04X"
	#define partPrint "X"
#elif longtype_max==0xff
	#define wholePrint "02X"
	#define partPrint "X"
#endif

#if longtype_max>UINT_MAX //This keeps randomStretchBloc working in all architectures
	#define slideInt
#endif

typedef struct stretchBloc{ //It's essentially a double pointer to a block of unsigned longs, so the block of longs can be moved, and all references to the stretchBloc
  longtype *data;           //will remain valid as long as the pointer is updated, which is done automatically in the functions where that is necessary.
}stretchBloc_t;       //This code will not malloc(sizeof(stretchBloc_t)) for you, so if you do not put one on the stack, it is your job to free it.

char* stretchBlocToString(stretchBloc_t*,int); //Returns string representing value in given base. Memory for string is malloced and must be freed.

//These functions all return STRETCHBLOC_SUCCESS or STRETCHBLOC_FAILURE except for the boolean functions, which return 1 or 0 as described

//PURE INITIALIZATION FUNCTIONS
int stretchBlocInit(stretchBloc_t*); //Initializes new stretchBloc to zero in the given location
int copyStretchBloc(stretchBloc_t*,const stretchBloc_t*); //Initializes new stretchBloc in given location to value of given stretchBloc
int newStretchBloc(stretchBloc_t*,longtype); //Initializes new stretchBloc in given location with value of given longtype
int stringToStretchBloc(stretchBloc_t*,char*); //Initializes new stretchBloc in given locations with value from null terminated (numerical c-standard) string
int randomStretchBloc(stretchBloc_t*,size_t); //Initializes new stretchBloc in given location with input number of randomly generated longtypes
//DESTRUCTOR FUNCTIONS
int deleteStretchBloc(stretchBloc_t*); //Frees the memory allocated by this library for any valid stretchBloc
//HEXADECIMAL PRINTING FUNCTIONS
int printStretchBloc(const stretchBloc_t*); //prints value of stretchBloc to stdout in hexadecimal
int printlnStretchBloc(const stretchBloc_t*); //prints value of stretchBloc to stdout with newline
int printDashedStretchBloc(const stretchBloc_t*); //prints value of stretchBloc to stdout with dashes between longs
int printlnDashedStretchBloc(const stretchBloc_t*); //prints value of stretchBloc to stdout with dashes between longs and a newline
char* sprintStretchBloc(const stretchBloc_t*);
//BOOLEAN FUNCTIONS
int stretchBlocTest(const stretchBloc_t*); //returns 1 if nonzero value and 0 otherwise (Functions as boolean test for value)
int stretchBlocEquals(const stretchBloc_t*,const stretchBloc_t*); //returns 1 if the two stretchBlocs have equivalent values
int stretchBlocNotEqual(const stretchBloc_t*,const stretchBloc_t*); //returns 1 if the two stretchBlocs have different values
int stretchBlocGreaterThan(const stretchBloc_t*,const stretchBloc_t*); //returns 1 if the first stretchBloc has a greater value than the second
int stretchBlocLessThan(const stretchBloc_t*,const stretchBloc_t*); //returns 1 if the first stretchBloc has a lesser value than the second
int stretchBlocGreaterThanOrEqual(const stretchBloc_t*,const stretchBloc_t*); //returns 1 if the first stretchBloc has a greater or equal value than the second
int stretchBlocLessThanOrEqual(const stretchBloc_t*,const stretchBloc_t*); //returns 1 if the first stretchBloc has a lesser or equal value than the second
//MATHEMATICAL INITIALIZATION FUNCTIONS
int stretchBlocPlus(stretchBloc_t*,const stretchBloc_t*,const stretchBloc_t*); //Initializes new stretchBloc with value of sum of input values
int stretchBlocMinus(stretchBloc_t*,const stretchBloc_t*,const stretchBloc_t*); //Initializes new stretchBloc with value of difference of input values, or zero if negative
int stretchBlocTimes(stretchBloc_t*,const stretchBloc_t*,const stretchBloc_t*); //Initializes new stretchBloc with value of product of input values
int stretchBlocDividedBy(stretchBloc_t*,const stretchBloc_t*,const stretchBloc_t*); //Initializes new stretchBloc with value of quotient of input values*
int stretchBlocRemainder(stretchBloc_t*,const stretchBloc_t*,const stretchBloc_t*); //Initializes new stretchBloc with value of remainder of division*
int stretchBlocDivideRemainder(stretchBloc_t*,stretchBloc_t*,const stretchBloc_t*,const stretchBloc_t*);
int stretchBlocLeftShiftOne(stretchBloc_t*,const stretchBloc_t*); //Initializes new stretchBloc with value of input left shifted one place
int stretchBlocRightShiftOne(stretchBloc_t*,const stretchBloc_t*); //Initializes new stretchBloc with value of input logically right shifted one place
int stretchBlocLeftShift(stretchBloc_t*,const stretchBloc_t*,size_t); //Initializes new stretchBloc with value of input left shifted input places
int stretchBlocRightShift(stretchBloc_t*,const stretchBloc_t*,size_t); //Initializes new stretchBloc with value of input right shifted logically input places
int stretchBlocAnd(stretchBloc_t*,const stretchBloc_t*,const stretchBloc_t*); //Initializes new stretchBloc with value of bitwise and of inputs
int stretchBlocOr(stretchBloc_t*,const stretchBloc_t*,const stretchBloc_t*); //Initializes new stretchBloc with value of bitwise or of inputs
int stretchBlocXor(stretchBloc_t*,const stretchBloc_t*,const stretchBloc_t*); //Initializes new stretchBloc with value of bitwise xor of inputs
int stretchBlocPower(stretchBloc_t*,const stretchBloc_t*,size_t); //Initializes new stretchBloc with value of input to the power of longtype input
int stretchBlocPOWER(stretchBloc_t*,const stretchBloc_t*,const stretchBloc_t*); //Initializes new stretchBloc with value of input to the power of stretchBloc input
//* dividedBy and remainder also accept an extra pointer where, if not null, the remainder or divisor (respectively) will be stored, as it is computed as a byproduct
//MUTATOR FUNCTIONS
int stretchBlocPlusPlus(stretchBloc_t*); //increments value of input by one
int stretchBlocMinusMinus(stretchBloc_t*); //decrements value of input by one
int stretchBlocPlusEquals(stretchBloc_t*,const stretchBloc_t*); //increments value of first by value of second
int stretchBlocMinusEquals(stretchBloc_t*,const stretchBloc_t*); //decrements value of first by value of second, will set to zero if difference is negative
int stretchBlocTimesEquals(stretchBloc_t*,const stretchBloc_t*); //sets value of first to the product of the two values
int stretchBlocDividedByEquals(stretchBloc_t*,const stretchBloc_t*,stretchBloc_t*); //sets value of first to the quotient of the first and second
int stretchBlocRemainderEquals(stretchBloc_t*,const stretchBloc_t*,stretchBloc_t*); //sets value of first to the remainder of division of the fisrt and second
int stretchBlocLeftShiftEqualsOne(stretchBloc_t*); //shifts the value left by one
int stretchBlocRightShiftEqualsOne(stretchBloc_t*); //shifts the value right logically by one
int stretchBlocLeftShiftEquals(stretchBloc_t*,size_t); //shifts the value left by input
int stretchBlocRightShiftEquals(stretchBloc_t*,size_t); //shifts the value right logically by the input
int stretchBlocAndEquals(stretchBloc_t*,const stretchBloc_t*); //sets the value of the first to the bitwise and of the two
int stretchBlocOrEquals(stretchBloc_t*,const stretchBloc_t*); //sets the value of the first to the bitwise or of the two
int stretchBlocXorEquals(stretchBloc_t*,const stretchBloc_t*); //sets the value of thefirst to the bitwise xor of the two
int stretchBlocPowerEquals(stretchBloc_t*,size_t); //sets the value of the first to the first to the power of the second
int stretchBlocPOWEREquals(stretchBloc_t*,const stretchBloc_t*); //sets the value of the first to the first to the power of the second

#endif
