all:
#	GOPATH=`pwd` go install main
	GOPATH=`pwd`:/home/rkonow/bin/go go build -compiler gccgo -gccgoflags '-O9 -fPIC' main

clean:
	rm -rf pkg/ bin/
	
