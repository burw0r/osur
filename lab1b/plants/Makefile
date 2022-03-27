include $(POCETNI_DIREKTORIJ)/config.ini

OBJEKTI = plants.o

mod-plants.o: plants.o
	$(LD) -r $^ $(LDFLAGS) -o $@   #$@ -> mod-plants.o

plants.o: plants.c plants.h
	$(CC) $(CFLAGS) -o $@ -c $<

.PHONY: brisi
brisi:
	rm -f $(OBJEKTI) mod-plants.o
