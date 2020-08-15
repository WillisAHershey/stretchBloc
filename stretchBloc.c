#include "stretchBloc.h"

static inline size_t blocLength(const stretchBloc *in){
  if(!in)
	return 0;
  return in->numLongs;
}

/*static inline size_t usedSpace(const stretchBloc *in){
  size_t len=blocLength(in);
  if(!len)
	return 0;
  for(--len;len;--len)
	if(in->data[len])
		return len+1;
  return len+1;
}*/

#define usedSpace(c) blocLength(c)

static inline int mallocStretchBloc(stretchBloc *dest,size_t in){
  if(!in||!dest)
	return STRETCHBLOC_FAILURE;
  LONGTYPE *pt=malloc(in*LONGBYTES);
  if(!pt)
	return STRETCHBLOC_FAILURE;
  *dest=(stretchBloc){.data=pt,.numLongs=in};
  return STRETCHBLOC_SUCCESS;
}

static inline int reallocStretchBloc(stretchBloc *in,size_t size){
  if(!in)
	return STRETCHBLOC_FAILURE;
  LONGTYPE *pt=realloc(in->data,size*LONGBYTES);
  if(!pt)
	return STRETCHBLOC_FAILURE;
  *in=(stretchBloc){.data=pt,.numLongs=size};
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
  if(mallocStretchBloc(dest,len)==STRETCHBLOC_FAILURE)
	return STRETCHBLOC_FAILURE;
  memcpy(dest->data,in->data,len*LONGBYTES);
  return STRETCHBLOC_SUCCESS;
}

int newStretchBloc(stretchBloc *dest,LONGTYPE in){
  if(mallocStretchBloc(dest,1)==STRETCHBLOC_FAILURE)
	return STRETCHBLOC_FAILURE;
  dest->data[0]=in;
  return STRETCHBLOC_SUCCESS;
}

int stringToStretchBloc(stretchBloc *dest,char *string){
  return 0; //To be implemented later
}

static inline LONGTYPE randomLongtype(){
//This function assumes RAND_MAX is some power of 2 minus 1 (0xFFF... 0x7FFF... 0x3FFF... etc.)
//If this is not the case, the results will not be very random.
  static int seed=0;
  static LONGTYPE leftovers=0;
  static int num=0;
  static int entropy=0;
  if(!seed){
	srand((seed=time(NULL)));
  	int mask=RAND_MAX;
	while(mask){
		++entropy;
		mask>>=1;
	}
  }
  int need=LONGBITS-num;
  LONGTYPE out=leftovers;
  LONGTYPE hold;
  while(need>entropy){
	hold=(LONGTYPE)rand();
	out^=hold<<(need-entropy);
	need-=entropy;
  }
  hold=(LONGTYPE)rand();
  out^=hold>>(entropy-need);
  num=entropy-need;
  leftovers=hold<<(LONGBITS-num);
  return out;
}

int randomStretchBloc(stretchBloc *dest,size_t num){
   if(mallocStretchBloc(dest,num)==STRETCHBLOC_FAILURE)
	return STRETCHBLOC_FAILURE;
  for(size_t c=0;c<num;++c)
	dest->data[c]=randomLongtype();
  return STRETCHBLOC_SUCCESS;
}

int deleteStretchBloc(stretchBloc *in){
  if(!in||!in->data)
	return STRETCHBLOC_FAILURE;
  free(in->data);
  *in=(stretchBloc){.data=NULL,.numLongs=0};
  return STRETCHBLOC_SUCCESS;
}

int printStretchBloc(const stretchBloc *in){
  size_t len=usedSpace(in);
  if(!len)
	return STRETCHBLOC_FAILURE;
  printf("%"PARTPRINT,in->data[--len]);
  for(--len;len!=SIZE_T_MAX;--len)
	printf("%"WHOLEPRINT,(int)sizeof(LONGTYPE)*2,in->data[len]);
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
  printf("%"PARTPRINT,in->data[--len]);
  for(--len;len!=SIZE_T_MAX;--len)
	printf("-%"WHOLEPRINT,(int)sizeof(LONGTYPE)*2,in->data[len]);
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
  char *out=(char*)malloc(len*LONGBYTES*2+1);
  if(!out)
	return NULL;
  char *pt=out;
  sprintf(pt,"%"PARTPRINT,in->data[--len]);
  while(*pt)
	++pt;
  for(--len;len!=SIZE_T_MAX;--len){
	sprintf(pt,"%"WHOLEPRINT,(int)sizeof(LONGTYPE)*2,in->data[len]);
	pt+=(LONGBYTES*2);
  }
  return out;
}

_Bool stretchBlocTest(const stretchBloc *in){
  size_t len=blocLength(in);
  size_t c;
  for(c=0;c<len;++c)
	if(in->data[c])
		return 1;
  return 0;
}

