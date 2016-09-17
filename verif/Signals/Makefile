CC = g++
CCFLAGS = -fPIC 
ODIR = $(DUMP_DIR)
SDIR = .
INC = -I../Common -I/usr/local/bin/iverilog_v10 
VPATH = ../Common /usr/local/bin/iverilog_v10 

_OBJS = V_Signal.o \
				V_Reg.o

OBJS = $(patsubst %,$(ODIR)/%,$(_OBJS))

all : $(OBJS)

$(ODIR)/%.o: $(SDIR)/%.cc  
	$(CC) $(CCFLAGS) -c $(INC) -o $@ $< $(CFLAGS) 

