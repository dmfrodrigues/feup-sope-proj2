COMMON	=./lib
SDIR	=./src
IDIR	=./include
ODIR   	=./obj
LDIR    =./lib
LIB		=$(LDIR)/libcommon.a
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

# CFLAGS =-Wall -g #-O3
CFLAGS=$(CFLAGS_PARANOID)

O_FILES=$(ODIR)/output.o $(ODIR)/common_time.o $(ODIR)/utils.o $(ODIR)/common_atomic.o

all: makefolders $(LIB)

clean:
	rm -f  $(TAPP)
	rm -f  $(APP)
	rm -rf $(ODIR)
	rm -rf $(LDIR)

test: all $(TSRC) $(LIB)
	@echo Current directory: $(PWD)
	$(CC) $(CFLAGS) $(IFLAGS) -I$(ACUTEST) -c $(TDIR)/$(TFILE).c -o $(ODIR)/$(TFILE).o
	$(CC) $(ODIR)/$(TFILE).o -o $(TAPP) $(LFLAGS)
	./$(TAPP)

testmem: all
	valgrind --leak-check=yes $(TAPP)

makefolders:
	mkdir -p $(ODIR)
	mkdir -p $(LDIR)

$(LIB):   $(O_FILES)
	rm -f $(LIB)
	ar rvs $(LIB) $(O_FILES)

$(ODIR)/%.o:          $(SDIR)/%.c
	$(CC) $(CFLAGS) $(IFLAGS) -c $^ -o $@
