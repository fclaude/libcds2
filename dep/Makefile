GTEST_DIR = gtest-1.6.0

.PHONY: all
all:

# All targets that might lead to make check.
.PHONY: check
check: gtest-stamp

.PHONY: installcheck
installcheck: gtest-stamp

gtest-stamp:
	@$(SHELL) build-gtest

# Support "clean" targets for automake and cmake Makefiles...
.PHONY: mostlyclean
mostlyclean:
	@cd $(GTEST_DIR) && $(MAKE) mostlyclean || $(MAKE) clean
	@rm gtest-stamp

.PHONY: clean
clean:
	@cd $(GTEST_DIR) && $(MAKE) clean
	@rm gtest-stamp

.PHONY: distclean
distclean:
	@cd $(GTEST_DIR) && $(MAKE) distclean || $(MAKE) clean
	@rm gtest-stamp

.PHONY: maintainer-clean
maintainer-clean:
	@cd $(GTEST_DIR) && $(MAKE) maintainer-clean || $(MAKE) clean
	@rm gtest-stamp


EMPTY_AUTOMAKE_TARGETS = dvi pdf ps info html tags ctags distdir install install-data install-exec uninstall install-dvi install-html install-info install-ps install-pdf installdirs
.PHONY: $(EMPTY_AUTOMAKE_TARGETS)
$(EMPTY_AUTOMAKE_TARGETS):
