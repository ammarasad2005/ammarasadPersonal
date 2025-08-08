# Makefile for Space Shooter Game
# Requires MASM32 and Irvine library

ASM = ml
LINK = link
TARGET_BASIC = SpaceShooter.exe
TARGET_ENHANCED = SpaceShooterEnhanced.exe
SOURCE_BASIC = SpaceShooter.asm
SOURCE_ENHANCED = SpaceShooterEnhanced.asm
OBJ_BASIC = SpaceShooter.obj
OBJ_ENHANCED = SpaceShooterEnhanced.obj

# Build flags
ASMFLAGS = /c /coff
LINKFLAGS = /subsystem:console
LIBS = irvine32.lib kernel32.lib user32.lib

# Default target - build both versions
all: basic enhanced

# Build basic version
basic: $(TARGET_BASIC)

# Build enhanced version
enhanced: $(TARGET_ENHANCED)

# Build basic executable
$(TARGET_BASIC): $(OBJ_BASIC)
	$(LINK) $(LINKFLAGS) $(OBJ_BASIC) $(LIBS)

# Build enhanced executable
$(TARGET_ENHANCED): $(OBJ_ENHANCED)
	$(LINK) $(LINKFLAGS) $(OBJ_ENHANCED) $(LIBS)

# Assemble basic source
$(OBJ_BASIC): $(SOURCE_BASIC)
	$(ASM) $(ASMFLAGS) $(SOURCE_BASIC)

# Assemble enhanced source
$(OBJ_ENHANCED): $(SOURCE_ENHANCED)
	$(ASM) $(ASMFLAGS) $(SOURCE_ENHANCED)

# Clean build artifacts
clean:
	del *.obj *.exe 2>nul || true

# Run basic version
run-basic: $(TARGET_BASIC)
	$(TARGET_BASIC)

# Run enhanced version
run-enhanced: $(TARGET_ENHANCED)
	$(TARGET_ENHANCED)

.PHONY: all basic enhanced clean run-basic run-enhanced