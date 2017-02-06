NOM = vectorisation
LIBS_DIR = libs
CC = gcc
SEUIL = 75


$(NOM): main.o bmp_worker.o io.o squelettisation.o transformee.o vectorisation.o
	$(CC) -o $(NOM) main.o bmp_worker.o io.o squelettisation.o transformee.o vectorisation.o

test: $(NOM)
	./$(NOM) ligne.bmp $(SEUIL)
	./$(NOM) lignes.bmp $(SEUIL)
	./$(NOM) triangle.bmp $(SEUIL)
	./$(NOM) o.bmp $(SEUIL)
	./$(NOM) u.bmp $(SEUIL)
	./$(NOM) x.bmp $(SEUIL)

main.o: main.c
	$(CC) -c main.c

bmp_worker.o: $(LIBS_DIR)/bmp_worker.c $(LIBS_DIR)/bmp_worker.h
	$(CC) -c $(LIBS_DIR)/bmp_worker.c

io.o: $(LIBS_DIR)/io.c $(LIBS_DIR)/io.h
	$(CC) -c $(LIBS_DIR)/io.c

squelettisation.o: $(LIBS_DIR)/squelettisation.c $(LIBS_DIR)/squelettisation.h
	$(CC) -c $(LIBS_DIR)/squelettisation.c

transformee.o: $(LIBS_DIR)/transformee.c $(LIBS_DIR)/transformee.h
	$(CC) -c $(LIBS_DIR)/transformee.c

vectorisation.o: $(LIBS_DIR)/vectorisation.c $(LIBS_DIR)/vectorisation.h
	$(CC) -c $(LIBS_DIR)/vectorisation.c

clean: *.o
	rm *.o

reset: clean
	rm $(NOM)