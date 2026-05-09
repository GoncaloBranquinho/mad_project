CXX = g++
CFLAGS =-std=c++23 

INCLUDES := $(shell find . -name '*.cpp')
OBJ := $(patsubst %.cpp,%.o,$(INCLUDES))
TEST := *.md
DEBUG = -g -Wall -Wextra -pedantic
OUT = ./bin/main
CORES = ./cores/core.*

all: build
cr : clean run

build: $(OBJ)
	@mkdir -p ./bin
	@echo "Linking..."
	$(CXX) $(CFLAGS) $(OBJ) -o $(OUT) 
	

%.o: %.cpp 
	@echo "Compiling..."
	$(CXX) $(CFLAGS) -c $< -o $@  	
	@echo "Compiled:" $@

run: $(OBJ)
	@mkdir -p ./bin
	@echo "Linking..."
	$(CXX) $(CFLAGS) $(OBJ) -o $(OUT) 
	@$(OUT)

debug: 
	@echo "Compiling..."
	$(CXX) $(DEBUG) $(CFLAGS) $(INCLUDES) -o $(OUT)

dr:
	@echo "Compiling..."
	$(CXX) $(DEBUG) $(CFLAGS) $(INCLUDES) -o $(OUT)
	@$(OUT)

clean:
	@echo "Deleting..."
	@rm -f $(OBJ) $(OUT) $(TEST) $(CORES)
