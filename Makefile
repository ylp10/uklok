all:
	clang++ -o coins/coins coins/main.cc
	clang++ -o alien/alien alien/main.cc

alien/alien: alien/main.cc
	clang++ -o alien/alien alien/main.cc

.PHONY: alien
alien: alien/alien

clean:
	rm -f coins/coins alien/alien
	rm -f coins/output.txt
