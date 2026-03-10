all:
	clang++ -o coins/coins coins/main.cc -std=c++17
	clang++ -o alien/alien alien/main.cc -std=c++17 


clean:
	rm -f coins/coins alien/alien
	rm -f coins/output.txt
