CXX = g++
CXXFLAGS = -g -std=c++11 -fopenmp

SRCDIR = src

HEADERDIR = headers

BUILDDIR = build

DATADIR = data

SOURCES := $(wildcard $(SRCDIR)/*.cpp)

OBJECTS := $(patsubst $(SRCDIR)/%.cpp,$(BUILDDIR)/%.o,$(SOURCES))

EXECUTABLE = main

all: $(EXECUTABLE) run

$(EXECUTABLE): $(OBJECTS)
	@$(CXX) $(CXXFLAGS) $^ -o $@

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp compile_msg| $(BUILDDIR) 
	@echo "Compiling $<"
	@$(CXX) $(CXXFLAGS) -I $(HEADERDIR) -c $< -o $@

$(BUILDDIR):
	@mkdir -p $(BUILDDIR)
	
compile_msg:
	@echo "Compiling source files"
	
clean:
	@rm -rf $(BUILDDIR)/* $(EXECUTABLE) $(DATADIR)/* $(wildcard *.csv) $(wildcard *.txt) $(.vscode)
	@echo "Cleaned build and data directories"

.PHONY: clean all run compile_msg

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