_Bool stretchBlocEquals(const stretchBloc *a,const stretchBloc *b){
  size_t alen=usedSpace(a);
  if(!alen||alen!=usedSpace(b))
	return 0;
  for(size_t c=0;c<alen;++c)
	if(a->data[c]!=b->data[c])
		return 0;
  return 1;
}

_Bool stretchBlocNotEqual(const stretchBloc *a,const stretchBloc *b){
  return !stretchBlocEquals(a,b);
}

_Bool stretchBlocGreaterThan(const stretchBloc *a,const stretchBloc *b){
  size_t alen=usedSpace(a);
  size_t c=usedSpace(b);
  if(!alen||alen!=c)
	return alen>c;
  for(c=alen-1;c!=SIZE_T_MAX;--c)
	if(a->data[c]!=b->data[c])
		return a->data[c]>b->data[c];
  return 0;
}

_Bool stretchBlocLessThan(const stretchBloc *a,const stretchBloc *b){
  size_t alen=usedSpace(a);
  size_t c=usedSpace(b);
  if(!alen||alen!=c)
	return alen<c;
  for(c=alen-1;c!=SIZE_T_MAX;--c)
	if(a->data[c]!=b->data[c])
		return a->data[c]<b->data[c];
  return 0;
}

_Bool stretchBlocGreaterThanOrEqual(const stretchBloc *a,const stretchBloc *b){
  return !stretchBlocLessThan(a,b);
}

_Bool stretchBlocLessThanOrEqual(const stretchBloc *a,const stretchBloc *b){
  return !stretchBlocGreaterThan(a,b);
}

