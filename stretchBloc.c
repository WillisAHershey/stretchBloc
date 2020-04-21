#include "stretchBloc.h"

inline size_t blocLength(const stretchBloc *in){
  if(!in||!in->data)
	return 0;
  return malloc_usable_size(in->data)/longbytes;
}

inline size_t usedSpace(const stretchBloc *in){
  size_t len=blocLength(in);
  if(!len)
	return 0;
  for(--len;len;--len)
	if(in->data[len])
		return len+1;
  return len+1;
}

inline int mallocStretchBloc(stretchBloc *dest,size_t in){
  if(!in||!dest)
	return STRETCHBLOC_FAILURE;
  longtype *pt=(longtype*)malloc(in*longbytes);
  if(!pt)
	return STRETCHBLOC_FAILURE;
  size_t len=malloc_usable_size(pt)/longbytes;
  memset(&pt[in],0,(len-in)*longbytes);
  dest->data=pt;
  return STRETCHBLOC_SUCCESS;
}

inline int reallocStretchBloc(stretchBloc *in,size_t size){
  size_t oldLen=blocLength(in);
  if(!size||!oldLen)
	return STRETCHBLOC_FAILURE;
  if(size==oldLen)
	return STRETCHBLOC_SUCCESS;
  longtype *pt=(longtype*)realloc(in->data,size*longbytes);
  if(!pt)
	return STRETCHBLOC_FAILURE;
  size_t newLen=malloc_usable_size(pt)/longbytes;
  memset(&pt[size],0,(newLen-size)*longbytes);
  in->data=pt;
  return STRETCHBLOC_SUCCESS;
}

int stretchBlocInit(stretchBloc *dest){
  if(mallocStretchBloc(dest,1)==STRETCHBLOC_FAILURE)
	return STRETCHBLOC_FAILURE;
  dest->data[0]=0;
  return STRETCHBLOC_SUCCESS;  
}

int copyStretchBloc(stretchBloc *dest,const stretchBloc *in){
  size_t len=usedSpace(in);
  if(!len||!dest||mallocStretchBloc(dest,len)==STRETCHBLOC_FAILURE)
	return STRETCHBLOC_FAILURE;
  memcpy(dest->data,in->data,len*longbytes);
  return STRETCHBLOC_SUCCESS;
}

int newStretchBloc(stretchBloc *dest,longtype in){
  if(mallocStretchBloc(dest,1)==STRETCHBLOC_FAILURE)
	return STRETCHBLOC_FAILURE;
  dest->data[0]=in;
  return STRETCHBLOC_SUCCESS;
}

int stringToStretchBloc(stretchBloc *dest,char *string){
  return 0; //To be implemented later
}

int seed=0;

int randomStretchBloc(stretchBloc *dest,size_t num){
  if(!seed)
	srand((seed=time(NULL)));
  if(mallocStretchBloc(dest,num)==STRETCHBLOC_FAILURE)
	return STRETCHBLOC_FAILURE;
  size_t c;
  for(c=0;c<num;++c)
#ifdef slideInt
	dest->data[c]=((longtype)rand())<<(longbits/2)|(longtype)rand();
#else
	dest->data[c]=(longtype)rand();
#endif
  return STRETCHBLOC_SUCCESS;
}

int deleteStretchBloc(stretchBloc *in){
  if(!in||!in->data)
	return STRETCHBLOC_FAILURE;
  free(in->data);
  return STRETCHBLOC_SUCCESS;
}

int printStretchBloc(const stretchBloc *in){
  size_t len=usedSpace(in);
  if(!len)
	return STRETCHBLOC_FAILURE;
  printf("%"partPrint,in->data[--len]);
  for(--len;len!=size_t_max;--len)
	printf("%"wholePrint,in->data[len]);
  return STRETCHBLOC_SUCCESS;
}

int printlnStretchBloc(const stretchBloc *in){
  int c=printStretchBloc(in);
  if(c==STRETCHBLOC_SUCCESS)
	printf("\n");
  return c;
}

int printDashedStretchBloc(const stretchBloc *in){
  size_t len=usedSpace(in);
  if(!len)
	return STRETCHBLOC_FAILURE;
  printf("%"partPrint,in->data[--len]);
  for(--len;len!=size_t_max;--len)
	printf("-%"wholePrint,in->data[len]);
  return STRETCHBLOC_SUCCESS;
}

int printlnDashedStretchBloc(const stretchBloc *in){
  int c=printDashedStretchBloc(in);
  if(c==STRETCHBLOC_SUCCESS)
	printf("\n");
  return c;
}

char* sprintStretchBloc(const stretchBloc *in){
  size_t len=usedSpace(in);
  if(!len)
	return NULL;
  char *out=(char*)malloc(len*longbytes*2+1);
  if(!out)
	return NULL;
  char *pt=out;
  sprintf(pt,"%"partPrint,in->data[--len]);
  while(*pt)
	++pt;
  for(--len;len!=size_t_max;--len){
	sprintf(pt,"%"wholePrint,in->data[len]);
	pt+=(longbytes*2);
  }
  return out;
}

