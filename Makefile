# this is a temporary makefile

CPPFLAGS=-O0 -g3 $(INC) -Wall

CPP=g++
INC=-I./includes/ -I./dep/gtest-1.6.0/include/
LIB=lib/libcds.a

OBJ=src/basic/array.o src/basic/io.o src/immutable/bitsequence.o \
	src/immutable/bitsequenceseq.o 

TESTOBJ= tests/test_main.o tests/test_array.o tests/test_ioh.o tests/test_libcdsh.o \
		tests/test_timeh.o tests/test_bitsequence1.o tests/test_bitsequence_utils.o

GTEST_DIR=./dep/gtest-1.6.0/

libcds: cfg include_files $(OBJ)
	@rm -f $(LIB)
	@ar rs $(LIB) $(OBJ) 2> /dev/null

%.o: %.cpp
	@echo " [C++] Compiling $<"
	@$(CPP) $(CPPFLAGS) $(INCL) -c $< -o $@

cfg:
	@echo " [MSG] Compiler: $(CPP)"
	@echo " [MSG] Compiler flags: $(CPPFLAGS)"

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

test: libcds $(TESTOBJ)
	@echo " [DEP] Compiling gtest library"
	@cd $(GTEST_DIR); ./configure > /dev/null; cd ../..
	@make -s -C $(GTEST_DIR) > /dev/null
	@echo " [LNK] Compiling and linking test_array"
	@$(CPP) $(CPPFLAGS) -o tests/test_array tests/test_array.o tests/test_main.o -lpthread $(LIB) $(INC) $(GTEST_DIR)/src/gtest-all.o
	@echo " [LNK] Compiling and linking test_ioh"
	@$(CPP) $(CPPFLAGS) -o tests/test_ioh tests/test_ioh.o tests/test_main.o -lpthread $(LIB) $(INC) $(GTEST_DIR)/src/gtest-all.o
	@echo " [LNK] Compiling and linking test_libcdsh"
	@$(CPP) $(CPPFLAGS) -o tests/test_libcdsh tests/test_libcdsh.o tests/test_main.o -lpthread $(LIB) $(INC) $(GTEST_DIR)/src/gtest-all.o
	@echo " [LNK] Compiling and linking test_timeh"
	@$(CPP) $(CPPFLAGS) -o tests/test_timeh tests/test_timeh.o tests/test_main.o -lpthread $(LIB) $(INC) $(GTEST_DIR)/src/gtest-all.o
	@echo " [LNK] Compiling and linking test_bitsequence1"
	@$(CPP) $(CPPFLAGS) -o tests/test_bitsequence1 tests/test_bitsequence1.o tests/test_bitsequence_utils.o tests/test_main.o -lpthread $(LIB) $(INC) $(GTEST_DIR)/src/gtest-all.o

autotest: test
	@echo "Not implemented" 

clean:
	@echo " [CLN] Cleaning source tree"
	@find ./ -name *.orig -exec rm -f {} \;
	@rm -f $(OBJ)
	@rm -f $(LIB)
	@touch lib/delete_me
	@rm -rf includes/*
	@mkdir includes/libcds
	@touch includes/libcds/delete_me
	@make -s -C $(GTEST_DIR) clean > /dev/null
	@rm -f $(TESTOBJ)
	@rm -f tests/test_array tests/test_timeh tests/test_libcdsh tests/test_ioh

