SRC = tmp100_i2c.c
OBJ = tmp100_i2c.o
PROJ = tmp100_i2c

$(PROJ) : $(OBJ)
	gcc $(OBJ) -o $(PROJ)

$(OBJ) : $(SRC)

clean:
	rm $(OBJ) $(PROJ)
