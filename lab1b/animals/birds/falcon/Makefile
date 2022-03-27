include $(POCETNI_DIREKTORIJ)/config.ini


OBJEKTI = falcon.o

mod-falcon.o: $(OBJEKTI)
	$(LD) -r $^ $(LDFLAGS) -o $@

falcon.o: falcon.c falcon.h
	$(CC) $(CFLAGS) -o $@ -c $<


.PHONY: $(MODULI)
$(MODULI):
	$(MAKE) -C $(dir $@)

.PHONY: brisi
brisi:
	-rm -f $(OBJEKTI) mod-falcon.o
