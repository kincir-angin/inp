bin_PROGRAMS = inp
inp_SOURCES = src/main.c src/install.c src/create-package.c src/utils/common.c src/utils/pair.c

inp_CFLAGS = -I$(top_srcdir) $(INCLUDE)
inp_LDFLAGS = -larchive

clean-local:
	@rm configure config.status config.log
	@rm Makefile Makefile.in aclocal.m4
	@rm -rf autom4te.cache build-aux