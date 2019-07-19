TARGET_EXEC := NewEngine

BUILD_DIR ?= bin
SRC_DIRS ?= src

LIB_DIR := C:/ProgrammingLibraries

CC	:= "C:/MinGW-w64/bin/g++"
CXX := "C:/MinGW-w64/bin/g++"

rwildcard=$(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2) $(filter $(subst *,%,$2),$d))

SRCS := $(call rwildcard, $(SRC_DIRS)/, *.cpp *.c)
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)

LDFLAGS := -Wall -Wl,--out-implib,libNewEngine.dll.a -Wl,--major-image-version,0,--minor-image-version,0
LDLIBS := -lglu32 -lopengl32 -L"$(LIB_DIR)/Glew/lib" -lglew32 -L"$(LIB_DIR)/SDL2/lib" -lSDL2d.dll -L"$(LIB_DIR)/assimp/lib" -lassimp.dll -L"$(LIB_DIR)/freetype/lib" -lfreetype -lkernel32 -luser32 -lgdi32 -lwinspool -lshell32 -lole32 -loleaut32 -luuid -lcomdlg32 -ladvapi32

#INC_DIRS := $(shell find $(SRC_DIRS) -type d)
#INC_FLAGS := $(addprefix -I,$(INC_DIRS))

#CPPFLAGS ?= $(INC_FLAGS) -MMD -MP

CPPFLAGS := -std=c++14 -g -ggdb
CXXFLAGS := -I$(CURDIR)/headers -I$(CURDIR)/sources -I$(CURDIR)/src -I$(LIB_DIR)/Glew/include -I$(LIB_DIR)/SDL2/include -I$(LIB_DIR)/assimp/include -I$(LIB_DIR)/freetype/include -msse2 -DDEBUG

$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	@echo "Building $(TARGET_EXEC)..."
	@$(CC) $(OBJS) -o $@ $(LDFLAGS) $(LDLIBS) 

# assembly
$(BUILD_DIR)/%.s.o: %.s
	@$(MKDIR_P) $(dir $@)
	@$(AS) $(ASFLAGS) -c $< -o $@

# c source
$(BUILD_DIR)/%.c.o: %.c
	@$(MKDIR_P) $(dir $@)
	@$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

# c++ source
$(BUILD_DIR)/%.cpp.o: %.cpp
	@echo "Building $@..."
	@$(MKDIR_P) $(dir $@)
	@$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

.PHONY: clean run cleanhigh cleangame

run:
	@echo "Running $(TARGET_EXEC)..."
	@"./$(BUILD_DIR)/$(TARGET_EXEC).exe"

clean:
	@echo "Cleaning binaries..."
	@$(RM) -r $(BUILD_DIR)/$(SRC_DIRS)

cleanhigh:
	@echo "Cleaning high level binaries..."
	@$(RM) -r $(BUILD_DIR)/$(SRC_DIRS)/gamecs
	@$(RM) -r $(BUILD_DIR)/$(SRC_DIRS)/terrain
	@$(RM) $(BUILD_DIR)/$(SRC_DIRS)/*.o
	@$(RM) $(BUILD_DIR)/$(SRC_DIRS)/rendering/*.o

cleangame:
	@echo "Cleaning game files..."
	@$(RM) -r $(BUILD_DIR)/$(SRC_DIRS)/gamecs
	#@$(RM) -r $(BUILD_DIR)/$(SRC_DIRS)/terrain
	@$(RM) $(BUILD_DIR)/$(SRC_DIRS)/game.cpp.o
	@$(RM) $(BUILD_DIR)/$(SRC_DIRS)/main.cpp.o


MKDIR_P ?= mkdir -p
