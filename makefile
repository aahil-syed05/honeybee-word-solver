build:
	rm -f spellingBee.exe
	gcc -Wall -Wextra -std=c11 main.c -o spellingBee.exe

run_play: build
	./spellingBee.exe -p -r 7

run_sample: build
	echo "acdeit" > sampleIn.txt
	echo "e" >> sampleIn.txt
	./spellingBee.exe -d newsamp.txt < sampleIn.txt

clean:
	rm -f spellingBee.exe
	rm -f sampleIn.txt
