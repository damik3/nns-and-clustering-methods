BINDIR=./bin
IDIR=./include
MODDIR=./modules
ODIR=./obj
SRCDIR=./src

CC=g++
CFLAGS= -std=c++0x -I $(IDIR) -Wall -g

DEPS = $(shell find $(IDIR) -name *.h -o -name *.hpp)

#OBJ1 = $(ODIR)/search.o $(ODIR)/Image.o $(ODIR)/ModOp.o  $(ODIR)/HFunc.o $(ODIR)/GFunc.o $(ODIR)/LSH.o $(ODIR)/IDX.o $(ODIR)/Dist.o
OBJ1 = $(ODIR)/search.o $(ODIR)/Image.o $(ODIR)/ModOp.o $(ODIR)/IDX.o 

all: $(BINDIR)/search

$(BINDIR)/search: $(OBJ1)
	$(CC) -o $@ $^ $(CFLAGS)

$(ODIR)/%.o: $(SRCDIR)/%.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(ODIR)/%.o: $(MODDIR)/%.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)
