TARGET = idist2 

PARAMS = curve2.data dist2.data   

SRC =  Main.C \
       ICurveDist.C \
       BrownICurveDist.C \
       global.C 
		
OBJ =  Main.o \
	ICurveDist.o \
	BrownICurveDist.o \
	global.o 

CC  = g++   

IFLAGS= -I.

				
CFLAGS = -DGLUE_REAL=double \
	-O3 \
	-Wall \
	-DLINUX \
	-ansi \
	-fPIC \
	-static \
	-c $(IFLAGS)

LD = g++  

LPATHS = -L.\
	-L/usr/X11R6/lib


LLIBS	= 
	
genexe:	genobj  
	$(LD)$(LFLAGS) $(LPATHS) $(OBJ) -o $(TARGET) $(LLIBS)
	echo "#!/bin/bash" > ./run 
	echo "./$(TARGET) $(PARAMS)" >> ./run
	chmod u+x ./run
	-@rm -rf $(OBJ)

genobj: $(SRC)
	$(CC) $(CFLAGS) $(SRC)


all: genexe 

clean:
	rm -f *.o *~ $(TARGET) ./run   

