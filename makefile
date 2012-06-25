#Var
VERSION = libircclient-1.6

all: kill build

#delete

kill:
	rm $(VERSION).tar.gz
	rm -r $(VERSION)

#install + download

build:
	wget http://sourceforge.net/projects/libircclient/files/libircclient/1.6/libircclient-1.6.tar.gz
	tar -zxf $(VERSION).tar.gz
	cd $(VERSION); ./configure; make; cd - 
	cd Bobot; make; cd-
