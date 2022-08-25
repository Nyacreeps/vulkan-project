CFLAGS = -std=c++17 -O2
LDFLAGS = -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi
CC = src/main.cc src/app.cc src/app_window.cc src/app_debug.cc \
	 src/app_presentation.cc src/app_graphics_pipeline.cc src/app_draw.cc \
	 src/app_buffers.cc src/app_descriptors.cc src/app_depth_buffering.cc \
	 src/app_image.cc
HH = src/app.hh src/vertex.hh src/scene.hh
INCLUDE_DIRS = src/

vulkantest: $(CC) $(HH)
	g++ $(CFLAGS) -I$(INCLUDE_DIRS) -o vulkantest $(CC) $(LDFLAGS)

.PHONY: test clean

test: vulkantest
	./vulkantest

clean:
	rm -f vulkantest