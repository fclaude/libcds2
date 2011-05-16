# this is a temporary makefile

CPP=g++
INC=-I./includes/
CPPFLAGS=-O0 -g3 $(INC) -Wall

libcds: include_files 
	@echo "Done"

include_files:
	@find ./src/ -name *.h -exec cp {} includes/libcds \;


tests: libcds
	$(CPP) $(CPPFLAGS) -o test/test_basic test/test_basic.cpp -lgtest -lpthread	
