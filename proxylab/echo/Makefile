all: echoclient echoserver

echoclient: echoclient.o csapp.o
	gcc -O2 -o echoclient echoclient.o csapp.o

echoserver: echoserver.o echo.o csapp.o
	gcc -O2 -o echoserver echoserver.o echo.o csapp.o

echoclient.o: echoclient.c
	gcc -O2 -c echoclient.c

echoserver.o: echoserver.c
	gcc -O2 -c echoserver.c

echo.o: echo.c
	gcc -O2 -c echo.c

csapp.o: csapp.c
	gcc -O2 -c csapp.c

clean:
	rm -f *.o echoclient echoserver *~