int stretchBlocTest(const stretchBloc *in){
  size_t len=blocLength(in);
  size_t c;
  for(c=0;c<len;++c)
	if(in->data[c])
		return 1;
  return 0;
}

int stretchBlocEquals(const stretchBloc *a,const stretchBloc *b){
  size_t alen=usedSpace(a);
  size_t c=usedSpace(b);
  if(!alen||alen!=c)
	return 0;
  for(c=0;c<alen;++c)
	if(a->data[c]!=b->data[c])
		return 0;
  return 1;
}

int stretchBlocNotEqual(const stretchBloc *a,const stretchBloc *b){
  return !stretchBlocEquals(a,b);
}

int stretchBlocGreaterThan(const stretchBloc *a,const stretchBloc *b){
  size_t alen=usedSpace(a);
  size_t c=usedSpace(b);
  if(!alen||alen!=c)
	return alen>c;
  for(c=alen-1;c!=size_t_max;--c)
	if(a->data[c]!=b->data[c])
		return a->data[c]>b->data[c];
  return 0;
}

int stretchBlocLessThan(const stretchBloc *a,const stretchBloc *b){
  size_t alen=usedSpace(a);
  size_t c=usedSpace(b);
  if(!alen||alen!=c)
	return alen<c;
  for(c=alen-1;c!=size_t_max;--c)
	if(a->data[c]!=b->data[c])
		return a->data[c]<b->data[c];
  return 0;
}

int stretchBlocGreaterThanOrEqual(const stretchBloc *a,const stretchBloc *b){
  return !stretchBlocLessThan(a,b);
}

int stretchBlocLessThanOrEqual(const stretchBloc *a,const stretchBloc *b){
  return !stretchBlocGreaterThan(a,b);
}

inline int inlineStretchBlocLessThanOrEqual(const stretchBloc *a,const stretchBloc *b,size_t len){
  for(--len;len!=size_t_max;--len)
	if(a->data[len]!=b->data[len])
		return a->data[len]<b->data[len];
  return 1;
}

int stretchBlocPlus(stretchBloc *dest,const stretchBloc *a,const stretchBloc *b){
  size_t alen=usedSpace(a);
  size_t blen=usedSpace(b);
  if(!dest||!alen||!blen)
	return STRETCHBLOC_FAILURE;
  size_t olen=alen>=blen?alen:blen;
  size_t slen=alen>=blen?blen:alen;
  if(mallocStretchBloc(dest,olen)==STRETCHBLOC_FAILURE)
	return STRETCHBLOC_FAILURE;
  size_t c;
  longtype carry=0;
  for(c=0;c<slen;++c){
	dest->data[c]=a->data[c]+b->data[c]+carry;
	carry=dest->data[c]<a->data[c]||(carry&&dest->data[c]==a->data[c]);
  }
  if(c==olen)
	;
  else if(c==blen){
	for(;carry&&c<alen;++c){
		dest->data[c]=a->data[c]+carry;
		carry=dest->data[c]==0;
	}
	for(;c<alen;++c)
		dest->data[c]=a->data[c];
  }
  else{
	for(;carry&&c<blen;++c){
		dest->data[c]=b->data[c]+1;
		carry=dest->data[c]==0;
	}
	for(;c<blen;++c)
		dest->data[c]=b->data[c];
  }
  if(carry){
	if(blocLength(dest)==olen)
		if(reallocStretchBloc(dest,olen+1)==STRETCHBLOC_FAILURE)
			return STRETCHBLOC_FAILURE;
	dest->data[olen]=(longtype)1;
  }
  return STRETCHBLOC_SUCCESS;
}

int stretchBlocMinus(stretchBloc *dest,const stretchBloc *a,const stretchBloc *b){
  size_t alen=usedSpace(a);
  size_t blen=usedSpace(b);
  if(!alen||!blen)
	return STRETCHBLOC_FAILURE;
  if(alen<blen||(alen==blen&&inlineStretchBlocLessThanOrEqual(a,b,alen)))
	return stretchBlocInit(dest);
  if(mallocStretchBloc(dest,alen)==STRETCHBLOC_FAILURE)
	return STRETCHBLOC_FAILURE;
  longtype carry=0;
  size_t c;
  for(c=0;c<blen;++c){
	dest->data[c]=a->data[c]-b->data[c]-carry;
	carry=dest->data[c]>a->data[c]||(carry&&dest->data[c]==a->data[c]);
  }
  if(c==alen)
	;
  else{
	for(;carry&&c<alen;++c){
		dest->data[c]=a->data[c]-1;
		carry=dest->data[c]==~(longtype)0;
	}
	for(;c<alen;++c)
		dest->data[c]=a->data[c];
  }
  if(!dest->data[alen-1])
	return reallocStretchBloc(dest,usedSpace(dest));
  return STRETCHBLOC_SUCCESS;
}

