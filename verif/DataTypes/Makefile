CC = g++
CCFLAGS = -fPIC 
ODIR = $(DUMP_DIR)
SDIR = .
INC = -I../Common \
			-I../Event \
			-I../Logging \
			-I../Pli \
			-I../Text \
			-I$(VPI_USER)
VPATH = ../Common \
				../Event \
				../Logging \
				../Pli \
				../Text \
				$(VPI_USER)

SRCS = $(wildcard *.cc)

OBJS = $(patsubst %.cc,$(ODIR)/%.o,$(SRCS))

all : $(OBJS)

$(ODIR)/%.o: $(SDIR)/%.cc  
	$(CC) $(CCFLAGS) -c $(INC) -o $@ $< $(CFLAGS) 

