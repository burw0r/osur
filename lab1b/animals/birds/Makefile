include $(POCETNI_DIREKTORIJ)/config.ini


OBJEKTI = birds1.o birds2.o
MODULI = falcon/mod-falcon.o owl/mod-owl.o


mod-birds.o: $(OBJEKTI) $(MODULI)
	$(LD) -r $^ $(LDFLAGS) -o $@

birds1.o: birds1.c birds1.h
	$(CC) $(CFLAGS) -o $@ -c $<

birds2.o: birds2.c birds2.h
	$(CC) $(CFLAGS) -o $@ -c $<


.PHONY: $(MODULI)
$(MODULI):
	$(MAKE) -C $(dir $@)

.PHONY: brisi
brisi:
	-rm -f $(OBJEKTI) mod-birds.o
	make -C falcon brisi
	make -C owl brisi
