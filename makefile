CLIENTDIR		=./client
CLIENTOLDAPP	=$(CLIENTDIR)/client
CLIENTNEWAPP	=./U1

SERVERDIR		=./server
SERVEROLDAPP	=$(SERVERDIR)/server
SERVERNEWAPP	=./Q1

makes:
	make -C $(CLIENTDIR)
	# make -C $(SERVERDIR)

all: makes
	cp $(CLIENTOLDAPP) $(CLIENTNEWAPP)
	# cp $(SERVEROLDAPP) $(SERVERNEWAPP)

clean:
	make -C $(CLIENTDIR) clean
	# make -C $(SERVERDIR) clean
	rm -f $(CLIENTNEWAPP)
	rm -f $(SERVERNEWAPP)

test:
	@echo Current directory: $(PWD)
	make -C $(CLIENTDIR) test
	# make -C $(SERVERDIR) test

testmem:
	make -C $(CLIENTDIR) testmem
	# make -C $(SERVERDIR) testmem

GROUP=T6G05

zip: clean
	rm -rf $(GROUP)
	rm -f $(GROUP).zip
	mkdir $(GROUP)
	cp -r client server acutest makefile README.md $(GROUP)
	zip --symlinks $(GROUP).zip -r $(GROUP)
	rm -rf $(GROUP)

DEST=~/Documents

testzip: zip
	rm -rf $(DEST)/$(GROUP)
	unzip $(GROUP).zip -d $(DEST)
	make -C $(DEST)/$(GROUP)/ test