int stretchBlocTimes(stretchBloc *dest,const stretchBloc *a,const stretchBloc *b){
  size_t alen=usedSpace(a);
  size_t blen=usedSpace(b);
  if(!alen||!blen)
	return STRETCHBLOC_FAILURE;
  //...
  return STRETCHBLOC_FAILURE;
}

int stretchBlocDividedBy(stretchBloc *dest,const stretchBloc *a,const stretchBloc *b){
  return 0;
}

int stretchBlocRemainder(stretchBloc *dest,const stretchBloc *a,const stretchBloc *b){
  return 0;
}

int stretchBlocDivideRemainder(stretchBloc *quo,stretchBloc *rem,const stretchBloc *a,const stretchBloc *b){
  return 0;	
}

int stretchBlocLeftShiftOne(stretchBloc *dest,const stretchBloc *in){
  size_t len=usedSpace(in);
  if(!len)
	return STRETCHBLOC_FAILURE;
  longtype mask=((longtype)1)<<(longbits-1);
  if(in->data[len-1]&mask){
	if(mallocStretchBloc(dest,len+1)==STRETCHBLOC_FAILURE)
		return STRETCHBLOC_FAILURE;
	dest->data[len]=(longtype)1;
  }
  else
  	if(mallocStretchBloc(dest,len)==STRETCHBLOC_FAILURE)
		return STRETCHBLOC_FAILURE;
  size_t c;
  longtype hold=0;
  for(c=0;c<len;++c){
	dest->data[c]=(in->data[c]<<1)+hold;
	hold=(longtype)((in->data[c]&mask)!=0);
  }
  return STRETCHBLOC_SUCCESS;
}

int stretchBlocRightShiftOne(stretchBloc *dest,const stretchBloc *in){
  size_t len=usedSpace(in);
  if(!len)
	return STRETCHBLOC_FAILURE;
  longtype mask=((longtype)1)<<(longbits-1);
  longtype hold=0;
  if(in->data[len-1]==(longtype)1){
	--len;
	hold=mask;
  }
  if(mallocStretchBloc(dest,len)==STRETCHBLOC_FAILURE)
	return STRETCHBLOC_FAILURE;
  for(--len;len!=size_t_max;--len){
	dest->data[len]=(in->data[len]>>1)|hold;
	hold=in->data[len]&(longtype)1?mask:0;
  }
  return STRETCHBLOC_SUCCESS;
}

int stretchBlocLeftShift(stretchBloc *dest,const stretchBloc *in,size_t shamt){
  return 0;
}

int stretchBlocRightShift(stretchBloc *dest,const stretchBloc *in,size_t shamt){
  return 0;
}

int stretchBlocAnd(stretchBloc *dest,const stretchBloc *a,const stretchBloc *b){
  size_t alen=usedSpace(a);
  size_t blen=usedSpace(b);
  if(!alen||!blen)
	return STRETCHBLOC_FAILURE;
  size_t olen=alen<=blen?alen:blen;
  if(mallocStretchBloc(dest,olen)==STRETCHBLOC_FAILURE)
	return STRETCHBLOC_FAILURE;
  size_t c;
  for(c=0;c<olen;++c)
	dest->data[c]=a->data[c]&b->data[c];
  return STRETCHBLOC_SUCCESS;
}

int stretchBlocOr(stretchBloc *dest,const stretchBloc *a,const stretchBloc *b){ 
  size_t alen=usedSpace(a);
  size_t blen=usedSpace(b);
  if(!alen||!blen)
	return STRETCHBLOC_FAILURE;
  size_t olen=alen>=blen?alen:blen;
  size_t slen=alen>=blen?blen:alen;
  if(mallocStretchBloc(dest,olen)==STRETCHBLOC_FAILURE)
	return STRETCHBLOC_FAILURE;
  size_t c;
  for(c=0;c<slen;++c)
	dest->data[c]=a->data[c]|b->data[c];
  if(c==olen)
	;
  else if(c==alen)
	for(;c<blen;++c)
		dest->data[c]=b->data[c];
  else
	for(;c<alen;++c)
		dest->data[c]=a->data[c];
  return STRETCHBLOC_SUCCESS;
}

int stretchBlocXor(stretchBloc *dest,const stretchBloc *a,const stretchBloc *b){
  size_t alen=usedSpace(a);
  size_t blen=usedSpace(b);
  if(!alen||!blen)
	return STRETCHBLOC_FAILURE;
  size_t olen=alen>=blen?alen:blen;
  size_t slen=alen>=blen?blen:alen;
  if(mallocStretchBloc(dest,olen)==STRETCHBLOC_FAILURE)
	return STRETCHBLOC_FAILURE;
  size_t c;
  for(c=0;c<slen;++c)
	dest->data[c]=a->data[c]^b->data[c];
  if(c==olen)
	;
  else if(c==alen)
	for(;c<blen;++c)
		dest->data[c]=b->data[c];
  else
	for(;c<alen;++c)
		dest->data[c]=a->data[c];
  return STRETCHBLOC_SUCCESS;
}

