DEBUG=yes
CC=g++
EXEC=test
#SRC= $(wildcard *.cpp)
SRC=test.cpp
OBJ= $(SRC:.c=.o)

LDFLAGS= `pkg-config --cflags --libs pocketsphinx sphinxbase`
#CONFIG=-DMODELDIR=\"`pkg-config --variable=modeldir pocketsphinx`\"
#POCOFLAGS=-lPocoFoundation
POCOFLAGS=

ifeq ($(DEBUG),yes)
	CFLAGS=-W -Wall -ansi -pedantic -g
else
	CFLAGS=
endif

all: $(EXEC)
ifeq ($(DEBUG),yes)
	@echo "Generation in debug mode"
else
	@echo "Generation in release mode"
endif

test: $(OBJ)
	@$(CC) -o $@ $^ $(LDFLAGS) $(POCOFLAGS)

%.o: %.cpp
	@$(CC) -o $@ -c $< $(CFLAGS)

.PHONY: clean mrproper

clean:
	@rm -rf *.o

mrproper: clean
	@rm -rf $(EXEC)
