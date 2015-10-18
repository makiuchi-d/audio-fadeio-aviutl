#  MAKEFILE for Borland C/C++ compiler
#

CC = BCC32
COPT = -tWD -O1 -O2

OBJ = fadeio.obj
AUF = audio_fadeio.auf

$(AUF): $(OBJ)
	$(CC) $(COPT) -e$(AUF) $(OBJ)

fadeio.obj: fadeio.c filter.h
	$(CC) $(COPT) -c fadeio.c


