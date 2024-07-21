# Project's permanent macros ==================================
IDIR =./include
CC=gcc
CFLAGS=-I$(IDIR)

ODIR=obj
CDIR=src

# Project's configurable macros ===============================
# WARNING! Change this at every deploy
PROJECT = etapa4

# You MUST list all header files here
_DEPS = parser.tab.h ast.h dataType.h symbolTable.h tableStack.h semanticErrors.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

# You MUST list all object files here
_OBJ = main.o lex.yy.o parser.tab.o ast.o dataType.o symbolTable.o tableStack.o semanticErrors.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

# Default command =============================================
all: parser scanner $(PROJECT)

# 1. Parser (Bison - Semantic analysis) =======================
$(CDIR)/parser.tab.c: $(CDIR)/parser.y
	bison -o $(CDIR)/parser.tab.c --defines=$(IDIR)/parser.tab.h $(CDIR)/parser.y 

$(IDIR)/parser.tab.h: $(CDIR)/parser.tab.c
	@touch $@

parser: $(CDIR)/parser.tab.c $(IDIR)/parser.tab.h

# 2. Scanner (Flex - Lexical analysis) ========================
$(CDIR)/lex.yy.c: $(CDIR)/scanner.l 
	flex -o $(CDIR)/lex.yy.c $(CDIR)/scanner.l

scanner: $(CDIR)/lex.yy.c

# 3. Compile project ==========================================
$(ODIR)/%.o: $(CDIR)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(PROJECT): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

# =============================================================
# Recipes only
.PHONY: clean
clean:
	rm -f $(ODIR)/*.o
	rm -f deploy/*.tgz
	rm -f $(PROJECT)
	rm -f $(CDIR)/lex.yy.c
	rm -f $(CDIR)/parser.tab.c
	rm -f $(IDIR)/parser.tab.h

.PHONY: deploy
deploy:
	make clean
	tar cvzf deploy/$(PROJECT).tgz\
	 --exclude=.git\
	 --exclude=deploy\
	 --exclude=scripts\
	 --exclude=README.md\
	 --exclude=.gitignore\
	 --exclude=tests\
	 --exclude=.vscode\
	 .