#
#       Copyright abandoned, 1983, The Rand Corporation
#




OFILES = ambas.o powerup.o


ambas: $(OFILES) ambas.h
	$(CC) $(CFLAGS) $(OFILES)
	-rm -f ambas
	mv a.out ambas

distribution: clean

clean:
	rm -f ambas
