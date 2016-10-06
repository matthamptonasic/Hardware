ODIR = $(DUMP_DIR)
CCFLAGS = -fPIC -std=c++11
SDIR = .
INC = -I$(PROJECT_ROOT)/verif/Common \ 
			-I$(PROJECT_ROOT)/verif/Test 
			
VPATH = $(PROJECT_ROOT)/verif/Common \
				$(PROJECT_ROOT)/verif/Test 

_OBJS = $(patsubst %.cc,%.o,$(wildcard *.cc))

OBJS = $(patsubst %,$(ODIR)/%,$(_OBJS))

all:
	cd monitors && $(MAKE) -e CCFLAGS="$(CCFLAGS)"
	cd drivers && $(MAKE) -e CCFLAGS="$(CCFLAGS)"
	cd testbench && $(MAKE) -e CCFLAGS="$(CCFLAGS)"

cleanall:
	rm -f $(ODIR)/*.o

