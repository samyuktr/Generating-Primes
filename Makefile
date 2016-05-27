all: main.o primes.o trialdiv.o millerrabin.o rndsearch.o maurer.o
	gcc -g  main.o primes.o trialdiv.o millerrabin.o rndsearch.o maurer.o -L/home/scf-22/csci551b/openssl/lib -lcrypto -lm -o hw6

main.o: main.c hw6.h
	gcc -c -g -Wall main.c  -I/home/scf-22/csci551b/openssl/include

primes.o: primes.c hw6.h
	gcc -c -g -Wall primes.c -I/home/scf-22/csci551b/openssl/include

trialdiv.o: trialdiv.c hw6.h
	gcc -c -g -Wall trialdiv.c -I/home/scf-22/csci551b/openssl/include

millerrabin.o: millerrabin.c hw6.h
	gcc -c -g -Wall millerrabin.c -I/home/scf-22/csci551b/openssl/include

rndsearch.o: rndsearch.c hw6.h
	gcc -c -g -Wall rndsearch.c -I/home/scf-22/csci551b/openssl/include

maurer.o: maurer.c hw6.h
	gcc -c -g -Wall maurer.c -I/home/scf-22/csci551b/openssl/include


clean:
	rm *.o
	rm hw6
