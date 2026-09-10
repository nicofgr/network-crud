# Code from https://makefiletutorial.com/
#

# Define color variables
RED    := \033[0;31m
GREEN  := \033[0;32m
YELLOW := \033[0;33m
BLUE   := \033[0;34m
NC     := \033[0m
CHECK  := $(GREEN)✔$(NC)



BUILD_DIR := ./build
SERVER_TARGET_EXEC := server.out
CLIENT_TARGET_EXEC := client.out


SERVER_SRC_DIRS  := ./src/server
CLIENT_SRC_DIRS  := ./src/client
SHARED_SRC_DIRS  := ./src/shared

SERVER_LIBS := -lm -lsqlite3

# Find all the C and C++ files we want to compile
# Note the single quotes around the * expressions. The shell will incorrectly expand these otherwise, but we want to send the * directly to the find command.
SERVER_SRCS := $(shell find $(SERVER_SRC_DIRS) -name '*.c')
CLIENT_SRCS := $(shell find $(CLIENT_SRC_DIRS) -name '*.c')
SHARED_SRCS := $(shell find $(SHARED_SRC_DIRS) -name '*.c')

# Prepends BUILD_DIR and appends .o to every src file
# As an example, ./your_dir/hello.cpp turns into ./build/./your_dir/hello.cpp.o
SHARED_OBJS := $(SHARED_SRCS:%=$(BUILD_DIR)/%.o)
SERVER_OBJS := $(SERVER_SRCS:%=$(BUILD_DIR)/%.o) $(SHARED_OBJS)
CLIENT_OBJS := $(CLIENT_SRCS:%=$(BUILD_DIR)/%.o) $(SHARED_OBJS)
ALL_OBJS    := $(sort $(SERVER_OBJS) $(CLIENT_OBJS))

# String substitution (suffix version without %).
# As an example, ./build/hello.cpp.o turns into ./build/hello.cpp.d
DEPS := $(ALL_OBJS:.o=.d)

# Every folder in ./src will need to be passed to GCC so that it can find header files
INC_DIRS := $(shell find $(SRC_DIRS) -type d)
# Add a prefix to INC_DIRS. So moduleA would become -ImoduleA. GCC understands this -I flag
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

# The -MMD and -MP flags together generate Makefiles for us!
# These files will have .d instead of .o as the output.
CPPFLAGS := $(INC_FLAGS) -MMD -MP -std=c99 -Wall -Wextra
LDFLAGS  += -fsanitize=address

all: $(BUILD_DIR)/$(SERVER_TARGET_EXEC) $(BUILD_DIR)/$(CLIENT_TARGET_EXEC)

$(BUILD_DIR)/$(SERVER_TARGET_EXEC): $(SERVER_OBJS)
	$(CC) $(CFLAGS) $(SERVER_OBJS) $(SERVER_LIBS) $(LDFLAGS) -o $@
	@echo " $(CHECK) Server compilation successful: $@"

$(BUILD_DIR)/$(CLIENT_TARGET_EXEC): $(CLIENT_OBJS)
	$(CC) $(CFLAGS) $(CLIENT_OBJS) $(CLIENT_LIBS) $(LDFLAGS) -o $@
	@echo " $(CHECK) Client compilation successful: $@"


# The final build step.
#$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
#	$(CXX) $(OBJS) -o $@ $(LDFLAGS) $(LIBS)

# Build step for C source
$(BUILD_DIR)/%.c.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@


.PHONY: clean server client
clean:
	rm -r $(BUILD_DIR)

server:
	./build/$(SERVER_TARGET_EXEC)
client:
	./build/$(CLIENT_TARGET_EXEC)
# Include the .d makefiles. The - at the front suppresses the errors of missing
# Makefiles. Initially, all the .d files will be missing, and we don't want those
# errors to show up.
-include $(DEPS)

