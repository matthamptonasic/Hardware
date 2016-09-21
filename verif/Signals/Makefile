CC = g++
CCFLAGS = -fPIC 
ODIR = $(DUMP_DIR)
SDIR = .
INC = -I../Common -I$(VPI_USER) -I../Pli/
VPATH = ../Common $(VPI_USER) ../Pli/

_OBJS = V_Signal.o \
				V_Reg.o

OBJS = $(patsubst %,$(ODIR)/%,$(_OBJS))

all : $(OBJS)

$(ODIR)/%.o: $(SDIR)/%.cc  
	$(CC) $(CCFLAGS) -c $(INC) -o $@ $< $(CFLAGS) 

