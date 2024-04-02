# WARNING! Update this macro before deploy
PROJECT = etapa2

IDIR =./include
CC=gcc
CFLAGS=-I$(IDIR)

ODIR=obj
CDIR=src

# You MUST list all header files here
_DEPS = parser.tab.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

# You MUST list all object files here
_OBJ = main.o lex.yy.o parser.tab.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

all: scanner parser $(PROJECT)

scanner: parser $(CDIR)/scanner.l
	flex -o $(CDIR)/lex.yy.c $(CDIR)/scanner.l
	$(CC) -c $(CDIR)/lex.yy.c -o $(ODIR)/lex.yy.o $(CFLAGS)

parser: $(CDIR)/parser.y
	bison -o $(CDIR)/parser.tab.c --defines=$(IDIR)/parser.tab.h $(CDIR)/parser.y 
	$(CC) -c $(CDIR)/parser.tab.c -o $(ODIR)/parser.tab.o $(CFLAGS)

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