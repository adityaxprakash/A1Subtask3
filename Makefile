# Define the compiler and flags
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11

# Define the sources directory
SRCDIR = src

# Define the headers directory
HEADERDIR = headers

# Define the build directory
BUILDDIR = build

# List of source files
SOURCES := $(wildcard $(SRCDIR)/*.cpp)

# List of object files (generated from source files)
OBJECTS := $(patsubst $(SRCDIR)/%.cpp,$(BUILDDIR)/%.o,$(SOURCES))

# The name of the executable
EXECUTABLE = main

# Main target
all: $(EXECUTABLE) run clean

# Rule to build the executable
$(EXECUTABLE): $(OBJECTS)
	@$(CXX) $(CXXFLAGS) $^ -o $@

# Rule to compile each source file into an object file
$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp | $(BUILDDIR)
	@$(CXX) $(CXXFLAGS) -I $(HEADERDIR) -c $< -o $@

# Create the build directory if it doesn't exist
$(BUILDDIR):
	@mkdir -p $(BUILDDIR)

# Clean rule
clean:
	@rm -rf $(BUILDDIR) $(EXECUTABLE)

# Phony target to prevent conflicts with files named 'clean' or 'all'
.PHONY: clean all run

# Additional target to run the program with command-line arguments
run:
	@./$(EXECUTABLE) $(strategy) $(symbol) $(n) $(x) "$(start_date)" "$(end_date)" $(p) $(max_hold_days) $(c1) $(c2) $(oversold_threshold) $(overbought_threshold) $(adx_threshold) 

strategy ?= DEFAULT_STRATEGY
symbol ?= DEFAULT_SYMBOL
start_date ?= DEFAULT_START_DATE
end_date ?= DEFAULT_END_DATE
n ?= 0
x ?= 0
p ?= DEFAULT_P
max_hold_days ?= DEFAULT_MAX_HOLD_DAYS
c1 ?= DEFAULT_C1
c2 ?= DEFAULT_C2
oversold_threshold ?= DEFAULT_OVERSOLD_THRESHOLD
overbought_threshold ?= DEFAULT_OVERBOUGHT_THRESHOLD
adx_threshold ?= DEFAULT_ADX_THRESHOLD


