CXX = g++
CXXFLAGS = -Wall -Wextra -g3 -std=c++17

LIBS = -lraylib -lopengl32 -lgdi32 -lwinmm

SRC_DIR = .
OBJ_DIR = obj
OUT_DIR = output

SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

OUT = $(OUT_DIR)/main.exe

all: $(OUT)

$(OUT_DIR):
	mkdir -p $(OUT_DIR)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(OUT): $(OBJS) | $(OUT_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(OUT)
	./$(OUT)

clean:
	del /q $(OBJ_DIR)\*.o $(OUT) 2>nul || true
