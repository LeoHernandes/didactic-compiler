PROJECT = etapa1
CC = gcc
CFLAGS = 
SRC_FILES = src/*.cpp
OUT_DIR = .

all: $(PROJECT)

# build app
$(PROJECT):
	$(CC) $(CFLAGS) -o $(OUT_DIR)/$(PROJECT) $(SRC_FILES)

# --------------
# RECIPES ONLY:
.PHONY: deploy
deploy:
	rm -f *.o
	rm -f $(PROJECT)
	tar cvzf $(PROJECT).tgz .
	