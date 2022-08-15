CFLAGS = -std=c++17 -O2
LDFLAGS = -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi
CC = src/main.cc src/app.cc
HH = src/app.hh src/vertex.hh
INCLUDE_DIRS = src/

vulkantest: $(CC) $(HH)
	g++ $(CFLAGS) -I$(INCLUDE_DIRS) -o vulkantest $(CC) $(LDFLAGS)

.PHONY: test clean

test: vulkantest
	./vulkantest

clean:
	rm -f vulkantest