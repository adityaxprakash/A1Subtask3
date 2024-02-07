CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11

SRCDIR = src

BUILDDIR = build

SOURCES := $(wildcard $(SRCDIR)/*.cpp)

OBJECTS := $(patsubst $(SRCDIR)/%.cpp,$(BUILDDIR)/%.o,$(SOURCES))

EXECUTABLE = stock_trader

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp | $(BUILDDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILDDIR):
	mkdir -p $(BUILDDIR)

clean:
	rm -rf $(BUILDDIR) $(EXECUTABLE)

.PHONY: clean all

run:
	./$(EXECUTABLE) $(strategy) $(symbol) $(n) $(x) $(p) $(start_date) $(end_date) $(c1) $(c2) $(max_hold_days) $(oversold_threshold) $(overbought_threshold) $(adx_threshold)

n ?= DEFAULT_N
x ?= DEFAULT_X
p ?= DEFAULT_P
max_hold_days ?= DEFAULT_MAX_HOLD_DAYS
c1 ?= DEFAULT_C1
c2?= DEFAULT_C2
oversold_threshold ?= DEFAULT_OVERSOLD_THRESHOLD
overbought_threshold ?= DEFAULT_OVERBOUGHT_THRESHOLD
adx_threshold ?= DEFAULT_ADX_THRESHOLD


