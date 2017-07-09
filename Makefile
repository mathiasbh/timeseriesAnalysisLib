CFLAGS = -std=c99 -Wall -O3
LDLIBS = -lm
OUTPUT = output.txt
PROGM  = main
HEADER = header.h
OBJECT = structures.o readdata.o main.o powerspectrum.o addfunc.o clean.o filter.o xcorr.o autocorr.o

all: main $(OUTPUT) #$(PLOT) 

$(OUTPUT) : $(PROGM) Makefile
	./$< 1> $@

main : $(OBJECT) $(HEADER)


clean :
	rm -rf $(PROGM) $(OUTPUT) $(OBJECT) *~ 
