COMMONDIR		=./common

CLIENTDIR		=./client
CLIENTOLDAPP	=$(CLIENTDIR)/client
CLIENTNEWAPP	=./U1

SERVERDIR		=./server
SERVEROLDAPP	=$(SERVERDIR)/server
SERVERNEWAPP	=./Q1

all: $(CLIENTNEWAPP) $(SERVERNEWAPP)

$(CLIENTOLDAPP):
	make -C $(CLIENTDIR)

$(SERVEROLDAPP):
	make -C $(SERVERDIR)

$(CLIENTNEWAPP): $(CLIENTOLDAPP)
	cp $(CLIENTOLDAPP) $(CLIENTNEWAPP)

$(SERVERNEWAPP): $(SERVEROLDAPP)
	cp $(SERVEROLDAPP) $(SERVERNEWAPP)

clean:
	make -C $(COMMONDIR) clean
	make -C $(CLIENTDIR) clean
	make -C $(SERVERDIR) clean
	rm -f $(CLIENTNEWAPP)
	rm -f $(SERVERNEWAPP)

test: all
	make -C $(COMMONDIR) test
	make -C $(CLIENTDIR) test
	make -C $(SERVERDIR) test
	chmod u+x test/*.sh
	test/test.sh

testmem:
	make -C $(CLIENTDIR) testmem
	make -C $(SERVERDIR) testmem

GROUP=T6G05

zip: clean
	rm -rf $(GROUP)
	rm -f $(GROUP).zip
	mkdir $(GROUP)
	cp -r common client server acutest makefile README.md $(GROUP)
	zip --symlinks $(GROUP).zip -r $(GROUP)
	rm -rf $(GROUP)

DEST=~/Documents

testzip: zip
	rm -rf $(DEST)/$(GROUP)
	unzip $(GROUP).zip -d $(DEST)
	make -C $(DEST)/$(GROUP)/ test
