assembler: main.o macro.o assembler.o binary.o base64.o maybeLabel.o printToFile.o Label.o Free.o insert.o lineFix.o wordChecks.o
	gcc -g -ansi -Wall -pedantic main.o macro.o assembler.o binary.o base64.o maybeLabel.o printToFile.o Label.o Free.o insert.o lineFix.o wordChecks.o -o main

main.o: main.c
	gcc -c -g -ansi -Wall -pedantic main.c -o main.o

macro.o: macro.c
	gcc -c -g -ansi -Wall -pedantic macro.c -o macro.o

first.o: assembler.c
	gcc -c -g -ansi -Wall -pedantic assembler.c -o assembler.o

binary.o: binary.c
	gcc -c -g -ansi -Wall -pedantic binary.c -o binary.o

base64.o: base64.c
	gcc -c -g -ansi -Wall -pedantic base64.c -o base64.o

maybeLabel.o: maybeLabel.c
	gcc -c -g -ansi -Wall -pedantic maybeLabel.c -o maybeLabel.o

printToFile.o: printToFile.c
	gcc -c -g -ansi -Wall -pedantic printToFile.c -o printToFile.o

Label.o: Label.c
	gcc -c -g -ansi -Wall -pedantic Label.c -o Label.o

Free.o: Free.c
	gcc -c -g -ansi -Wall -pedantic Free.c -o Free.o

insert.o: insert.c
	gcc -c -g -ansi -Wall -pedantic insert.c -o insert.o

lineFix.o: lineFix.c
	gcc -c -g -ansi -Wall -pedantic lineFix.c -o lineFix.o

wordChecks.o: wordChecks.c
	gcc -c -g -ansi -Wall -pedantic wordChecks.c -o wordChecks.o

clean:
	rm -f main.o macro.o assembler.o binary.o base64.o maybeLabel.o printToFile.o Label.o Free.o insert.o lineFix.o wordChecks.o main
