SUBDIRS := $(wildcard */.)

all: $(SUBDIRS)

$(SUBDIRS):
	@$(MAKE) --no-print-directory -C $@

.PHONY: all $(SUBDIRS)
