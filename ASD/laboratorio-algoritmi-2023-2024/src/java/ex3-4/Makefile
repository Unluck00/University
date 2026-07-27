JAVAC = javac
JAVA  = java
# Junit and options for compilation
JFLAGS = lib/junit-4.13.2.jar:lib/hamcrest-core-1.3.jar 	

# Directories
BIN = build
STRC = structures
EXCP = exceptions

# All Java sources (class + interface + test)
SOURCES = $(wildcard $(STRC)/*.java) $(wildcard $(EXCP)/*.java) $(wildcard tests/*.java)

# Targets: compile all
all:
	mkdir -p $(BIN)
	$(JAVAC) -d $(BIN) -cp $(BIN):$(JFLAGS) $(SOURCES)

# Rule for compile .java -> .class inside build
$(BIN)/%.class: %.java
# $(dir $@) extract only directory of the target
	@mkdir -p $(dir $@)
	$(JAVAC) -d $(BIN) -cp $(BIN):$(JFLAGS) $<

clean:
	rm -f -r $(BIN) *~

run: all
	$(JAVA) -cp $(BIN) structures.Prim italian_dist_graph.csv

# JUnit test
test: all
	$(JAVA) -cp $(BIN):$(JFLAGS) org.junit.runner.JUnitCore \
		tests.PriorityQueueTests \
		tests.GraphTests \
		tests.PrimTests