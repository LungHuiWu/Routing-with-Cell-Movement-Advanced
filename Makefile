#
# This is for your executable
#
CFLAGS =-g -Wall -std=c++11
CFLAGS = -O3 -std=c++11
#
# This is the setting for TA
#
# CFLAGS = -g -Wall -DTA_KB_SETTING -std=c++11
# CFLAGS = -O3 -DTA_KB_SETTING -std=c++11
# CFLAGS = -O3 -m32 -DTA_KB_SETTING -std=c++11

###############
# PHONY rules #
###############
.PHONY : eda

eda : MAKEFLAGS = $(CFLAGS)

eda : .cell_move_router


##############
# Executable #
#############
cell_move_router: main.o Design.o GGrid.o Net.o CellInst.o Blkg.o Pin.o MCell.o Route.o Layer.o
	g++ -o $@ $(MAKEFLAGS) main.o Design.o GGrid.o Net.o CellInst.o Blkg.o Pin.o MCell.o Route.o Layer.o
#	strip $@



################
# Object files #
################
main.o: main.cpp Design.h
	g++ -g -c $(MAKEFLAGS) main.cpp

Design.o: Design.cpp Design.h MCell.h Pin.h Net.h GGrid.h Route.h CellInst.h
	g++ -g -c $(MAKEFLAGS) Design.cpp

MCell.o:MCell.cpp MCell.h Pin.h Blkg.h
	g++ -g -c $(MAKEFLAGS) MCell.cpp

Net.o:Net.cpp Net.h CellInst.h Route.h GGrid.h
	g++ -g -c $(MAKEFLAGS) Net.cpp

CellInst.o:CellInst.cpp CellInst.h MCell.h Pin.h Blkg.h
	g++ -g -c $(MAKEFLAGS) CellInst.cpp

Blkg.o:Blkg.cpp Blkg.h Layer.h
	g++ -g -c $(MAKEFLAGS) Blkg.cpp

GGrid.o:GGrid.cpp GGrid.h Layer.h
	g++ -g -c $(MAKEFLAGS) GGrid.cpp

Pin.o:Pin.cpp Pin.h Layer.h
	g++ -g -c $(MAKEFLAGS) Pin.cpp

Route.o:Route.cpp Route.h
	g++ -c $(MAKEFLAGS) Route.cpp

Layer.o:Layer.cpp Layer.h
	g++ -c $(MAKEFLAGS) Layer.cpp

#######################
# No dependency rules #
#######################

clean:
	rm -f *.o 

ctags:
	ctags *.cpp *.h