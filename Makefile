HEADERS=net.h logging.h handle.h config.h

all: ircat

ircat: main.o net.o logging.o handle.o
	gcc -g main.o net.o logging.o handle.o -o ircat

main.o: main.c ${HEADERS}
	gcc -g -c main.c

net.o: net.c ${HEADERS}
	gcc -g -c net.c

logging.o: logging.c ${HEADERS}
	gcc -g -c logging.c

handle.o: handle.c ${HEADERS}
	gcc -g -c handle.c

clean:
	rm -f *o ircat
