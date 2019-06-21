# in large copy pasted makefile
TARGET_EXEC ?= application.out

BUILD_DIR ?= ./build
SRC_DIRS ?= ./src

SRCS := $(shell find $(SRC_DIRS) -name *.cpp -or -name *.c -or -name *.s)
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

INC_DIRS := ./dependencies/GLFW/include/ ./dependencies/GLEW/include/ 
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

LIB_DIRS := ./dependencies/GLFW/lib-vc2017/ ./dependencies/GLEW/lib64/
LIB_FLAGS := $(addprefix -L,$(LIB_DIRS))

CPPFLAGS ?= $(INC_FLAGS) -MMD -MP -g -std=c++11
LDFLAGS = $(LIB_FLAGS)  
LDLIBS = -lGLEW -lGLU -lGL -lglfw
CC = g++
CXX = g++

$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS) $(LDLIBS)

# assembly
$(BUILD_DIR)/%.s.o: %.s
	$(MKDIR_P) $(dir $@)
	$(AS) $(ASFLAGS) -c $< -o $@

# c source
$(BUILD_DIR)/%.c.o: %.c
	$(MKDIR_P) $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

# c++ source
$(BUILD_DIR)/%.cpp.o: %.cpp
	$(MKDIR_P) $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@


.PHONY: clean

clean:
	$(RM) -r $(BUILD_DIR)

-include $(DEPS)

MKDIR_P ?= mkdir -p

run:
	./build/a.out

crun: $(BUILD_DIR)/$(TARGET_EXEC)
	./build/a.out

profile: CPPFLAGS += -pg
profile: LDFLAGS += -pg
profile: $(BUILD_DIR)/$(TARGET_EXEC)

stacktrace: CPPFLAGS += -v -da -Q
stacktrace: $(BUILD_DIR)/$(TARGET_EXEC)
