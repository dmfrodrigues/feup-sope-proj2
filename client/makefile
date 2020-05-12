COMMON	=../common/lib
COMMON_IDIR=../common/include
APP		=client
SDIR	=./src
IDIR	=./include
ODIR   	=./obj
TDIR   	=./test
TFILE	=test
TAPP   	=$(ODIR)/test
ACUTEST =../acutest/include

CC     =gcc

IFLAGS =-I$(IDIR) -I$(COMMON_IDIR)
LFLAGS =-lpthread -lrt -L$(COMMON) -lcommon
CFLAGS_PARANOID =-Wall -Wextra -Wformat-nonliteral -Wcast-align -Wpointer-arith -Wbad-function-cast \
-Wstrict-prototypes -Winline -Wundef \
-Wnested-externs -Wcast-qual -Wshadow -Wwrite-strings -Wunused-parameter \
-Wfloat-equal -pedantic -Wno-long-long -Wno-endif-labels -Wunused-result -g 
#CFLAGS =-Wall -g #-O3
CFLAGS=$(CFLAGS_PARANOID)

O_FILES =$(ODIR)/client_args.o $(ODIR)/client_thread_args.o $(ODIR)/client_threads.o
O_MAIN_FILE=$(ODIR)/client.o

all: $(APP)

clean:
	rm -f  $(TAPP)
	rm -f  $(APP)
	rm -rf $(ODIR)

$(TAPP): all $(TSRC)
	$(CC) $(CFLAGS) $(IFLAGS) -I$(ACUTEST) -c $(TDIR)/$(TFILE).c -o $(ODIR)/$(TFILE).o
	$(CC) $(ODIR)/$(TFILE).o $(O_FILES) -o $(TAPP) $(LFLAGS)

test: $(TAPP)
	./$(TAPP)

testmem: $(TAPP)
	valgrind --leak-check=yes $(TAPP)

$(APP):   $(O_MAIN_FILE) $(O_FILES)
	make -C ../common
	$(CC) $(O_MAIN_FILE) $(O_FILES) -o $(APP) $(LFLAGS)

$(ODIR):
	mkdir -p $(ODIR)

$(ODIR)/%.o: $(SDIR)/%.c | $(ODIR)
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@
