CC = g++
ODIR = $(DUMP_DIR)
SDIR = .
INC = -I../Common -I../Pli -I$(VPI_USER)
VPATH = ../Common ../Pli $(VPI_USER)

SRCS = $(wildcard *.cc)

OBJS = $(patsubst %.cc,$(ODIR)/%.o,$(SRCS))

all : $(OBJS)

$(ODIR)/%.o: $(SDIR)/%.cc  
	$(CC) $(CCFLAGS) -c $(INC) -o $@ $< $(CFLAGS) 

