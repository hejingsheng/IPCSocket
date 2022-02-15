CXX=g++
AR=ar -cr

C_INCLUDES = 

# compile gcc flags
CFLAGS =  $(C_INCLUDES) $(OPT) -Wall -Wextra
#CFLAGS += -g
# Generate dependency information
CFLAGS += -std=c++11

BUILD_DIR = build

CC_SOURCES = $(wildcard ./*.cc)

TARGET := libIPC.a

all: $(BUILD_DIR)/$(TARGET)
	
#######################################
# build the application
#######################################
# list of objects
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(CC_SOURCES:.cc=.o)))
vpath %.c $(sort $(dir $(CC_SOURCES)))

$(BUILD_DIR)/%.o: %.cc Makefile | $(BUILD_DIR) 
	@echo CXX $@
	$(CXX) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.cc=.lst)) $< -o $@

$(BUILD_DIR)/$(TARGET): $(OBJECTS) Makefile
	@echo LN $@
	$(AR) $@ $(OBJECTS)

$(BUILD_DIR):
	mkdir -p $@		

#######################################
# clean up
#######################################
clean:
	-rm -fR .dep $(BUILD_DIR)
  
#######################################
# dependencies
#######################################
-include $(shell mkdir .dep 2>/dev/null) $(wildcard .dep/*)

.PHONY: clean all

# *** EOF ***
