CC = g++
CCFLAGS = -fPIC 
ODIR = $(DUMP_DIR)
SDIR = .
INC = -I../Common \
			-I$(VPI_USER) \
			-I../Pli/
VPATH = ../Common \
				$(VPI_USER) \
				../Pli/

SRCS = $(wildcard *.cc)

OBJS = $(patsubst %.cc,$(ODIR)/%.o,$(SRCS))

all : $(OBJS)

$(ODIR)/%.o: $(SDIR)/%.cc  
	$(CC) $(CCFLAGS) -c $(INC) -o $@ $< $(CFLAGS) 

