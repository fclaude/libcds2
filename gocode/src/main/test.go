package main

import "fmt"
import "libcds2"

func check(b libcds2.BitSequenceOneLevelRank, a libcds2.Array, i, j int, done chan bool) {
	for k := i ; k < j; k++ {
		if b.Access(i) != (a.GetField(i) > 0) {
			fmt.Println("Error, values do not match (", b.Access(i), ", ", a.GetField(i), ")")
		}
	}
	done <- true
}

func main() {
	// libcds2.PrintBinary(35)
	N := 10000000
	K := 500000
	a := libcds2.ArrayCreate(N, libcds2.Msb(N+1))
	for j := 0; j < N; j++ {
		a.SetField(j, j+1)
	}
	for j := 0; j < N; j++ {
		if int(a.GetField(j)) != j+1 {
			fmt.Println("Error, values do not match")
		}
	}
	fmt.Println("Size for a: ", a.GetSize())
	
	a2 := libcds2.ArrayCreate(N, 1)
	for i := 0 ; i < N; i++ {
		if i%2 == 1 {
			a2.SetField(i, 1)
		}
	}
	fmt.Println("Bitmap of length ", N, " requires ", a2.GetSize(), " bytes")
	b := libcds2.NewBitSequenceOneLevelRank(a2, 20)
	d := make(chan bool)
	for i := 0; i < N/K; i++ {	
		go check(b, a2, i*K, (1+i)*K-1, d)
	}
	for i := 0; i < N/K; i++ {
		var done bool
		select {
			case done = <- d: fmt.Println(done)
		}
	}
	fmt.Println("We are done!")
}