static inline int inlineStretchBlocLessThanOrEqual(const stretchBloc *a,const stretchBloc *b,size_t len){
  for(--len;len!=SIZE_T_MAX;--len)
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
  _Bool carry=0;
  for(c=0;c<slen;++c){
	dest->data[c]=a->data[c]+b->data[c]+carry;
	carry=dest->data[c]<a->data[c]||(carry&&dest->data[c]==a->data[c]);
  }
  if(c==olen)
	;
  else if(c==blen){
	for(;carry&&c<alen;++c){
		dest->data[c]=a->data[c]+1;
		carry=dest->data[c]==0;
	}
	if(c<alen)
		memcpy(&dest->data[c],&a->data[c],LONGBYTES*(alen-c));
  }
  else{
	for(;carry&&c<blen;++c){
		dest->data[c]=b->data[c]+1;
		carry=dest->data[c]==0;
	}
	if(c<blen)
		memcpy(&dest->data[c],&b->data[c],LONGBYTES*(blen-c));
  }
  if(carry){
	if(reallocStretchBloc(dest,olen+1)==STRETCHBLOC_FAILURE)
		return STRETCHBLOC_FAILURE;
	dest->data[olen]=(LONGTYPE)1;
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
  _Bool carry=0;
  size_t c;
  for(c=0;c<blen;++c){
	dest->data[c]=a->data[c]-b->data[c]+carry;
	carry=dest->data[c]>a->data[c]||(carry&&dest->data[c]==a->data[c]);
  }
  if(c==alen)
	;
  else{
	for(;carry&&c<alen;++c){
		dest->data[c]=a->data[c]-1;
		carry=dest->data[c]==((LONGTYPE)0)-1;
	}
	if(c<alen)
		memcpy(&dest->data[c],&a->data[c],LONGBYTES*(alen-c));
  }
  if(!dest->data[alen-1])
	return reallocStretchBloc(dest,usedSpace(dest));
  return STRETCHBLOC_SUCCESS;
}

int stretchBlocTimes(stretchBloc *dest,const stretchBloc *a,const stretchBloc *b){
  size_t alen=usedSpace(a);
  size_t blen=usedSpace(b);
  if(!alen||!blen||!dest)
	return STRETCHBLOC_FAILURE;
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
  if(!len||!dest)
	return STRETCHBLOC_FAILURE;
  LONGTYPE mask=((LONGTYPE)1)<<(LONGBITS-1);
  if(in->data[len-1]&mask){
	if(mallocStretchBloc(dest,len+1)==STRETCHBLOC_FAILURE)
		return STRETCHBLOC_FAILURE;
	dest->data[len]=(LONGTYPE)1;
  }
  else
  	if(mallocStretchBloc(dest,len)==STRETCHBLOC_FAILURE)
		return STRETCHBLOC_FAILURE;
  _Bool hold=0;
  for(size_t c=0;c<len;++c){
	dest->data[c]=(in->data[c]<<1)|hold;
	hold=in->data[c]&mask;
  }
  return STRETCHBLOC_SUCCESS;
}

int stretchBlocRightShiftOne(stretchBloc *dest,const stretchBloc *in){
  size_t len=usedSpace(in);
  if(!len)
	return STRETCHBLOC_FAILURE;
  LONGTYPE mask=((LONGTYPE)1)<<(LONGBITS-1);
  LONGTYPE hold=0;
  if(in->data[len-1]==(LONGTYPE)1){
	--len;
	hold=mask;
  }
  if(mallocStretchBloc(dest,len)==STRETCHBLOC_FAILURE)
	return STRETCHBLOC_FAILURE;
  for(--len;len!=SIZE_T_MAX;--len){
	dest->data[len]=(in->data[len]>>1)|hold;
	hold=in->data[len]&(LONGTYPE)1?mask:0;
  }
  return STRETCHBLOC_SUCCESS;
}

int stretchBlocLeftShift(stretchBloc *dest,const stretchBloc *in,size_t shamt){
  if(!dest||!in)
	return STRETCHBLOC_FAILURE;
  if(!shamt)
	return copyStretchBloc(dest,in);
  size_t longs=shamt/LONGBITS;
  size_t shifts=shamt%LONGBITS;
  size_t len=usedSpace(in);
  if(shifts&&in->data[len-1]&(~((((LONGTYPE)1)<<(LONGBITS-shifts))-1))){
	if(mallocStretchBloc(dest,len+longs+1)==STRETCHBLOC_FAILURE)
		return STRETCHBLOC_FAILURE;
	dest->data[len+longs]=in->data[len-1]>>(LONGBITS-shifts);
  }
  else
	if(mallocStretchBloc(dest,len+longs)==STRETCHBLOC_FAILURE)
		return STRETCHBLOC_FAILURE;
  memset(&dest->data[0],0,LONGBYTES*longs);
  if(shifts){
	LONGTYPE hold=0;
	for(size_t c=0;c<len;++c){
		dest->data[c+longs]=(in->data[c]<<shifts)|hold;
		hold=in->data[c]>>(LONGBITS-shifts);
	}
  }
  else
	memcpy(&dest->data[longs],&in->data[0],LONGBYTES*len);
  return STRETCHBLOC_SUCCESS;
}

int stretchBlocRightShift(stretchBloc *dest,const stretchBloc *in,size_t shamt){
  if(!dest||!in)
	return STRETCHBLOC_FAILURE;
  if(!shamt)
	return copyStretchBloc(dest,in);
  size_t longs=shamt/LONGBITS;
  size_t shifts=shamt%LONGBITS;
  size_t len=usedSpace(in);
  if(longs>=len||(longs-len==1&&!(in->data[len-1]>>shifts)))
	return stretchBlocInit(dest);
  return STRETCHBLOC_FAILURE;
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
  in->data[len]=(LONGTYPE)1;
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
  LONGTYPE carry=0;
  size_t c;
  for(c=0;c<slen;++c){ //Do the heavywork of addition until the smaller of the two values
	a->data[c]=a->data[c]+b->data[c]+carry;
	carry=(LONGTYPE)(a->data[c]<b->data[c]||(carry&&a->data[c]==b->data[c]));
  }
  if(c==blen) //If a was not resized, and there's potentially plenty of data up there
	for(;carry&&c<alen;++c) //While carry
		carry=(LONGTYPE)(++a->data[c]==0); //Plusplus every LONGTYPE
  else{ //Otherwise if a was resized, and we've reached the part where a is topped with zeroes
	for(;carry&&c<blen;++c){
		a->data[c]=b->data[c]+1;
		carry=(LONGTYPE)!a->data[c];
	}
	for(;c<blen;++c)
		a->data[c]=b->data[c];
  }
  if(carry){
	if(c==alen)
		if(reallocStretchBloc(a,alen+1)==STRETCHBLOC_FAILURE)
			return STRETCHBLOC_FAILURE;
	a->data[c]=(LONGTYPE)1;
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
  size_t len=usedSpace(in);
  if(!len)
	return STRETCHBLOC_FAILURE;
  LONGTYPE mask=((LONGTYPE)1)<<(LONGBITS-1);
  if(in->data[len-1]&mask){
	if(reallocStretchBloc(in,len+1)==STRETCHBLOC_FAILURE)
		return STRETCHBLOC_FAILURE;
	in->data[len]=(LONGTYPE)0;
  }
  for(--len;len!=SIZE_T_MAX;--len){
	if(in->data[len]&mask)
		in->data[len+1]|=1;
	in->data[len]<<=1;
  }
  return STRETCHBLOC_SUCCESS;
}

int stretchBlocRightShiftEqualsOne(stretchBloc *in){
  size_t len=usedSpace(in);
  if(!len)
	return STRETCHBLOC_FAILURE;
  LONGTYPE mask=((LONGTYPE)1)<<(LONGBITS-1);
  for(size_t c=0;c<len-1;++c){
	in->data[c]>>=1;
	if(in->data[c+1]&(LONGTYPE)1)
		in->data[c]|=mask;
  }
  if(in->data[len-1]==(LONGTYPE)1){
	if(reallocStretchBloc(in,len-1)==STRETCHBLOC_FAILURE)
		return STRETCHBLOC_FAILURE;
  }
  else
  	in->data[len-1]>>=1;
  return STRETCHBLOC_SUCCESS;
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

