include $(POCETNI_DIREKTORIJ)/config.ini


OBJEKTI = animals.o
MODULI = birds/mod-birds.o fish/mod-fish.o


mod-animals.o: $(OBJEKTI) $(MODULI)
	$(LD) -r $^ $(LDFLAGS) -o $@

animals.o: animals.c animals.h
	$(CC) $(CFLAGS) -o $@ -c $<


.PHONY: $(MODULI)
$(MODULI):
	$(MAKE) -C $(dir $@)

.PHONY: brisi
brisi:
	-rm -f $(OBJEKTI) mod-animals.o
	make -C birds brisi
	make -C fish brisi
