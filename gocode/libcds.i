%module libcds2
%{ 
    #include <libcds2/immutable/mapper.h>
    #include <libcds2/immutable/mappernone.h>
    #include <libcds2/immutable/coder.h>
    #include <libcds2/immutable/codernone.h>
    #include <libcds2/immutable/bitsequence.h>
	#include <libcds2/immutable/bitsequenceonelevelrank.h>
    #include <libcds2/libcds.h>
    #include <libcds2/array.h>
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

