CC = g++
ODIR = $(DUMP_DIR)
SDIR = .
INC = -I../Common \
			-I../Logging \
		  -I../Pli \
			-I$(VPI_USER) \
			-I../Text 
			
VPATH = ../Common \
				../Logging \
			  ../Pli \
				$(VPI_USER) \
				../Text 

SRCS = $(wildcard *.cc)

OBJS = $(patsubst %.cc,$(ODIR)/%.o,$(SRCS))

all : $(OBJS)

$(ODIR)/%.o: $(SDIR)/%.cc  
	$(CC) $(CCFLAGS) -c $(INC) -o $@ $< $(CFLAGS) 

