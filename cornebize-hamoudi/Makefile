EXEC=average test
FLAGS=-Wextra -Wall -g

all : setup $(EXEC)

setup:
	mkdir -p obj

obj/%.o : src/%.c
	mpicc -c $< -o $@ $(FLAGS)

average : obj/automata.o obj/process.o obj/average.o
	mpicc $^ -o $@ -lm

test : obj/automata.o obj/process.o obj/test.o
	mpicc $^ -o $@ -lm

clean:
	rm -rf obj $(EXEC) *~ */*~ average.log.* *.out
