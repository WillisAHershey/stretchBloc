stretchBloc.o: stretchBloc.h stretchBloc.c
	gcc -o stretchBloc.o -Wall -O3 -c stretchBloc.c

.PHONY: clean

clean:
	rm stretchBloc.o

.PHONY: debug

debug: stretchBloc.h stretchBloc.c
	gcc -o stretchBloc.o -Wall -O0 -g -c stretchBloc.c


