CC = g++
CCFLAGS = -fPIC -std=c++11
ODIR = $(DUMP_DIR)
SDIR = .
INC = -I./ \
			-I../Common \
			-I$(VPI_USER) \
			-I../Signals \
			-I../BitVector \
			-I../Logging \
			-I../Environment \
			-I../Test
VPATH = ./ \
				../Common \
				$(VPI_USER) \
				../Signals \
				../BitVector \
				../Logging \
				../Environment \
				../Test

SRCS = $(wildcard *.cc)

OBJS = $(patsubst %.cc,$(ODIR)/%.o,$(SRCS))

all : $(OBJS)

$(ODIR)/%.o: $(SDIR)/%.cc  
	$(CC) $(CCFLAGS) -c $(INC) -o $@ $< $(CFLAGS) 

