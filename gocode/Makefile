all:
	GOPATH=`pwd` go install -compiler gccgo -gccgoflags '-fPIC -lcds2 -shared -L/usr/lib/gcc/x86_64-linux-gnu/4.6/' main
	#GOPATH=`pwd` go install main

clean:
	rm -rf pkg/ bin/
	
