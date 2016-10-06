CC = g++
ODIR = $(DUMP_DIR)
SDIR = .
INC = -I$(PROJECT_ROOT)/verif/Common \
      -I$(PROJECT_ROOT)/verif/Test \
      -I$(PROJECT_ROOT)/verif/Pli \
			-I$(VPI_USER)
			
VPATH = $(PROJECT_ROOT)/verif/Common \
        $(PROJECT_ROOT)/verif/Test \
        $(PROJECT_ROOT)/verif/Pli \
				$(VPI_USER) 

SRCS = $(wildcard *.cc)

OBJS = $(patsubst %.cc,$(ODIR)/%.o,$(SRCS))

all : $(OBJS)

$(ODIR)/%.o: $(SDIR)/%.cc  
	$(CC) $(CCFLAGS) -c $(INC) -o $@ $< $(CFLAGS) 