int stretchBlocPower(stretchBloc *dest,const stretchBloc *in,size_t pow){
  return 0;
}

int stretchBlocPOWER(stretchBloc *dest,const stretchBloc *in,const stretchBloc *pow){
  return 0;
}

int stretchBlocPlusPlus(stretchBloc *in){
  if(!in||!in->data)
	return STRETCHBLOC_FAILURE;
  if(++in->data[0])
	return STRETCHBLOC_SUCCESS;
  size_t len=blocLength(in);
  size_t c;
  for(c=1;c<len;++c)
	if(++in->data[c])
		return STRETCHBLOC_SUCCESS;
  reallocStretchBloc(in,len+1);
  in->data[len]=(longtype)1;
  return STRETCHBLOC_SUCCESS;
}

int stretchBlocMinusMinus(stretchBloc *in){
  if(!stretchBlocTest(in))
	return STRETCHBLOC_FAILURE;
  if(in->data[0]--)
	return STRETCHBLOC_SUCCESS;
  size_t len=usedSpace(in);
  size_t c;
  for(c=1;c<len;++c)
	if(in->data[c]--)
		break;
  if(!in->data[len-1])
	return reallocStretchBloc(in,len-1);
  return STRETCHBLOC_SUCCESS;
}

int stretchBlocPlusEquals(stretchBloc *a,const stretchBloc *b){
  size_t alen=blocLength(a); //We're concerned with the whole size of a
  size_t blen=usedSpace(b); //But only the utilized portion of b
  if(!alen||!blen)
	return STRETCHBLOC_FAILURE;
  size_t slen;
  if(blen>alen){ //No matter what, make sure a has the capacity to hold the same amount of data as b, but slen will tell us if a was resized
	if(reallocStretchBloc(a,blen)==STRETCHBLOC_FAILURE)
		return STRETCHBLOC_FAILURE;
	slen=alen;
	alen=blocLength(a);
  }
  else
	slen=blen;
  longtype carry=0;
  size_t c;
  for(c=0;c<slen;++c){ //Do the heavywork of addition until the smaller of the two values
	a->data[c]=a->data[c]+b->data[c]+carry;
	carry=(longtype)(a->data[c]<b->data[c]||(carry&&a->data[c]==b->data[c]));
  }
  if(c==blen) //If a was not resized, and there's potentially plenty of data up there
	for(;carry&&c<alen;++c) //While carry
		carry=(longtype)(++a->data[c]==0); //Plusplus every longtype
  else{ //Otherwise if a was resized, and we've reached the part where a is topped with zeroes
	for(;carry&&c<blen;++c){
		a->data[c]=b->data[c]+1;
		carry=(longtype)!a->data[c];
	}
	for(;c<blen;++c)
		a->data[c]=b->data[c];
  }
  if(carry){
	if(c==alen)
		if(reallocStretchBloc(a,alen+1)==STRETCHBLOC_FAILURE)
			return STRETCHBLOC_FAILURE;
	a->data[c]=(longtype)1;
  }
  return STRETCHBLOC_SUCCESS;
}

int stretchBlocMinusEquals(stretchBloc *a,const stretchBloc *b){
  return 0;
}

int stretchBlocTimesEquals(stretchBloc *a,const stretchBloc *b){
  return 0;
}

int stretchBlocDividedByEquals(stretchBloc *a,const stretchBloc *b,stretchBloc *rem){
  return 0;
}

int stretchBlocRemainderEquals(stretchBloc *a,const stretchBloc *b,stretchBloc *quo){
  return 0;
}

int stretchBlocLeftShiftEqualsOne(stretchBloc *in){
  return 0;
}

int stretchBlocRightShiftEqualsOne(stretchBloc *in){
  return 0;
}

int stretchBlocLeftShiftEquals(stretchBloc *in,size_t shamt){
  return 0;
}

int stretchBlocRightShiftEquals(stretchBloc *in,size_t shamt){
  return 0;
}

int stretchBlocAndEquals(stretchBloc *a,const stretchBloc *b){
  return 0;
}

int stretchBlocOrEquals(stretchBloc *a,const stretchBloc *b){
  return 0;
}

int stretchBlocXorEquals(stretchBloc *a,const stretchBloc *b){
 return 0;
}

int stretchBlocPowerEquals(stretchBloc *in,size_t pow){
  return 0;
}

int stretchBlocPOWEREquals(stretchBloc *in,const stretchBloc *pow){
  return 0;
}

