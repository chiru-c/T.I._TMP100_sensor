SRC = i2c.c
OBJ = i2c.o
PROJ = i2c

$(PROJ) : $(OBJ)
	gcc $(OBJ) -o $(PROJ)

$(OBJ) : $(SRC)

clean:
	rm $(OBJ) $(PROJ)
