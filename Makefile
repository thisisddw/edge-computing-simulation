TARGET_EXEC := simulator

BUILD_DIR := ./build
SRC_DIR := ./src
INCLUDE_DIR := ./include

SRCS := $(wildcard $(SRC_DIR)/*.cpp)
SRCS := $(SRCS:$(SRC_DIR)/%=%)
OBJS := $(SRCS:%.cpp=$(BUILD_DIR)/%.o)

CXX := g++
CPPFLAGS += -I$(INCLUDE_DIR) -I "C:\Program Files\Python38\include" -I "C:\Program Files\Python38\Lib\site-packages\numpy\core\include"
CXXFLAGS += -std=c++17 -g -Wall# -O2
LDFLAGS += -L "C:\Program Files\Python38\libs" -lpython38

$(BUILD_DIR)/$(TARGET_EXEC):$(OBJS)
	@$(CXX) $(OBJS) $(LDFLAGS) -o $@

$(BUILD_DIR)/%.o:$(SRC_DIR)/%.cpp
	@$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $<

run: $(BUILD_DIR)/$(TARGET_EXEC)
	@$(BUILD_DIR)/$(TARGET_EXEC)

clean:
	-@rm $(BUILD_DIR)/*.o $(BUILD_DIR)/$(TARGET_EXEC)

.PHONY: run clean
