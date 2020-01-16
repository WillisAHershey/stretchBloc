#include "stretchBloc.h"

#define NUM 12

int main(){
  stretchBloc_t a,b;
  newStretchBloc(&a,1);
  printlnDashedStretchBloc(&a);
  int c;
  for(c=0;c<1000;++c){
	stretchBlocLeftShiftOne(&b,&a);
	printlnDashedStretchBloc(&b);
	deleteStretchBloc(&a);
	a=b;
  }
  deleteStretchBloc(&a);
}

