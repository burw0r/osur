include $(POCETNI_DIREKTORIJ)/config.ini


OBJEKTI = tuna.o

mod-tuna.o: $(OBJEKTI)
	$(LD) -r $^ $(LDFLAGS) -o $@

tuna.o: tuna.c tuna.h
	$(CC) $(CFLAGS) -o $@ -c $<


.PHONY: $(MODULI)
$(MODULI):
	$(MAKE) -C $(dir $@)

.PHONY: brisi
brisi:
	-rm -f $(OBJEKTI) mod-tuna.o
