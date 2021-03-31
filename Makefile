PROJECTNAME = VirtualMachine
CC = g++
CFLAGS = -std=c++11 -pthread -Wall -Wextra
APP_DIR = app
SRC_DIR = src
OBJ_DIR = obj
PRSOURCES = $(wildcard $(APP_DIR)/*.cpp) $(wildcard $(SRC_DIR)/*.cpp)
PROBJECTS = $(PRSOURCES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
PREXEC = $(PROJECTNAME).exe

.DEFAULT_GOAL := help
.PHONY: help

help:
	@grep -E '^[a-zA-Z0-9_-]+:.*?## .*$$' $(MAKEFILE_LIST) \
	| sed -n 's/^\(.*\): \(.*\)##\(.*\)/\1\3/p' \
	| column -t -s ' '

build: $(PREXEC) ## Build project

$(PREXEC): $(PROBJECTS)
	$(CC) $(PROBJECTS) -o $(PREXEC) $(CFLAGS)

# telling make how to turn file.cpp into file.o for an arbitary file
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CC) -c $^ -o $@ $(CFLAGS)

$(OBJ_DIR):
	mkdir $@

clean: ## Clean project
	@echo "Clean..."
	rm -rf $(OBJ_DIR)
	rm ./*.exe

run: ## Run project
	@echo "Running..."
	./$(PREXEC) $(ARGS)

TEST_CFLAGS = -std=c++11 -pthread -Wall -Wextra -lgtest
TEST_SRC_DIR = tests
TEST_SOURCE = $(wildcard $(TEST_SRC_DIR)/*.cpp) $(wildcard $(SRC_DIR)/*.cpp)
TEST_EXEC = $(PROJECTNAME)-TEST.exe
buildtests: ## Build tests
	@echo "Building tests..."
	$(CC) $(TEST_SOURCE) -o $(TEST_EXEC) $(TEST_CFLAGS) -D_VM_TEST_CONTROL

runtests: ## Run tests
	@echo "Running tests..."
	./$(TEST_EXEC)
