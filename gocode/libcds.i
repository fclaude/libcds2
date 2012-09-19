%module libcds2
%{ 
    #include <libcds/immutable/mapper.h>
    #include <libcds/immutable/mappernone.h>
    #include <libcds/immutable/coder.h>
    #include <libcds/immutable/codernone.h>
    #include <libcds/immutable/bitsequence.h>
	#include <libcds/immutable/bitsequenceonelevelrank.h>
    #include <libcds/libcds.h>
    #include <libcds/array.h>
    #include <iostream>
    using namespace std;
%}
%include "includes/libcds/libcds.h"
%include "includes/libcds/immutable/bitsequence.h"
%include "includes/libcds/immutable/bitsequenceonelevelrank.h"
%include "includes/libcds/array.h"
%include "includes/libcds/immutable/mapper.h"
%include "includes/libcds/immutable/mappernone.h"
%include "includes/libcds/immutable/coder.h"
%include "includes/libcds/immutable/codernone.h"

