DIR = /usr/bin/
.PHONY: all doc clean

all: warcraft editeur

#Compilations
%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@

#Editions de lien
%: %.o
	$(CC) $^ -o $@

#Dependences editions de liens
warcraft: warcraft.o video.o perso.o
editeur: editeur.o perso.o video.o

#Dependances
depends:
	makedepend -Y *.c

#Clean
clean:
	rm -f *.o warcraft editeur

#Installation
install:
	mkdir -p $(PREFIX)$(DIR)
	install -D -m 755 editeur $(PREFIX)$(DIR)
	install -D -m 755 warcraft $(PREFIX)$(DIR)

#Désinstallation
uninstall:
	rm -f $(PREFIX)$(DIR)editeur
	rm -f $(PREFIX)$(DIR)warcraft

# DO NOT DELETE

editeur.o: perso.h video.h editeur.h
perso.o: perso.h video.h
video.o: video.h
warcraft.o: video.h perso.h warcraft.h
