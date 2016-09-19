CC = g++
CCFLAGS = -fPIC -std=c++11
ODIR = $(DUMP_DIR)
SDIR = .
INC = -I../Common -I$(VPI_USER) -I../Signals
VPATH = ../Common $(VPI_USER) ../Signals

_OBJS = vpi_entry.o \
				vpi.o \
				V_Signal.o

OBJS = $(patsubst %,$(ODIR)/%,$(_OBJS))

all : $(OBJS)

$(ODIR)/%.o: $(SDIR)/%.cc  
	$(CC) $(CCFLAGS) -c $(INC) -o $@ $< $(CFLAGS) 

