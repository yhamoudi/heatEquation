EXEC=average test
FLAGS=-Wextra -Wall -g

all : setup $(EXEC)

setup:
	mkdir -p obj

obj/%.o : src/%.c
	mpicc -c $< -o $@ $(FLAGS)

average : obj/automata.o obj/process.o obj/average.o
	mpicc -lm $^ -o $@

test : obj/automata.o obj/process.o obj/test.o
	mpicc -lm $^ -o $@

clean:
	rm -rf obj $(EXEC) *~ */*~ average.log.* *.out
