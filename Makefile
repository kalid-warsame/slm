CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic -O2

SRC_DIR   = src
BUILD_DIR = build

SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)
LIB_OBJS = $(filter-out $(BUILD_DIR)/main.o, $(OBJS))

.PHONY: all clean test

all: slm

slm: $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

$(BUILD_DIR):
	mkdir -p $@

test: test_model
	./test_model

test_model: tests/test_model.cpp $(LIB_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	rm -rf $(BUILD_DIR) slm test_model

-include $(OBJS:.o=.d)
