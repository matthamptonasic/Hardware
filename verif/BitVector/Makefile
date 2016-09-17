CC = g++
ODIR = $(DUMP_DIR)
SDIR = .
INC = -I../Common
VPATH = ../Common

_OBJS = BitVector.o

OBJS = $(patsubst %,$(ODIR)/%,$(_OBJS))

all : $(OBJS)

$(ODIR)/%.o: $(SDIR)/%.cc  
	$(CC) $(CCFLAGS) -c $(INC) -o $@ $< $(CFLAGS) 

