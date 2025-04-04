#CC = gcc
#CFLAGS = -O2 -Wall -I .
#
## This flag includes the Pthreads library on a Linux box.
## Others systems will probably require something different.
#LIB = -lpthread
#
#all: tiny cgi
#
#tiny: tiny.c csapp.o
#	$(CC) $(CFLAGS) -o tiny tiny.c csapp.o $(LIB)
#
#csapp.o: csapp.c
#	$(CC) $(CFLAGS) -c csapp.c
#
#cgi:
#	(cd cgi-bin; make)
#
#clean:
#	rm -f *.o tiny *~
#	(cd cgi-bin; make clean)
#

echoclient: echoclient.o csapp.o
	gcc -02 -o echoclient echoclient.o csapp.o

echoserver: echoserver.o echo.o csapp.o
	gcc -02 -o echoserver echoserver.o echo.o csapp.o

echoclient.o: echoclient.c
	gcc -02 -c echoclient.o

echoserver.o: echoserver.c
	gcc -02 -c echoserver.o

echo.o: echo.c
	gcc -02 -c echo.o

csapp.o: csapp.c
	gcc -02 -c csapp.c