/*
char* stretchBlocToString(stretchBloc *in){
  size_t len=usedSpace(in);
  if(!len)
	return NULL;
  char *out=(char*)malloc(len*longbytes*2);
  if(!out)
	return NULL;
  sprintf(out,"%"partPrint,in->data[--len]);
  char *pt=out+strlen(out);
  for(here_to_bottom(len)){
	sprintf(pt,"%"wholePrint,in->data[len]);
	pt+=longbytes*2;
  }
  return out;
}

int printStretchBloc(stretchBloc *in){
  size_t len=usedSpace(in);
  if(!len)
	return STRETCHBLOC_FAILURE;
  printf("%"partPrint,in->data[--len]);
  for(here_to_bottom(len))
	printf("%"wholePrint,in->data[len]);
  return STRETCHBLOC_SUCCESS;
}

int printlnStretchBloc(stretchBloc *in){
  int c=printStretchBloc(in);
  if(c==STRETCHBLOC_SUCCESS)
	printf("\n");
  return c;
}

int printDashedStretchBloc(stretchBloc *in){
  size_t len=usedSpace(in);
  if(!len)
	return STRETCHBLOC_FAILURE;
  printf("%"partPrint,in->data[--len]);
  for(here_to_bottom(len))
	printf("-%"wholePrint,in->data[len]);
  return STRETCHBLOC_SUCCESS;
}

int printlnDashedStretchBloc(stretchBloc *in){
  int c=printDashedStretchBloc(in);
  if(c==STRETCHBLOC_SUCCESS)
	printf("\n");
  return c;
}

int deleteStretchBloc(stretchBloc *in){
  if(!in||!in->data)
	return STRETCHBLOC_FAILURE;
  free(in->data);
  return STRETCHBLOC_SUCCESS;
}

inline int mallocStretchBloc(stretchBloc *dest,size_t in){
  if(!in||dest)
	return STRETCHBLOC_FAILURE;
  longtype *pt=(longtype*)malloc(in*longbytes);
  if(!pt)
	return STRETCHBLOC_FAILURE;
  dest->data=pt;
  size_t len=malloc_usable_size(pt)/longbytes;
  for(;in<len;++in)
	pt[in]=0;
  return STRETCHBLOC_SUCCESS;
}

inline void reallocStretchBloc(stretchBloc *in,size_t requestedLen){
  size_t oldLen=blocLength(in);
  if(!oldLen||requestedLen==oldLen)
	return;
  in->data=realloc(in->data,requestedLen*longbytes);
  size_t newLen=blocLength(in);
  if(requestedLen>oldLen)
	for(requestedLen=oldLen;requestedLen<newLen;++requestedLen)
		in->data[requestedLen]=0;
  else
	for(oldLen=newLen-1;oldLen>=requestedLen;--oldLen)
		in->data[oldLen]=0;
}

int stretchBlocInit(stretchBloc *dest){
  if(mallocStretchBloc(dest,1)!=STRETCHBLOC_SUCCESS)
	return STRETCHBLOC_FAILURE;
  out->data[0]=0;
  return STRETCHBLOC_SUCCESS;
}

int copyStretchBloc(stretchBloc *dest,stretchBloc *in){
  size_t len=usedSpace(in);
  if(!len)
	return STRETCHBLOC_FAILURE;
  if(mallocStretchBloc(dest,len)!=STRETCHBLOC_SUCCESS)
	return STRETCHBLOC_FAILURE;
  for(here_to_bottom(len))
	dest->data[len]=in->data[len];
  return STRETCHBLOC_SUCCESS;
}

stretchBloc* newStretchBloc(longtype in){
  stretchBloc *out=mallocStretchBloc(1);
  if(!out)
	return NULL;
  out->data[0]=in;
  return out;
}

stretchBloc* stringToStretchBloc(char *in){
  return NULL;
}

int seed=0;

stretchBloc* randomStretchBloc(size_t numlongs){
  if(!seed)
	srand((seed=time(NULL)));
  stretchBloc *out=mallocStretchBloc(numlongs);
  if(!out)
	return NULL;
  size_t c;
  for(bottom_to_top(c,numlongs))
#ifdef slideInt
	out->data[c]=((longtype)rand())<<(longbits/2)|(longtype)rand();
#else
	out->data[c]=(longtype)rand();
#endif
  return out;
}

int stretchBlocTest(stretchBloc *in){
  size_t len=blocLength(in);
  size_t c;
  for(c=0;c<len;++c)
	if(in->data[c])
		return 1;
  return 0; //If len is 0 it will fall through to here anyway :)
}

int stretchBlocEquals(stretchBloc *a,stretchBloc *b){
  size_t alen=usedSpace(a);
  size_t c=usedSpace(b);
  if(!alen||alen!=c)
	return 0;
  for(c=0;c<alen;++c)
	if(a->data[c]!=b->data[c])
		return 0;
  return 1;
}

int stretchBlocNotEqual(stretchBloc *a,stretchBloc *b){
  return !stretchBlocEquals(a,b);
}

int stretchBlocGreaterThan(stretchBloc *a,stretchBloc *b){
  size_t alen=usedSpace(a);
  size_t c=usedSpace(b);
  if(!alen||alen!=c)
	return alen>c;
  for(c=alen-1;c!=size_t_max;--c)
	if(a->data[c]!=b->data[c])
		return a->data[c]>b->data[c];
  return 0;
}

int stretchBlocLessThan(stretchBloc *a,stretchBloc *b){
  size_t alen=usedSpace(a);
  size_t c=usedSpace(b);
  if(!alen||alen!=c)
	return alen<c;
  for(c=alen-1;c!=size_t_max;--c)
	if(a->data[c]!=b->data[c])
		return a->data[c]<b->data[c];
  return 0;
}

int stretchBlocGreaterThanOrEqual(stretchBloc *a,stretchBloc *b){
  return !stretchBlocLessThan(a,b);
}

int stretchBlocLessThanOrEqual(stretchBloc *a,stretchBloc *b){
  return !stretchBlocGreaterThan(a,b);
}

inline int inlineStretchBlocLessThanOrEqual(stretchBloc *a,stretchBloc *b,size_t len){
  size_t c;
  for(c=len-1;c!=size_t_max;--c)
	if(a->data[c]!=b->data[c])
		return a->data[c]<b->data[c];
  return 1;
}

stretchBloc* stretchBlocPlus(stretchBloc *a,stretchBloc *b){
  size_t alen=usedSpace(a);
  size_t blen=usedSpace(b);
  if(!alen||!blen)
	return NULL;
  size_t olen=alen>=blen?alen:blen;
  size_t slen=alen>=blen?blen:alen;
  stretchBloc *out=mallocStretchBloc(olen);
  if(!out)
	return NULL;
  size_t c;
  longtype carry=0;
  for(c=0;c<slen;++c){
	out->data[c]=a->data[c]+b->data[c]+carry;
	carry=out->data[c]<a->data[c]||(carry&&out->data[c]==a->data[c]);
  }
  if(c==olen)
	;
  else if(c==blen){
	for(;carry&&c<alen;++c){
		out->data[c]=a->data[c]+carry;
		carry=out->data[c]==0;
	}
	for(;c<alen;++c)
		out->data[c]=a->data[c];
  }
  else{
	for(;carry&&c<blen;++c){
		out->data[c]=b->data[c]+carry;
		carry=out->data[c]==0;
	}
	for(;c<blen;++c)
		out->data[c]=b->data[c];
  }
  if(carry){
	out->data=realloc(out->data,olen+1);
	if(!out->data){
		free(out);
		return NULL;
	}
	out->data[olen]=(longtype)1;
  }
  return out;
}

stretchBloc* stretchBlocMinus(stretchBloc *a,stretchBloc *b){
  size_t alen=usedSpace(a);
  size_t blen=usedSpace(b);
  if(!alen||!blen)
	return NULL;
  size_t c;
  if(alen<blen||(alen==blen&&inlineStretchBlocLessThanOrEqual(a,b,alen)))
	return stretchBlocInit();
  stretchBloc *out=mallocStretchBloc(alen);
  if(!out)
	return NULL;
  longtype carry=0;
  for(c=0;c<blen;++c){
	out->data[c]=a->data[c]-b->data[c]-carry;
	carry=out->data[c]>a->data[c]||(carry&&out->data[c]==a->data[c]);
  }
  if(c==alen)
	;
  else{
	for(;carry&&c<alen;++c){
		out->data[c]=a->data[c]-carry;
		carry=out->data[c]==~(longtype)0;
	}
	for(;c<alen;++c)
		out->data[c]=a->data[c];
  }
  if(!out->data[alen-1])
	reallocStretchBloc(out,usedSpace(out));
  return out;
}

stretchBloc* stretchBlocTimes(stretchBloc *oa,stretchBloc *ob){
  return NULL;
}

stretchBloc* stretchBlocDividedBy(stretchBloc *a,stretchBloc *b,stretchBloc **remainder){
  return NULL;
}

stretchBloc* stretchBlocRemainder(stretchBloc *a,stretchBloc *b,stretchBloc **quotient){
  return NULL;
}

stretchBloc* stretchBlocLeftShiftOne(stretchBloc* in){
  size_t len=usedSpace(in);
  if(!len)
	return NULL;
  stretchBloc *out=mallocStretchBloc(len);
  size_t c;
  longtype lastCarry,carry=0;
  longtype mask=(longtype)1<<(longbits-1);
  for(c=0;c<len;++c){
	lastCarry=carry;
	if(in->data[c]&mask)
		carry=(longtype)1;
  	else
		carry=0;
	out->data[c]=(in->data[c]<<1)|lastCarry;
  }
  if(carry){
	len=malloc_usable_size(in->data)/longbytes;
	if(c<len)
		out->data[c]=(longtype)1;
	else{
		out->data=realloc(out->data,len+1);
		len=blocLength(out);
		out->data[c]=(longtype)1;
		for(++c;c<len;++c)
			out->data[c]=0;
	}
  }
  return out;
}

stretchBloc* stretchBlocRightShiftOne(stretchBloc *in){
  size_t len=usedSpace(in);
  if(!len)
	return NULL;
  stretchBloc *out=mallocStretchBloc(len);
  size_t c;
  longtype lastCarry,carry=0;
  longtype mask=(longtype)1<<(longbits-1);
  for(c=len-1;c!=size_t_max;--c){
	lastCarry=carry;
	if(in->data[c]&(longtype)1)
		carry=mask;
  	else
		carry=0;
	out->data[c]=(in->data[c]>>1)|lastCarry;
  }
  return out;
}

stretchBloc* stretchBlocLeftShift(stretchBloc *in,size_t shamt){
  return NULL;
}

stretchBloc* stretchBlocRightShift(stretchBloc *in,size_t shamt){
  return NULL;
}

stretchBloc* stretchBlocAnd(stretchBloc *a,stretchBloc *b){
  size_t alen=usedSpace(a);
  size_t blen=usedSpace(b);
  if(!alen||!blen)
	return NULL;
  size_t olen=alen>blen?blen:alen;
  stretchBloc *out=mallocStretchBloc(olen);
  if(!out)
	return NULL;
  size_t c;
  for(c=0;c<olen;++c)
	out->data[c]=a->data[c]&b->data[c];
  return out;
}

stretchBloc* stretchBlocOr(stretchBloc *a,stretchBloc *b){
  size_t alen=usedSpace(a);
  size_t blen=usedSpace(b);
  if(!alen||!blen)
	return NULL;
  size_t olen=alen>=blen?alen:blen;
  size_t slen=alen>=blen?blen:alen;
  stretchBloc *out=mallocStretchBloc(olen);
  size_t c;
  for(c=0;c<slen;++c)
	out->data[c]=a->data[c]|b->data[c];
  if(c==olen)
	;
  else if(c==blen)
	for(;c<alen;++c)
		out->data[c]=a->data[c];
  else
	for(;c<blen;++c)
		out->data[c]=b->data[c];
  return out;
}

stretchBloc* stretchBlocXor(stretchBloc *a,stretchBloc *b){
  size_t alen=usedSpace(a);
  size_t blen=usedSpace(b);
  if(!alen||!blen)
	return NULL;
  size_t olen=alen>=blen?alen:blen;
  size_t slen=alen>=blen?blen:alen;
  stretchBloc *out=mallocStretchBloc(olen);
  size_t c;
  for(c=0;c<slen;++c)
	out->data[c]=a->data[c]^b->data[c];
  if(c==olen)
	;
  else if(c==blen)
	for(;c<alen;++c)
		out->data[c]=a->data[c];
  else
	for(;c<blen;++c)
		out->data[c]=b->data[c];
  return out;
}

stretchBloc* stretchBlocPower(stretchBloc *in,size_t pow){
  return NULL;
}

stretchBloc* stretchBlocPOWER(stretchBloc *in,stretchBloc *pow){
  return NULL;
}

int stretchBlocPlusPlus(stretchBloc *in){
  if(!in||!in->data)
	return STRETCHBLOC_FAILURE;
  if(++in->data[0])
	return STRETCHBLOC_SUCCESS;
  size_t len=blocLength(in);
  size_t c;
  for(c=1;c<len;++c)
	if(++in->data[c])
		return STRETCHBLOC_SUCCESS;
  reallocStretchBloc(in,len+1);
  in->data[len]=(longtype)1;
  return STRETCHBLOC_SUCCESS;
}

int stretchBlocMinusMinus(stretchBloc *in){
  if(!stretchBlocTest(in))
	return STRETCHBLOC_FAILURE;
  if(in->data[0]--)
	return STRETCHBLOC_SUCCESS;
  size_t len=blocLength(in);
  size_t c;
  for(c=1;c<len;++c)
	if(in->data[c]--)
		return STRETCHBLOC_SUCCESS;
  if(!in->data[len-1])
	reallocStretchBloc(in,usedSpace(in));
  return STRETCHBLOC_SUCCESS;
}

int stretchBlocPlusEquals(stretchBloc *a,stretchBloc *b){
  size_t alen=blocLength(a);
  size_t blen=usedSpace(b);
  size_t olen=alen>=blen?alen:blen;
  size_t slen=alen>=blen?blen:alen;
  longtype carry=0;
  size_t c;
  for(c=0;c<slen;++c){
	a->data[c]=a->data[c]+b->data[c]+carry;
	carry=a->data[c]<b->data[c]||(carry&&a->data[c]==b->data[c]);
  }
  if(c==olen)
	;
  else if(c==blen)
	for(;c<alen&&carry;++c)
		carry=(++a->data[c]==0);
  else{
	reallocStretchBloc(a,blen);
	alen=olen=blocLength(a);
	for(;carry&&c<blen;++c){
		a->data[c]=b->data[c]+carry;
		carry=!a->data[c];
	}
	for(;c<blen;++c)
		a->data[c]=b->data[c];
  }
  if(carry){
	reallocStretchBloc(a,alen+1);
	a->data[alen]=(longtype)1;
  }
  return STRETCHBLOC_SUCCESS;
}

int stretchBlocMinusEquals(stretchBloc *a,stretchBloc *b){
  size_t alen=usedSpace(a);
  size_t blen=usedSpace(b);
  size_t c;
  if(alen<blen||(alen==blen&&inlineStretchBlocLessThanOrEqual(a,b,alen))){
	a->data=realloc(a->data,longbytes);
	alen=blocLength(a);
	for(c=0;c<alen;++c)
		a->data[c]=0;
	return STRETCHBLOC_SUCCESS;
  }
  longtype carry=0,lastCarry;
  for(c=0;c<blen;++c){
	lastCarry=a->data[c]<b->data[c]||(carry&&a->data[c]==b->data[c]);
	a->data[c]-=(b->data[c]+carry);
	carry=lastCarry;
  }
  for(;carry&&c<alen;++c)
	carry=a->data[c]--;
  if(!a->data[alen-1])
	reallocStretchBloc(a,usedSpace(a));
  return STRETCHBLOC_SUCCESS;
}

int stretchBlocTimesEquals(stretchBloc *a,stretchBloc *b){
  return STRETCHBLOC_FAILURE;
}

int stretchBlocDividedByEquals(stretchBloc *a,stretchBloc *b){
  return STRETCHBLOC_FAILURE;
}

int stretchBlocRemainderEquals(stretchBloc *a,stretchBloc *b){
  return STRETCHBLOC_FAILURE;
}

int stretchBlocLeftShiftEqualsOne(stretchBloc *in){
  size_t len=usedSpace(in);
  if(!len)
	return STRETCHBLOC_FAILURE;
  size_t c;
  longtype mask=(longtype)1<<(longbits-1);
  longtype carry=0,lastCarry;
  for(c=0;c<len;++c){
	lastCarry=carry;
	carry=(in->data[c]&mask)!=0;
	in->data[c]=in->data[c]<<1|lastCarry;
  }
  if(carry){
	len=malloc_usable_size(in->data)/longbytes;
	if(c<len)
		in->data[c]=(longtype)1;
	else{
		in->data=realloc(in->data,len+1);
		len=blocLength(in);
		in->data[c]=(longtype)1;
		for(++c;c<len;++c)
			in->data[c]=0;
	}
  }
  return STRETCHBLOC_SUCCESS;
}

int stretchBlocRightShiftEqualsOne(stretchBloc *in){
  return STRETCHBLOC_FAILURE;
}

int stretchBlocLeftShiftEquals(stretchBloc *in,size_t shamt){
  return STRETCHBLOC_FAILURE;
}

int stretchBlocRightShiftEquals(stretchBloc *in,size_t shamt){
  return STRETCHBLOC_FAILURE;
}

int stretchBlocAndEquals(stretchBloc *a,stretchBloc *b){
  size_t alen=blocLength(a);
  size_t blen=usedSpace(b);
  if(!alen||!blen)
	return STRETCHBLOC_FAILURE;
  size_t olen=alen<=blen?alen:blen;
  if(alen!=blen)
	reallocStretchBloc(
  if(alen>blen)
	reallocStretchBloc(a,blen);
  size_t c;
  for(c=0;c<olen;++c)
	a->data[c]=a->data[c]&b->data[c];
  return STRETCHBLOC_SUCCESS;
}

int stretchBlocOrEquals(stretchBloc *a,stretchBloc *b){
  size_t alen=usedSpace(a);
  size_t blen=usedSpace(b);
  if(!alen||!blen)
	return STRETCHBLOC_FAILURE;
  if(alen
  size_t olen=alen>=blen?alen:blen;
  size_t slen=alen>=blen?blen:alen;
  if(olen>alen)
	reallocStretchBloc(a,olen);
  size_t c;
  for(c=0;c<slen;++c)
	a->data[c]=a->data[c]|b->data[c];
  if(c==olen)
	;
  else if(c==alen)
	for(;c<blen;++c)
		a->data[c]=b->data[c];
  return STRETCHBLOC_SUCCESS;
}

int stretchBlocXorEquals(stretchBloc *a,stretchBloc *b){
  return STRETCHBLOC_FAILURE;
}

int stretchBlocPowerEquals(stretchBloc *in,size_t shamt){
  return STRETCHBLOC_FAILURE;
}

int stretchBlocPOWEREquals(stretchBloc *in,stretchBloc *shamt){
  return STRETCHBLOC_FAILURE;
}

int main(){
  stretchBloc *a=randomStretchBloc(70);
  int c;
  for(c=0;c<70000;++c)
	stretchBlocPlusPlus(a);
  printlnDashedStretchBloc(a);
  for(c=0;c<70001;++c)
	if(stretchBlocMinusMinus(a)==STRETCHBLOC_FAILURE)
		printf("Failure\n");
  printlnDashedStretchBloc(a);
  deleteStretchBloc(a);
}
*/
