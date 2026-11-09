CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Wpedantic -O2 -Iinclude

BIN_DIR = bin
TEST_DIR = tests

TESTS = test_vector test_linked_list test_stack test_queue test_bst test_avl
TEST_BINS = $(addprefix $(BIN_DIR)/, $(TESTS))

all: $(TEST_BINS)

$(BIN_DIR)/test_%: $(TEST_DIR)/test_%.cpp | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $< -o $@

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

test: $(TEST_BINS)
	@echo "========================================="
	@echo " Running Phase 1 Linear Container Tests"
	@echo "========================================="
	@for t in $(TEST_BINS); do ./$$t || exit 1; done
	@echo "All Phase 1 tests passed successfully!"

clean:
	rm -rf $(BIN_DIR) build

.PHONY: all test clean
