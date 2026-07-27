CC = gcc
CFLAGS = -g -O2 -Wall -Wextra -Wpedantic -Wconversion

# Directories
BIN = bin
OBJ = obj
$(info Creation folders $(BIN) $(OBJ)) 		#print on the screen the message
$(shell mkdir -p $(BIN) $(OBJ)) 	#create folders if doesn't exist

# Unity tests
DTEST = test

TESTSORT_SRCS = $(wildcard $(DTEST)/test_ex1.c)
TESTSORT_OBJS = $(patsubst $(DTEST)/%.c, $(OBJ)/%.o, $(TESTSORT_SRCS))

TESTHASH_SRCS = $(wildcard $(DTEST)/hash_table_test.c)
TESTHASH_OBJS = $(patsubst $(DTEST)/%.c, $(OBJ)/%.o, $(TESTHASH_SRCS))

TESTPQ_SRCS = $(wildcard $(DTEST)/test_ex3.c)
TESTPQ_OBJS = $(patsubst $(DTEST)/%.c, $(OBJ)/%.o, $(TESTPQ_SRCS))

TESTGRAPH_SRCS = $(wildcard $(DTEST)/graph_bfs_test.c)
TESTGRAPH_OBJS = $(patsubst $(DTEST)/%.c, $(OBJ)/%.o, $(TESTGRAPH_SRCS))

UNITY = $(DTEST)/unity
UNITY_SRCS = $(wildcard $(UNITY)/*.c)
UNITY_OBJS = $(patsubst $(UNITY)/%.c, $(OBJ)/%.o, $(UNITY_SRCS))
UNITY_HDRS = $(wildcard $(UNITY)/*.h)

# Ex1
EX1 = ex1
EX1_SRCS = $(wildcard $(EX1)/*.c)
EX1_OBJS = $(patsubst $(EX1)/%.c, $(OBJ)/%.o, $(EX1_SRCS))
EX1_HDRS = $(wildcard $(EX1)/headers/*.h)

# Ex2
EX2 = ex2
EX2_SRCS = $(wildcard $(EX2)/*.c)
EX2_OBJS = $(patsubst $(EX2)/%.c, $(OBJ)/%.o, $(EX2_SRCS))
EX2_HDRS = $(wildcard $(EX2)/headers/*.h)

# Ex3
EX3 = ex3
EX3_SRCS = $(wildcard $(EX3)/*.c)
EX3_OBJS = $(patsubst $(EX3)/%.c, $(OBJ)/%.o, $(EX3_SRCS))
EX3_HDRS = $(wildcard $(EX3)/headers/*.h)

# Ex4
EX4 = ex4
EX4_SRCS = $(wildcard $(EX4)/*.c)
EX4_OBJS = $(patsubst $(EX4)/%.c, $(OBJ)/%.o, $(EX4_SRCS))
EX4_HDRS = $(wildcard $(EX2)/headers/*.h)

# Targets
all: main_ex1 test_ex1 main_ex2 test_ex2 main_ex3 test_ex3 main_ex4 test_ex4

main_ex1: $(BIN)/main_ex1
$(BIN)/main_ex1: $(EX1_OBJS)
	$(CC) $(CFLAGS) -o $@ $^

test_ex1: $(BIN)/test_ex1
$(BIN)/test_ex1: $(filter-out $(OBJ)/main_ex1.o, $(EX1_OBJS)) $(TESTSORT_OBJS)
	$(CC) $(CFLAGS) -o $@ $^

main_ex2: $(BIN)/main_ex2
$(BIN)/main_ex2: $(EX2_OBJS)
	$(CC) $(CFLAGS) -o $@ $^

test_ex2: $(BIN)/test_ex2
$(BIN)/test_ex2: $(UNITY_OBJS) $(filter-out $(OBJ)/main_ex2.o, $(EX2_OBJS)) $(TESTHASH_OBJS)
	$(CC) $(CFLAGS) -o $@ $^

main_ex3: $(BIN)/main_ex3 
$(BIN)/main_ex3: $(EX3_OBJS) $(OBJ)/hash_table.o
	$(CC) $(CFLAGS) -o $@ $^

test_ex3: $(BIN)/test_ex3
$(BIN)/test_ex3:$(filter-out $(OBJ)/main_ex3.o, $(EX3_OBJS)) $(TESTPQ_OBJS) $(OBJ)/hash_table.o
	$(CC) $(CFLAGS) -o $@ $^

main_ex4: $(BIN)/main_ex4
$(BIN)/main_ex4: $(EX4_OBJS) $(OBJ)/hash_table.o
	$(CC) $(CFLAGS) -o $@ $^

test_ex4: $(BIN)/test_ex4
$(BIN)/test_ex4: $(UNITY_OBJS) $(filter-out $(OBJ)/main_ex4.o, $(EX4_OBJS)) $(TESTGRAPH_OBJS) $(OBJ)/hash_table.o
	$(CC) $(CFLAGS) -o $@ $^

$(OBJ)/%.o : $(EX1)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ)/%.o : $(EX2)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ)/%.o : $(EX3)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ)/%.o : $(EX4)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ)/%.o : $(DTEST)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ)/%.o : $(UNITY)/%.c $(UNITY_HDRS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f -r $(BIN) $(OBJ) *.bin *.log *~

# Valgrind per main_ex1
valgrind_ex1: main_ex1
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes \
		--verbose --log-file=valgrind_main_ex1.log ./$(BIN)/main_ex1 $(ARGS)
	@echo "Report Valgrind salvato in valgrind_main_ex1.log"

# Valgrind per test_ex4
valgrind_ex4: test_ex4
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes \
		--verbose --log-file=valgrind_test_ex4.log ./$(BIN)/test_ex4
	@echo "Report Valgrind salvato in valgrind_test_ex4.log"