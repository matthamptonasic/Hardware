CC = g++
CCFLAGS = -fPIC -std=c++11
ODIR = $(DUMP_DIR)
SDIR = .
INC = -I../Common -I$(VPI_USER) -I../Signals -I. -I../BitVector -I../Logging -I../Test
VPATH = ../Common $(VPI_USER) ../Signals . ../BitVector ../Logging ../Test

_OBJS = vpi_entry.o \
				vpi.o \
				pli.o \
				V_Signal.o

OBJS = $(patsubst %,$(ODIR)/%,$(_OBJS))

all : $(OBJS)

$(ODIR)/%.o: $(SDIR)/%.cc  
	$(CC) $(CCFLAGS) -c $(INC) -o $@ $< $(CFLAGS) 

