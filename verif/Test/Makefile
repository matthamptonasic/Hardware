CC = g++
ODIR = $(DUMP_DIR)
SDIR = .
INC = -I../Common \
		  -I../Pli \
			-I$(VPI_USER) \
			-I../Text 
			
VPATH = ../Common \
			  ../Pli \
				$(VPI_USER) \
				../Text 

_OBJS = TestController.o \
				TestBase.o \
				TestDB.o

OBJS = $(patsubst %,$(ODIR)/%,$(_OBJS))

all : $(OBJS)

$(ODIR)/%.o: $(SDIR)/%.cc  
	$(CC) $(CCFLAGS) -c $(INC) -o $@ $< $(CFLAGS) 

