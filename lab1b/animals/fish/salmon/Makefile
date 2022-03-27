include $(POCETNI_DIREKTORIJ)/config.ini


OBJEKTI = salmon.o

mod-salmon.o: $(OBJEKTI)
	$(LD) -r $^ $(LDFLAGS) -o $@

salmon.o: salmon.c salmon.h
	$(CC) $(CFLAGS) -o $@ -c $<


.PHONY: $(MODULI)
$(MODULI):
	$(MAKE) -C $(dir $@)

.PHONY: brisi
brisi:
	-rm -f $(OBJEKTI) mod-salmon.o
