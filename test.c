#include <stdio.h>
#include <stdlib.h>
#include "stretchBloc.h"
#include <pthread.h>
#include <semaphore.h>

#define NUM 12

int main(){
  stretchBloc_t a,b,c,d,e;
  int i;
  for(i=0;i<10000000;++i){
  	randomStretchBloc(&a,NUM);
  	randomStretchBloc(&b,NUM);
  	stretchBlocPlus(&c,&a,&b);
  	stretchBlocMinus(&d,&c,&a);
  	stretchBlocMinus(&e,&c,&b);
  	if(stretchBlocNotEqual(&d,&b)||stretchBlocNotEqual(&e,&a)){
		printlnDashedStretchBloc(&a);
		printlnDashedStretchBloc(&b);
		printlnDashedStretchBloc(&c);
		printlnDashedStretchBloc(&d);
		printlnDashedStretchBloc(&e);
		deleteStretchBloc(&a);
		deleteStretchBloc(&b);
		deleteStretchBloc(&c);
		deleteStretchBloc(&d);
		deleteStretchBloc(&e);
		break;
	}
	deleteStretchBloc(&a);
	deleteStretchBloc(&b);
	deleteStretchBloc(&c);
	deleteStretchBloc(&d);
	deleteStretchBloc(&e);
  }
  return 0;
}

