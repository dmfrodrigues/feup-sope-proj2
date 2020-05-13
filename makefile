COMMONDIR		=./common

CLIENTDIR		=./client
CLIENTOLDAPP	=$(CLIENTDIR)/client
CLIENTNEWAPP	=./U2

SERVERDIR		=./server
SERVEROLDAPP	=$(SERVERDIR)/server
SERVERNEWAPP	=./Q2

# Create client and server binaries
all: $(CLIENTNEWAPP) $(SERVERNEWAPP)

$(CLIENTNEWAPP): FORCE
	make -C $(CLIENTDIR)
	cp $(CLIENTOLDAPP) $(CLIENTNEWAPP)

$(SERVERNEWAPP): FORCE
	make -C $(SERVERDIR)
	cp $(SERVEROLDAPP) $(SERVERNEWAPP)

FORCE:

# Clean binary files
clean: 
	make -C $(COMMONDIR) clean
	make -C $(CLIENTDIR) clean
	make -C $(SERVERDIR) clean
	rm -f $(CLIENTNEWAPP)
	rm -f $(SERVERNEWAPP)

# Test code
test: all
	make -C $(COMMONDIR) test
	make -C $(CLIENTDIR) test
	make -C $(SERVERDIR) test
	chmod u+x **/*.sh
	test/test.sh

# Test code and find memory leaks
testmem:
	make -C $(CLIENTDIR) testmem
	make -C $(SERVERDIR) testmem

GROUP=T6G05

# Create zip file to submit
zip: clean
	rm -rf $(GROUP)
	rm -f $(GROUP).zip
	mkdir $(GROUP)
	cp -r common client server test acutest makefile README.md $(GROUP)
	zip --symlinks $(GROUP).zip -r $(GROUP)
	rm -rf $(GROUP)

DEST=~/Documents

# Test zip file to submit
testzip: zip
	rm -rf $(DEST)/$(GROUP)
	unzip $(GROUP).zip -d $(DEST)
	make -C $(DEST)/$(GROUP)/ test
