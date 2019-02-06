#TP n°2 : Bibliothèque + mémoire partagée

## Première partie : utilisation d'une bibliothèque

### Désarchivez-là dans /tmp:
```
	cd ~/Téléchargements/
	mv graphic.tgz /tmp/
	cd /tmp
	tar -zxvf graphic.tgz
```

### Étudiez sa documentation (graphic.3.gz):
```
	cd graphic/share/man/man6/
	chmod 755 graphic.6.gz
	man ./graphic.6.gz
```
### Récupérez dans votre HOME le programme test.c ci-dessous, étudiez-le...
```
	cat test.c
```
### ...puis, sans changer son code source vous :

#### - générerez en ligne de commande le module objet test.o
```
	gcc -Wall -c test.c -I /tmp/graphic/usr/include/
```
#### - générerez en ligne de commande le programme test
```
	gcc -o test test.o -lX11 -lgraphic -L /tmp/graphic/usr/lib/
```
#### - exécuterez le programme test
```
	export LD_LIBRARY_PATH="$LD_LIBRARY_PATH:/tmp/graphic/usr/lib/"
	./test
```

## Seconde partie : un petit Makefile:

### Écrivez les règles all et clean du Makefile permettant de générer l'exécutable test:
```
	CC = gcc
	CFLAGS = -Wall
	INCLUDE = -I /tmp/graphic/usr/include/
	LIBRARY = -L /tmp/graphic/usr/lib/
	LIB = -lX11 -lgraphic
	EXECUTABLE = test

	all: $(EXECUTABLE)

	%.o: %.c
		$(CC) $(CFLAGS) -c $< $(INCLUDE)

	%: %.o
		$(CC) $^ -o $@ $(LIB) $(LIBRARY)

	test: test.o
	test.o:test.c

	clean:
		rm -f *.o $(EXECUTABLE)

```
## Troisième partie : mémoire partagée:

### Modifiez le fichier test.c pour qu'il corresponde à la mise en oeuvre des algorithmes précédents:



Notes:
fichier creux:
	dd of=~/fic bs=1G seek=2000 count=0
	ls -lh fic
	du fic
