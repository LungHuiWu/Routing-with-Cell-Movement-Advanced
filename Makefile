# variables
CC = g++ -std=c++11
CFLAGS = -c
OPTFLAGS = -O3

all : cell_move_router
	@echo "Compile Finished!!!"

# optimized version
cell_move_router: main.o Design.o GGrid.o Net.o CellInst.o Blkg.o Pin.o MCell.o Route.o Layer.o
	@$(CC) $(OPTFLAGS) main.o Design.o GGrid.o Net.o CellInst.o Blkg.o Pin.o MCell.o Route.o Layer.o -o cell_move_router

main.o: main.cpp Design.h
	@$(CC) $(CFLAGS) $(OPTFLAGS) $< -o $@

Design.o: Design.cpp Design.h MCell.h Pin.h Net.h GGrid.h Route.h CellInst.h
	@$(CC) $(CFLAGS) $(OPTFLAGS) $< -o $@

MCell.o:MCell.cpp MCell.h Pin.h Blkg.h
	@$(CC) $(CFLAGS) $(OPTFLAGS) $< -o $@

Net.o:Net.cpp Net.h CellInst.h Route.h GGrid.h
	@$(CC) $(CFLAGS) $(OPTFLAGS) $< -o $@

CellInst.o:CellInst.cpp CellInst.h MCell.h Pin.h Blkg.h
	@$(CC) $(CFLAGS) $(OPTFLAGS) $< -o $@

Blkg.o:Blkg.cpp Blkg.h Layer.h
	@$(CC) $(CFLAGS) $(OPTFLAGS) $< -o $@

GGrid.o:GGrid.cpp GGrid.h Layer.h
	@$(CC) $(CFLAGS) $(OPTFLAGS) $< -o $@

Pin.o:Pin.cpp Pin.h Layer.h
	@$(CC) $(CFLAGS) $(OPTFLAGS) $< -o $@

Route.o:Route.cpp Route.h
	@$(CC) $(CFLAGS) $(OPTFLAGS) $< -o $@

Layer.o:Layer.cpp Layer.h
	@$(CC) $(CFLAGS) $(OPTFLAGS) $< -o $@

# clean
clean:
	@rm -f *.o 