EXEC=average
FLAGS=-Wextra -Wall

all : $(EXEC)

% : src/%.c
	mpicc -lm $< -o $@ $(FLAGS)

clean:
	rm -f $(EXEC) *~ */*~
