default: program

program: grimshaa.adventure grimshaa.buildrooms
	gcc -pthread -o grimshaa.adventure grimshaa.adventure.c -std=c89
	gcc -o grimshaa.buildrooms grimshaa.buildrooms.c -std=c89

clean:
	-rm -f grimshaa.adventure
	-rm -f grimshaa.buildrooms
	-rm -rf grimshaa.rooms.*
