# This Makefile builds the libcds library.

AM_CPPFLAGS = -I../include/

lib_LTLIBRARIES = libcds@CDS_API_VERSION@.la
libcds@CDS_API_VERSION@_la_LDFLAGS = -version-info $(CDS_SO_VERSION)
libcds@CDS_API_VERSION@_la_SOURCES = \
    ../src/basic/array.cpp \
    ../src/basic/io.cpp \
    ../src/immutable/bitsequence.cpp \
    ../src/immutable/bitsequenceseq.cpp \
    ../src/immutable/bitsequenceonelevelrank.cpp \
    ../src/immutable/sequence.cpp \
    ../src/immutable/mapper.cpp \
    ../src/immutable/mappernone.cpp \
    ../src/immutable/coder.cpp \
    ../src/immutable/codernone.cpp \
    ../src/immutable/permutation.cpp \
    ../src/immutable/permutationmrrr.cpp \
    ../src/immutable/treelouds.cpp \
    ../src/immutable/wavelettreenoptrs.cpp \
    ../src/immutable/tree.cpp
