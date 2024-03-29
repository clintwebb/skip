## make file for skip.

# Project specific variables.
PRODUCT := skip
VER     := 0.0.4

# Additional compile args.
ARGS    :=

# Project derived variables.  Do not manually change these. These should be dependent on the variables already set.


MANPATH=/usr/share/man

PKGFILE := $(PRODUCT)-$(VER).tar.gz
SRCFILE := $(PRODUCT)-src-$(VER).tar.gz


all: $(PRODUCT)
package: $(PKGFILE)
src: $(SRCFILE)



# Need to be able to make 'man-pages' as well.  Not sure where to get the source for those... 

$(PRODUCT): $(PRODUCT).c
	@echo "Making: $@"
	gcc -o $@ $^ $(ARGS)

$(SRCFILE): $(PRODUCT).c Makefile
	@echo "Making: $@"
	@-[ -e $@ ] && rm $@
	tar zcf $@ $^

$(PKGFILE): $(PRODUCT)
	@echo "Making: $@"
	@-[ -e $@ ] && rm $@
	tar zcf $@ $^


install: $(PRODUCT)
	install -D -m 755 $^ $(DESTDIR)$(PREFIX)/usr/bin/$^
#	chmod a+rx $(DESTDIR)/usr/bin/skip

#
#
# man-pages: manpages/librisp.3 manpages/risp_addbuf.3 manpages/risp_addbuf_noparam.3 manpages/risp_add_command.3 manpages/risp_init.3 manpages/risp_shutdown.3 manpages/risp.3 manpages/risp_addbuf_int.3 manpages/risp_addbuf_str.3 manpages/risp_add_invalid.3 manpages/risp_process.3 manpages/risp_needs.3
# 	@mkdir tmp.install
# 	@cp manpages/* tmp.install/
# 	@gzip tmp.install/*.3
# 	cp tmp.install/*.3.gz $(MANPATH)/man3/
# 	@rm tmp.install/*
# 	@rmdir tmp.install
# 	@echo "Man-pages Install complete."
#

uninstall:
	@-test -e /usr/bin/$(PRODUCT) && rm /usr/bin/$(PRODUCT)

clean:
	@-[ -e $(PRODUCT) ] && rm $(PRODUCT)
