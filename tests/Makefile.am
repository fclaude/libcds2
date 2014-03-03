# This Makefile is used to build and execute the tests when "make check"
# is run.

AM_CXXFLAGS = $(PTHREAD_CFLAGS) -I../include/ -I../dep/gtest-1.6.0/include/
AM_LIBS = $(PTHREAD_LIBS)

check_PROGRAMS = test_array test_ioh test_libcdsh test_timeh test_bitsequence test_bitsequenceonelevelrank test_sequence test_mappernone test_codernone test_permutationmrrr test_treelouds test_memorymanager

LDADD = ../dep/gtest-1.6.0/lib/libgtest.la ../lib/libcds$(CDS_API_VERSION).la
test_array_SOURCES                   = test_array.cpp test_main.cpp
test_ioh_SOURCES                     = test_ioh.cpp test_main.cpp
test_libcdsh_SOURCES                 = test_libcdsh.cpp test_main.cpp
test_timeh_SOURCES                   = test_timeh.cpp test_main.cpp
test_bitsequence_SOURCES             = test_bitsequence.cpp \
				       test_bitsequence_utils.cpp test_main.cpp
test_bitsequenceonelevelrank_SOURCES = test_bitsequenceonelevelrank.cpp \
				       test_bitsequence_utils.cpp test_main.cpp
test_sequence_SOURCES                = test_sequence.cpp \
				       test_sequence_utils.cpp test_main.cpp
test_mappernone_SOURCES              = test_mappernone.cpp test_main.cpp
test_codernone_SOURCES               = test_codernone.cpp test_main.cpp
test_permutationmrrr_SOURCES         = test_permutationmrrr.cpp test_main.cpp
test_treelouds_SOURCES               = test_treelouds.cpp test_main.cpp
test_memorymanager_SOURCES           = test_memorymanager.cpp test_main.cpp

TESTS = $(check_PROGRAMS)
