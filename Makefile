#format is target-name: target dependencies
#{-tab-}actions

# All Targets
all: calc

# Tool invocations
# Executable "program" depends on the files program.o and run.o.
calc: main.o calc.o
	gcc -g -Wall -o calc main.o calc.o

# Depends on the source and header files
main.o: main.c
	gcc -g -Wall -c -o main.o main.c 
	
calc.o: calc.s
	nasm -g -f elf64 -w+all -o calc.o calc.s
#tell make that "clean" is not a file name!
.PHONY: clean

#Clean the build directory
clean: 
	rm -f *.o calc