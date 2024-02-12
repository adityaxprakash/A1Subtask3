# Define the compiler and flags
CXX = g++
CXXFLAGS = -g -std=c++11 -fopenmp

# Define the sources directory
SRCDIR = src

# Define the headers directory
HEADERDIR = headers

# Define the build directory
BUILDDIR = build

DATADIR = data

# List of source files
SOURCES := $(wildcard $(SRCDIR)/*.cpp)

# List of object files (generated from source files)
OBJECTS := $(patsubst $(SRCDIR)/%.cpp,$(BUILDDIR)/%.o,$(SOURCES))

# The name of the executable
EXECUTABLE = main

# Main target
all: $(EXECUTABLE) run

# Rule to build the executable
$(EXECUTABLE): $(OBJECTS)
	@$(CXX) $(CXXFLAGS) $^ -o $@

# Rule to compile each source file into an object file
$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp compile_msg  | $(BUILDDIR) 
	@$(CXX) $(CXXFLAGS) -I $(HEADERDIR) -c $< -o $@

compile_msg:
	@echo "Compiling source code..."	

# Create the build directory if it doesn't exist
$(BUILDDIR):
	@mkdir -p $(BUILDDIR)
	

# Clean rule
clean:
	@rm -rf $(BUILDDIR)/* $(EXECUTABLE) $(DATADIR)/* $(wildcard *.csv) $(wildcard *.txt) $(.vscode)

	@echo "Cleaned build and data directories"


# Phony target to prevent conflicts with files named 'clean' or 'all'
.PHONY: clean all run compile_msg

# Additional target to run the program with command-line arguments
run:
	@echo "Running the predictor"
	@ ./$(EXECUTABLE) $(strategy) $(symbol) $(n) $(x) "$(start_date)" "$(end_date)" $(p) $(max_hold_days) $(c1) $(c2) $(oversold_threshold) $(overbought_threshold) $(adx_threshold) $(train_start_date) $(train_end_date) $(symbol1) $(symbol2) $(threshold) $(stop_loss_threshold)
	@echo "Results written to data directory"

strategy ?= DEFAULT_STRATEGY
symbol ?= DEFAULT_SYMBOL
start_date ?= DEFAULT_START_DATE
end_date ?= DEFAULT_END_DATE
n ?= 0
x ?= 0
p ?= 0
max_hold_days ?= 0
c1 ?= 0
c2 ?= 0
oversold_threshold ?= 0
overbought_threshold ?= 0
adx_threshold ?= 0
train_start_date ?= DEFAULT_TRAIN_START_DATE
train_end_date ?= DEFAULT_TRAIN_END_DATE
symbol1 ?= DEFAULT_SYMBOL1
symbol2 ?= DEFAULT_SYMBOL2
threshold ?= 0
stop_loss_threshold ?= -1

