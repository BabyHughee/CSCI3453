CC=g++
LD=g++
CC_FLAGS= -std=c++11
OBJECTS= trystan_kaes_lab1.o
OB= trystan_kaes_lab1
BINARY=trystan_kaes_lab1

all: $(OBJECTS) #docs
	$(LD) -o $(BINARY) $(OBJECTS)
clean:
	rm -rf $(OBJECTS) $(TEST) $(BINARY) $(OB).zip

%.o: %.cpp
	$(CC) -c $(CC_FLAGS) $< -o $@
