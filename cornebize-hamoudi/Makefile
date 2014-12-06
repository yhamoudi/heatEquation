EXEC=average test
FLAGS=-Wextra -Wall -g

all : setup $(EXEC)

setup:
	mkdir -p obj

obj/%.o : src/%.c
	mpicc -c $< -o $@ $(FLAGS)

average : obj/data.o obj/average.o
	mpicc -lm $^ -o $@

test : obj/data.o obj/test.o
	mpicc -lm $^ -o $@

clean:
	rm -rf obj $(EXEC) *~ */*~ average.log.*
