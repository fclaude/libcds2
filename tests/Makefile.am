AM_CPPFLAGS = -I../include/ -I../dep/gtest-1.6.0/include/
LIBS = "$PTHREAD_LIBS -lcds"
TEST_SRC = test_main.cpp ../dep/gtest-1.6.0/src/gtest-all.cc

bin_PROGRAMS = test_ioh test_libcdsh test_timeh test_bitsequence test_bitsequenceonelevelrank test_sequence test_mappernone test_codernone test_permutationmrrr test_treelouds test_memorymanager

test_main.o: test_main.cpp
	$(CXXCOMPILE) test_main.cpp

LDADD = test_main.o ../dep/gtest-1.6.0/src/gtest-all.o
test_array_SOURCES                   = test_array.cpp
test_ioh_SOURCES                     = test_ioh.cpp
test_libcdsh_SOURCES                 = test_libcdsh.cpp
test_timeh_SOURCES                   = test_timeh.cpp
test_bitsequence_SOURCES             = test_bitsequence.cpp
test_bitsequenceonelevelrank_SOURCES = test_bitsequenceonelevelrank.cpp
test_sequence_SOURCES                = test_sequence.cpp
test_mappernone_SOURCES              = test_mappernone.cpp
test_codernone_SOURCES               = test_codernone.cpp
test_permutationmrrr_SOURCES         = test_permutationmrrr.cpp
test_treelouds_SOURCES               = test_treelouds.cpp
test_memorymanager_SOURCES           = test_memorymanager.cpp
