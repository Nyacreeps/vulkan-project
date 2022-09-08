MKDIR   := mkdir
RMDIR   := rm -r
INCLUDE := src
BIN     := .
OBJ     := obj
SRC     := src
CC      := g++
CFLAGS  := -std=c++17 -O2 -I$(INCLUDE)
LDLIBS  := -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi
SRCS    := $(wildcard $(SRC)/*.cc)
OBJS    := $(patsubst $(SRC)/%.cc,$(OBJ)/%.o,$(SRCS))
EXE     := $(BIN)/vulkantest.exe

.PHONY: all run clean

all: $(EXE)

$(EXE): $(OBJS) | $(BIN)
	$(CC) $(LDFLAGS) $^ -o $@ $(LDLIBS)

$(OBJ)/%.o: $(SRC)/%.cc | $(OBJ)
	$(CC) $(CFLAGS) -c $< -o $@

$(BIN) $(OBJ):
	$(MKDIR) $@

run: $(EXE)
	./$<

clean:
	$(RMDIR) $(OBJ)