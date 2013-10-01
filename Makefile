CC=gcc
CFLAGS=­DLINUX ­ansi ­Wall ­D_GNU_SOURCE # g
CPPFLAGS=
BIN=shell
ODIR=build
all:	$(ODIR)/$(BIN)
$(ODIR):
	mkdir $(ODIR)
$(ODIR)/$(BIN):	$(ODIR)/$(BIN).o
	$(CC) -o $(ODIR)/$(BIN) $(ODIR)/$(BIN).o
$(ODIR)/$(BIN).o:	$(BIN).c $(ODIR)
	$(CC) -std=c99 -c -o $(ODIR)/$(BIN).o $(BIN).c
clean:	
	rm -fr $(ODIR)/
