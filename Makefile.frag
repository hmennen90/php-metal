# Compile Objective-C .m files as .lo objects for the PHP build system
metal.lo: $(srcdir)/metal.m
	$(LIBTOOL) --tag=CC --mode=compile $(CC) -I. -I$(srcdir) $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -fobjc-arc -DCOMPILE_DL_METAL -DHAVE_CONFIG_H -c $(srcdir)/metal.m -o metal.lo -MMD -MF metal.dep -MT metal.lo
