CC = gcc
CFLAGS = -g -O2 -Wall -Wextra -Wpedantic -Wconversion

# Directories
BIN = bin
OBJ = obj
$(info Creation folders $(BIN) $(OBJ)) 		#print on the screen the message
$(shell mkdir -p $(BIN) $(OBJ)) 	#create folders if doesn't exist

# Unity tests
DTEST = test

TESTDIST_SRCS = $(wildcard $(DTEST)/edit_distance_test.c)
TESTDIST_OBJS = $(patsubst $(DTEST)/%.c, $(OBJ)/%.o, $(TESTDIST_SRCS))

TESTSORT_SRCS = $(wildcard $(DTEST)/sorting_test.c)
TESTSORT_OBJS = $(patsubst $(DTEST)/%.c, $(OBJ)/%.o, $(TESTSORT_SRCS))

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

# Targets
all: main_ex1 test_ex1 main_ex2 test_ex2

main_ex1: $(BIN)/main_ex1
$(BIN)/main_ex1: $(EX1_OBJS)
	$(CC) $(CFLAGS) -o $@ $^

test_ex1: $(BIN)/test_ex1
$(BIN)/test_ex1: $(UNITY_OBJS) $(filter-out $(OBJ)/main_ex1.o, $(EX1_OBJS)) $(TESTSORT_OBJS)
	$(CC) $(CFLAGS) -o $@ $^

main_ex2: $(BIN)/main_ex2
$(BIN)/main_ex2: $(EX2_OBJS)
	$(CC) $(CFLAGS) -o $@ $^

test_ex2: $(BIN)/test_ex2
$(BIN)/test_ex2: $(UNITY_OBJS) $(filter-out $(OBJ)/main_ex2.o, $(EX2_OBJS)) $(TESTDIST_OBJS)
	$(CC) $(CFLAGS) -o $@ $^

$(OBJ)/%.o : $(EX1)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ)/%.o : $(EX2)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ)/%.o : $(DTEST)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ)/%.o : $(UNITY)/%.c $(UNITY_HDRS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f -r $(BIN) $(OBJ) *~
