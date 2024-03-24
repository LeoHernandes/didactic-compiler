# WARNING! Update this macro before deploy
PROJECT = etapa1

CC=gcc
CFLAGS=-I.

# You MUST list all `header` files here
DEPS =

# You MUST list all `object` files here
OBJ = main.o 

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(PROJECT): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

# --------------
# RECIPES ONLY:
.PHONY: deploy

deploy:
	rm -f *.o
	rm -f $(PROJECT)
	tar cvzf deploy/$(PROJECT).tgz\
	 --exclude=.git\
	 --exclude=deploy\
	 --exclude=README.md\
	 --exclude=.gitignore\
	 .