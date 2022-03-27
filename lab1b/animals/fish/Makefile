include $(POCETNI_DIREKTORIJ)/config.ini


OBJEKTI = fish.o
MODULI = salmon/mod-salmon.o tuna/mod-tuna.o


mod-fish.o: $(OBJEKTI) $(MODULI)
	$(LD) -r $^ $(LDFLAGS) -o $@

fish.o: fish.c fish.h
	$(CC) $(CFLAGS) -o $@ -c $<


.PHONY: $(MODULI)
$(MODULI):
	$(MAKE) -C $(dir $@)

.PHONY: brisi
brisi:
	-rm -f $(OBJEKTI) mod-fish.o
	make -C salmon brisi
	make -C tuna brisi
