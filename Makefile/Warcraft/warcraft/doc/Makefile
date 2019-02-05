DIR = /usr/share/man/man6/

all:
	pod2man --utf8 -r 0.1 -c "Game" -s 6 warcraft.pod > warcraft.6

#Clean
clean:
	rm warcraft.6

#Installation
install:
	mkdir -p $(PREFIX)$(DIR)
	install -D -m 644 warcraft.6 $(PREFIX)$(DIR)

#Désinstallation
uninstall:
	rm $(PREFIX)$(DIR)warcraft.6
