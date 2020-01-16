
stretchBloc.o: stretchBloc.h stretchBloc.c
	gcc -o stretchBloc.o -Wall -O3 -c stretchBloc.c

.PHONY: clean

clean:
	rm stretchBloc.o
