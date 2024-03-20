# WARNING! Update this macro before deploy
PROJECT = etapa1

IDIR =./include
CC=gcc
CFLAGS=-I$(IDIR)

ODIR=obj
CDIR=src

# You MUST list all header files here
_DEPS =
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

# You MUST list all object files here
_OBJ = main.o 
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: $(CDIR)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(PROJECT): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

# --------------
# RECIPES ONLY:
.PHONY: deploy

deploy:
	rm -f $(PROJECT).tgz
	rm -f $(ODIR)/*.o
	rm -f $(PROJECT)
	tar cvzf $(PROJECT).tgz .