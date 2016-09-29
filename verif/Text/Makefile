CC = g++
ODIR = $(DUMP_DIR)
SDIR = .
INC = -I../Common -I../Pli -I$(VPI_USER)
VPATH = ../Common ../Pli $(VPI_USER)

_OBJS = Manip.o \
			  Parsing.o

OBJS = $(patsubst %,$(ODIR)/%,$(_OBJS))

all : $(OBJS)

$(ODIR)/%.o: $(SDIR)/%.cc  
	$(CC) $(CCFLAGS) -c $(INC) -o $@ $< $(CFLAGS) 

