EXEC=average
FLAGS=-Wextra -Wall

all : setup $(EXEC)

setup:
	mkdir -p obj

obj/%.o : src/%.c
	mpicc -c $< -o $@ $(FLAGS)

average : obj/data.o obj/average.o
	mpicc -lm $^ -o $@

clean:
	rm -rf obj $(EXEC) *~ */*~
