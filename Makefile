CC=	cc
CFLAGS=	-O
RM=	rm -rf

all: KusaReMKN data

KusaReMKN: kusaremkn.c
	$(CC) $(CFLAGS) -o KusaReMKN kusaremkn.c

data: mkdata.sh
	sh mkdata.sh

.PHONY: clean
clean:
	$(RM) KusaReMKN data temp
