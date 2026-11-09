CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Wpedantic -O2 -Iinclude

BIN_DIR = bin
TEST_DIR = tests
BENCH_DIR = benchmarks
EX_DIR = examples

TESTS = test_vector test_linked_list test_stack test_queue test_bst test_avl test_heap test_graph test_sorting
TEST_BINS = $(addprefix $(BIN_DIR)/, $(TESTS))

BENCHS = benchmark_linear benchmark_trees benchmark_sorting
BENCH_BINS = $(addprefix $(BIN_DIR)/, $(BENCHS))

EXAMPLES = example_vector example_avl example_graph
EXAMPLE_BINS = $(addprefix $(BIN_DIR)/, $(EXAMPLES))

DEMO_BIN = $(BIN_DIR)/dsa_demo

all: $(DEMO_BIN) $(TEST_BINS) $(BENCH_BINS) $(EXAMPLE_BINS)

$(DEMO_BIN): src/main.cpp | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $< -o $@

$(BIN_DIR)/test_%: $(TEST_DIR)/test_%.cpp | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $< -o $@

$(BIN_DIR)/benchmark_%: $(BENCH_DIR)/benchmark_%.cpp | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $< -o $@

$(BIN_DIR)/example_%: $(EX_DIR)/%_example.cpp | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $< -o $@

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

test: $(TEST_BINS)
	@echo "========================================="
	@echo " Running Complete Unit Test Suite"
	@echo "========================================="
	@for t in $(TEST_BINS); do ./$$t || exit 1; done
	@echo "All tests passed successfully!"

benchmark: $(BENCH_BINS)
	@echo "========================================="
	@echo " Running Empirical Benchmarks"
	@echo "========================================="
	@for b in $(BENCH_BINS); do ./$$b; done

clean:
	rm -rf $(BIN_DIR) build

.PHONY: all test benchmark clean
