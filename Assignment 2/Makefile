EXE_NAME 				= myProg.out
TEST_EXE_NAME			= test.out
SRC_DIR 				= src
HEADERS_DIR 			= .
BIN_DIR 				= bin
TEST_DIR 				= test

OBJ_DIR 				= $(BIN_DIR)/obj
SRC_FILES 				= $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES 				= $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))
OBJ_FILES_NO_MAIN		= $(filter-out $(OBJ_DIR)/main.o, $(OBJ_FILES))
CPPFLAGS 				=
CXXFLAGS 				= 

OBJ_TEST_DIR			= $(BIN_DIR)/obj-test
TEST_FILES 				= $(wildcard $(TEST_DIR)/*.cpp)
OBJ_TEST_FILES			= $(patsubst $(TEST_DIR)/%.cpp,$(OBJ_TEST_DIR)/%.o,$(TEST_FILES))
TEST_CXXFLAGS 			= -Wall -Wextra -Werror -O3 -std=c++17 -pthread

GTEST_INCLUDE 			= googletest/googletest/include
GMOCK_INCLUDE	 		= googletest/googlemock/include
GBENCHMARK_INCLUDE 		= benchmark/include/
GTEST_LIBS 				= googletest/build/lib/libgtest.a googletest/build/lib/libgtest_main.a
GBENCHMARK_LIBS 		= benchmark/build/src/libbenchmark.a benchmark/build/src/libbenchmark_main.a 

$(EXE_NAME): $(OBJ_FILES) | $(OBJ_DIR)
	g++ -o $(BIN_DIR)/$@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	g++ $(CPPFLAGS) $(CXXFLAGS) -I"$(HEADERS_DIR)" -c -o $@ $<

$(TEST_EXE_NAME): $(OBJ_TEST_FILES) $(OBJ_FILES_NO_MAIN) | $(OBJ_TEST_DIR)
	g++ $(TEST_CXXFLAGS) -o $(BIN_DIR)/$@ $^ $(GTEST_LIBS) $(GBENCHMARK_LIBS)

$(OBJ_TEST_DIR)/%.o: $(TEST_DIR)/%.cpp | $(OBJ_TEST_DIR)
	g++ $(TEST_CXXFLAGS) -I"$(HEADERS_DIR)" -I"$(GTEST_INCLUDE)" -I"$(GMOCK_INCLUDE)" -I"$(GBENCHMARK_INCLUDE)" -c -o $@ $<
	
$(OBJ_DIR):
	mkdir -p $@

$(OBJ_TEST_DIR):
	mkdir -p $@

docs:
	doxygen Doxyfile

.PHONY: 
	clean

clean:
	rm -rf bin

CXXFLAGS += -MMD
-include $(OBJ_FILES:.o=.d)