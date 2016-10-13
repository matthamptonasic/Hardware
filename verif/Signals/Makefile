CC = g++
CCFLAGS = -fPIC 
ODIR = $(DUMP_DIR)
SDIR = .
INC = -I../Common \
			-I$(VPI_USER) \
			-I../Logging/ \
			-I../Pli/
VPATH = ../Common \
				$(VPI_USER) \
				../Logging/ \
				../Pli/

SRCS = $(wildcard *.cc)

OBJS = $(patsubst %.cc,$(ODIR)/%.o,$(SRCS))

all : $(OBJS)

$(ODIR)/%.o: $(SDIR)/%.cc  
	$(CC) $(CCFLAGS) -c $(INC) -o $@ $< $(CFLAGS) 

