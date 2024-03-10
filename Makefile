## make file for skip.

# Project specific variables.
PRODUCT := skip
VER     := 0.0.1
WORKDIR := .

# Additional compile args.
ARGS    :=

# Project derived variables.  Do not manually change these. These should be dependent on the variables already set.


MANPATH=/usr/share/man

PKGFILE := $(WORKDIR)/$(PRODUCT)-$(VER).tar.gz
SRCFILE := $(WORKDIR)/$(PRODUCT)-src-$(VER).tar.gz


all: $(WORKDIR)/$(PRODUCT)
package: $(PKGFILE)
src: $(SRCFILE)



# Need to be able to make 'man-pages' as well.  Not sure where to get the source for those... 

$(WORKDIR)/$(PRODUCT): $(PRODUCT).c
	@echo "Making: $@"
	@-[ -d $(WORKDIR) ] || mkdir $(WORKDIR)
	gcc -o $@ $^ $(ARGS)

$(WORKDIR)/$(SRCFILE): $(PRODUCT).c Makefile
	@echo "Making: $@"
	@-[ ! -d $(WORKDIR) ] && mkdir $(WORKDIR)
	@-[ -e $@ ] && rm $@
	tar zcf $@ $^

$(WORKDIR)/$(PKGFILE): $(PRODUCT)
	@echo "Making: $@"
	@-[ ! -d $(WORKDIR) ] && mkdir $(WORKDIR)
	@-[ -e $@ ] && rm $@
	tar zcf $@ $^


install: $(WORKDIR)/$(PRODUCT)
	cp $^ /usr/bin/

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
	@-[ -e $(WORKDIR)/$(PRODUCT) ] && rm $(WORKDIR)/$(PRODUCT)
