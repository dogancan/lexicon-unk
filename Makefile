# Set these two in command line make call or modify them here
FSTDIR=/Users/dogan/Work/software/openfst-1.4.1
CC=clang++ -std=c++11

UNAME := $(shell uname)

FSTLIBDIR=$(FSTDIR)/lib
FSTINCDIR=$(FSTDIR)/include
BINDIR=$(PWD)
INCS=$(FSTINCDIR)/fst/*.h $(FSTINCDIR)/fst/script/*.h
BINS=fstphirhocompose

ifeq ($(UNAME), Linux)
  LIBTYPE=so
else ifeq ($(UNAME), Darwin)
  LIBTYPE=dylib
endif

FSTLIB=$(FSTLIBDIR)/libfst.$(LIBTYPE)
FSTSCRIPTLIB=$(FSTLIBDIR)/libfstscript.$(LIBTYPE)
FSTFARLIB=$(FSTLIBDIR)/fst/libfstfar.$(LIBTYPE)
LIBS= -lfst -lm -lpthread -ldl
OPT=-O2
CFLAGS=-I$(FSTINCDIR) -I$(INCDIR) $(OPT) -DFST_DL -fPIC
LDLFLAGS=-Wl,-L$(FSTLIBDIR),-L$(FSTLIBDIR)/fst,-L$(BINDIR)

ifeq ($(UNAME), Linux)
  LDRFLAGS=-Wl,-rpath,$(FSTLIBDIR),-rpath,$(BINDIR)
else ifeq ($(UNAME), Darwin)
  LDRFLAGS=
endif

LDFLAGS=$(LDLFLAGS) $(LDRFLAGS)

all: $(BINS)

fstphirhocompose: fstphirhocompose.o $(FSTLIB)
	$(CC) $(LDFLAGS) -o $@ $@.o $(LIBS)

fstphirhocompose.o: fstphirhocompose.cc $(INCS) Makefile
	$(CC) $(CFLAGS) -o $@ -c $*.cc
