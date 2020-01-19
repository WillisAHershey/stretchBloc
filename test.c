#include "stretchBloc.h"

#define NUM 10

int main(){
  stretchBloc_t a,b,c;
  int i;
  for(i=0;i<100000;++i){
	randomStretchBloc(&a,NUM);
	stretchBlocLeftShiftOne(&b,&a);
	stretchBlocRightShiftOne(&c,&b);
	printlnDashedStretchBloc(&a);
	printlnDashedStretchBloc(&b);
	printlnDashedStretchBloc(&c);
	if(stretchBlocNotEqual(&a,&c))
		exit(EXIT_FAILURE);
	deleteStretchBloc(&a);
	deleteStretchBloc(&b);
	deleteStretchBloc(&c);
  }
  exit(EXIT_SUCCESS);
}

