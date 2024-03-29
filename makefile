# WARNING! Update this macro before deploy
PROJECT = etapa1

IDIR =./include
CC=gcc
CFLAGS=-I$(IDIR)

ODIR=obj
CDIR=src

# You MUST list all header files here
_DEPS = tokens.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

# You MUST list all object files here
_OBJ = main.o lex.yy.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

all: scanner $(PROJECT)

scanner: $(CDIR)/scanner.l
	flex -o $(CDIR)/lex.yy.c $(CDIR)/scanner.l
	$(CC) -c $(CDIR)/lex.yy.c -o $(ODIR)/lex.yy.o $(CFLAGS)

$(ODIR)/%.o: $(CDIR)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(PROJECT): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

# --------------
# RECIPES ONLY:

.PHONY: clean
clean:
	rm -f $(ODIR)/*.o
	rm -f deploy/*.tgz
	rm -f $(PROJECT)
	rm -f $(CDIR)/lex.yy.c

.PHONY: deploy
deploy:
	make clean
	tar cvzf deploy/$(PROJECT).tgz\
	 --exclude=.git\
	 --exclude=deploy\
	 --exclude=README.md\
	 --exclude=.gitignore\
	 --exclude=tests\
	 --exclude=.vscode\
	 .