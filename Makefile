# this is a temporary makefile

CPPFLAGS=-O0 -g3 $(INC) -Wall

CPP=g++
INC=-I./includes/
LIB=lib/libcds.a

OBJ=src/basic/array.o

libcds: include_files $(OBJ)
	@ar rcs $(LIB) $(OBJECTS) 

%.o: %.cpp
	@echo " [C++] Compiling $<"
	@$(CPP) $(CPPFLAGS) $(INCL) -c $< -o $@

include_files:
	@find ./src/ -name *.h -exec ./copy_header.py {} \;

tests: libcds
	@echo " [LNK] Compiling and linking test_basic"
	@$(CPP) $(CPPFLAGS) -o test/test_basic test/test_basic.cpp -lgtest -lpthread $(LIB)	

clean:
	@echo " [CLN] Cleaning source tree"
	@rm -f $(OBJ)
	@rm $(LIB)
	@touch lib/delete_me
	@rm -rf includes/*
	@mkdir includes/libcds
	@touch includes/libcds/delete_me
	@rm -f test/test_basic
