all: sin srs cin crs
.PHONY:all

sin: serverInput.c myhead.h
	gcc serverInput.c -o sin
srs: serverRS.c myhead.h
	gcc serverRS.c -o srs
cin: clientInput.c myhead.h
	gcc clientInput.c -o cin
crs: clientRS.c myhead.h
	gcc clientRS.c -o crs

.PHONY:clean
clean:
	rm -f *.o *.~ sin cin srs crs
