APP		=client
SDIR	=./src
IDIR	=./include
ODIR   	=./obj
TDIR   	=./test
TFILE	=test
TAPP   	=$(ODIR)/test
ACUTEST =../acutest/include

IFLAGS =-I$(IDIR)

CC     =gcc

CFLAGS_PARANOID =-Wall -Wextra -Wformat-nonliteral -Wcast-align -Wpointer-arith -Wbad-function-cast \
-Wstrict-prototypes -Winline -Wundef \
-Wnested-externs -Wcast-qual -Wshadow -Wwrite-strings -Wno-unused-parameter \
-Wfloat-equal -pedantic -Wno-long-long -Wno-endif-labels -g 

# CFLAGS =-Wall -g #-O3
CFLAGS=$(CFLAGS_PARANOID)

all: makefolders $(APP)

clean:
	rm -f  $(TAPP)
	rm -f  $(APP)
	rm -rf $(ODIR)

test: all $(TSRC)
	@echo Current directory: $(PWD)
	$(CC) $(CFLAGS) $(IFLAGS) -I$(ACUTEST) -c $(TDIR)/$(TFILE).c -o $(ODIR)/$(TFILE).o
	$(CC) $(ODIR)/$(TFILE).o $(ODIR)/client_args.o -o $(TAPP)
	./$(TAPP)

testmem: all
	valgrind --leak-check=yes $(TAPP)

makefolders:
	mkdir -p $(ODIR)

$(APP):   $(ODIR)/client.o $(ODIR)/client_args.o
	$(CC) $(ODIR)/client.o $(ODIR)/client_args.o -o $(APP)

$(ODIR)/%.o:          $(SDIR)/%.c
	$(CC) $(CFLAGS) $(IFLAGS) -c $^ -o $@
