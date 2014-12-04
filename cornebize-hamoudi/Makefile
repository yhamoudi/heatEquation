EXEC=average

all : $(EXEC)

% : src/%.c
	mpicc $< -o $@

clean:
	rm -f $(EXEC) *~ */*~
