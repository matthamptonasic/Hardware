CC = g++
CCFLAGS = -fPIC -std=c++11
ODIR = $(DUMP_DIR)
SDIR = .
INC = -I./ \
			-I../BitVector \
			-I../Common \
			-I../DataTypes \
			-I../Environment \
			-I../Event \
			-I../Logging \
			-I../Test \
			-I$(VPI_USER)
VPATH = ./ \
				../BitVector \
				../Common \
			  ../DataTypes \
				../Environment \
				../Event \
				../Logging \
				../Test \
				$(VPI_USER)

SRCS = $(wildcard *.cc)

OBJS = $(patsubst %.cc,$(ODIR)/%.o,$(SRCS))

all : $(OBJS)

$(ODIR)/%.o: $(SDIR)/%.cc  
	$(CC) $(CCFLAGS) -c $(INC) -o $@ $< $(CFLAGS) 

