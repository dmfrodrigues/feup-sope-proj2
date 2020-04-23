COMMON	=../common/lib
APP		=client
SDIR	=./src
IDIR	=./include
ODIR   	=./obj
TDIR   	=./test
TFILE	=test
TAPP   	=$(ODIR)/test
ACUTEST =../acutest/include

CC     =gcc

IFLAGS =-I$(IDIR)
LFLAGS =-lpthread -lrt -L$(COMMON) -lcommon
CFLAGS_PARANOID =-Wall -Wextra -Wformat-nonliteral -Wcast-align -Wpointer-arith -Wbad-function-cast \
-Wstrict-prototypes -Winline -Wundef \
-Wnested-externs -Wcast-qual -Wshadow -Wwrite-strings -Wno-unused-parameter \
-Wfloat-equal -pedantic -Wno-long-long -Wno-endif-labels -g 
CFLAGS =-Wall -g #-O3
#CFLAGS=$(CFLAGS_PARANOID)

all: $(APP)

clean:
	rm -f  $(TAPP)
	rm -f  $(APP)
	rm -rf $(ODIR)

test: all $(TSRC)
	@echo Current directory: $(PWD)
	$(CC) $(CFLAGS) $(IFLAGS) -I$(ACUTEST) -c $(TDIR)/$(TFILE).c -o $(ODIR)/$(TFILE).o
	$(CC) $(ODIR)/$(TFILE).o $(ODIR)/client_args.o -o $(TAPP) $(LFLAGS)
	./$(TAPP)

testmem: all
	valgrind --leak-check=yes $(TAPP)

$(APP):   $(ODIR)/client.o $(ODIR)/client_args.o
	make -C ../common
	$(CC) $(ODIR)/client.o $(ODIR)/client_args.o -o $(APP) $(LFLAGS)

$(ODIR):
	mkdir -p $(ODIR)

$(ODIR)/%.o: $(SDIR)/%.c $(ODIR)
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@
