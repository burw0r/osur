
POCETNI_DIREKTORIJ = $(shell pwd)
export POCETNI_DIREKTORIJ

include $(POCETNI_DIREKTORIJ)/config.ini

OBJEKTI = main.o dodatak.o
MODULI = animals/mod-animals.o plants/mod-plants.o


executable: $(OBJEKTI) $(MODULI)
	$(CC) $(LDFLAGS) $^ -o executable # $@


main.o: main.c   #implicit

dodatak.o: dodatak.c dodatak.h


.PHONY: $(MODULI)
$(MODULI):
	$(MAKE) -C $(dir $@)


.PHONY: pokreni
pokreni: executable
	./executable


.PHONY: brisi
brisi:
	make -C animals brisi
	make -C plants brisi
	rm -f $(OBJEKTI) executable
