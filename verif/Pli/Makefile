CC = g++
CCFLAGS = -fPIC -std=c++11
ODIR = $(DUMP_DIR)
SDIR = .
INC = -I./ \
			-I../BitVector \
			-I../Common \
			-I../Environment \
			-I../Event \
			-I../Logging \
			-I../Signals \
			-I../Test \
			-I$(VPI_USER)
VPATH = ./ \
				../BitVector \
				../Common \
				../Environment \
				../Event \
				../Logging \
				../Signals \
				../Test \
				$(VPI_USER)

SRCS = $(wildcard *.cc)

OBJS = $(patsubst %.cc,$(ODIR)/%.o,$(SRCS))

all : $(OBJS)

$(ODIR)/%.o: $(SDIR)/%.cc  
	$(CC) $(CCFLAGS) -c $(INC) -o $@ $< $(CFLAGS) 

