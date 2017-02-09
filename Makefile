NOM = vectorisation
LIBS_DIR = libs
CC = gcc
SEUIL = 100


$(NOM): main.o bmp_worker.o io.o squelettisation.o transformee.o vectorisation.o simplification.o
	$(CC) -o $(NOM) *.o -lm

test: $(NOM)
	./$(NOM) ligne.bmp $(SEUIL)
	./$(NOM) lignes.bmp $(SEUIL)
	./$(NOM) lignes2.bmp $(SEUIL)
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

vectorisation.o: $(LIBS_DIR)/vectorisation.c $(LIBS_DIR)/vectorisation.h squelettisation.o transformee.o
	$(CC) -c $(LIBS_DIR)/vectorisation.c

simplification.o: $(LIBS_DIR)/simplification.c $(LIBS_DIR)/simplification.h vectorisation.o
	$(CC) -c $(LIBS_DIR)/simplification.c

clean:
	rm -f *.o

reset: clean
	rm -f *.tex
	rm -f $(NOM)