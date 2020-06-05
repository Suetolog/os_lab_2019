CC=gcc
CFLAGS=-I -g -std=c99 
tcln : tcpclient.c
	$(CC) tcpclient.c -o tc $(CFLAGS)

tsrv : tcpserver.c
	$(CC) tcpserver.c -o ts $(CFLAGS)

ucln : udpclient.c
	$(CC) udpclient.c -o uc $(CFLAGS)

udsrv : udpserver.c
	$(CC) udpserver.c -o us $(CFLAGS)

all : tcln tsrv ucln udsrv
    
clean :
	rm tc ts uc us