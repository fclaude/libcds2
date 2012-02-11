# this is a temporary makefile

CPPFLAGS=-O0 -g3 $(INC) -Wall

CPP=g++
INC=-I./includes/
LIB=lib/libcds.a

OBJ=src/basic/array.o

libcds: include_files $(OBJ)
	@rm -f $(LIB)
	@ar rs $(LIB) $(OBJ) 2> /dev/null

%.o: %.cpp
	@echo " [C++] Compiling $<"
	@$(CPP) $(CPPFLAGS) $(INCL) -c $< -o $@

include_files:
	@find ./src/ -name *.h -exec ./copy_header.py {} \;

test: libcds
	@make -s -C gtest > /dev/null
	@echo " [LNK] Compiling and linking test_basic"
	@$(CPP) $(CPPFLAGS) -o tests/test_basic tests/test_basic.cpp -lpthread $(LIB)	-I./gtest/include/ gtest/src/gtest-all.o

clean:
	@echo " [CLN] Cleaning source tree"
	@rm -f $(OBJ)
	@rm $(LIB)
	@touch lib/delete_me
	@rm -rf includes/*
	@mkdir includes/libcds
	@touch includes/libcds/delete_me
	@make -s -C gtest clean > /dev/null
	@rm -f tests/test_basic

