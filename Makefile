# this is a temporary makefile

CPPFLAGS=-O0 -g3 $(INC) -Wall

CPP=g++
INC=-I./includes/
LIB=lib/libcds.a

OBJ=src/basic/array.o src/basic/io.o src/immutable/bitsequence.o

GTEST_DIR=dep/gtest-1.6.0

libcds: include_files $(OBJ)
	@rm -f $(LIB)
	@ar rs $(LIB) $(OBJ) 2> /dev/null

%.o: %.cpp
	@echo " [C++] Compiling $<"
	@$(CPP) $(CPPFLAGS) $(INCL) -c $< -o $@

indent:
	@find ./src/ -name *.h -exec python ./config/indent.py {} \;
	@find ./tests/ -name *.h -exec python ./config/indent.py {} \;
	@find ./src/ -name *.cpp -exec python ./config/indent.py {} \;
	@find ./tests/ -name *.cpp -exec python ./config/indent.py {} \;
	

cpplint:
	@find ./src/ -name *.h -exec python ./config/cpplint.py --filter=-runtime/sizeof,-whitespace/line_length,-runtime/references,-readability/streams,-runtime/int {} \;
	@find ./src/ -name *.cpp -exec python ./config/cpplint.py --filter=-runtime/sizeof,-whitespace/line_length,-runtime/references,-readability/streams,-runtime/int {} \;
	@find ./tests/ -name *.h -exec python ./config/cpplint.py --filter=-runtime/sizeof,-whitespace/line_length,-runtime/references,-readability/streams,-runtime/int {} \;
	@find ./tests/ -name *.cpp -exec python ./config/cpplint.py --filter=-runtime/sizeof,-whitespace/line_length,-runtime/references,-readability/streams,-runtime/int {} \;
	
include_files:
	@find ./src/ -name *.h -exec python ./config/copy_header.py {} \;

test: libcds
	@echo " [DEP] Compiling gtest library"
	@cd $(GTEST_DIR); ./configure > /dev/null; cd ../..
	@make -s -C $(GTEST_DIR) > /dev/null
	@echo " [LNK] Compiling and linking test_basic"
	@$(CPP) $(CPPFLAGS) -o tests/test_basic tests/test_basic.cpp -lpthread $(LIB)	-I./$(GTEST_DIR)/include/ $(GTEST_DIR)/src/gtest-all.o

clean:
	@echo " [CLN] Cleaning source tree"
	@find ./ -name *.orig -exec rm -f {} \;
	@rm -f $(OBJ)
	@rm -f $(LIB)
	@touch lib/delete_me
	@rm -rf includes/*
	@mkdir includes/libcds
	@touch includes/libcds/delete_me
	@make -s -C gtest clean > /dev/null
	@rm -f tests/test_basic

