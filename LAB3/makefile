CC=g++
LD=g++
CC_FLAGS= -std=c++11
OBJECTS= trystan_kaes_lab3.o lab3_implementation.o
OB= trystan_kaes_lab3
BINARY=myscheduler


all: $(OBJECTS) #docs
	$(LD) -o $(BINARY) $(OBJECTS)
clean:
	rm -rf $(OBJECTS) $(TEST) $(BINARY) $(OB).zip

%.o: %.cpp
	$(CC) -c $(CC_FLAGS) $< -o $@
