export CC = gcc
export CFLAGS = -Wall
export LDFLAGS =
export PREFIX = /tmp/root
SRC = src/
DOC = doc/
ARCHIVE = src.tar.gz

.PHONY: all doc clean

all:
	make -C $(SRC) $@
	make -C $(DOC) $@

#Dependances
depends:
	make depends -C $(SRC) $@

#Clean
clean:
	make clean -C $(SRC) $@
	make clean -C $(DOC) $@

#Installation
install:
	make install -C $(SRC) $@
	make install -C $(DOC) $@

#Désinstallation
uninstall:
	make uninstall -C $(SRC) $@
	make uninstall -C $(DOC) $@

#Compresser sources
tgz:
	tar -zcvf $(ARCHIVE) $(SRC)

untgz:
	tar -zxvf $(ARCHIVE)
