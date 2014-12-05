EXEC=average
FLAGS=-Wextra -Wall

all : $(EXEC)
	mkdir -p obj

obj/%.o : src/%.c
	mpicc -c $< -o $@ $(FLAGS)

average : obj/average.o obj/data.o
	mpicc -lm $^ -o $@ $(FLAGS)

clean:
	rm -f $(EXEC) *~ */*~
