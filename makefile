# WARNING! Update this macro before deploy
PROJECT = etapa1

CC=gcc
CFLAGS=-I.

# You MUST list all `header` files here
DEPS = tokens.h

# You MUST list all `object` files here
OBJ = main.o lex.yy.c

all: scanner $(PROJECT)

scanner: scanner.l
	flex scanner.l

$(PROJECT): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

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