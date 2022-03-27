include $(POCETNI_DIREKTORIJ)/config.ini


OBJEKTI = owl.o

mod-owl.o: $(OBJEKTI)
	$(LD) -r $^ $(LDFLAGS) -o $@

owl.o: owl.c owl.h
	$(CC) $(CFLAGS) -o $@ -c $<


.PHONY: $(MODULI)
$(MODULI):
	$(MAKE) -C $(dir $@)

.PHONY: brisi
brisi:
	-rm -f $(OBJEKTI) mod-owl.o
