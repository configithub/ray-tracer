# -*- mode: Makefile; -*-
# -----------------------------------------
# project sheep


export PATH := $(PATH)
export LD_LIBRARY_PATH := $(LD_LIBRARY_PATH)



CFLAGS_C = $(filter-out -include "sdk.h",$(CFLAGS))

# -----------------------------------------

# MAKE_DEP = -MMD -MT $@ -MF $(@:.o=.d)

#CFLAGS = -g -std=c99 
#CFLAGS = -g -O3 -ffast-math -std=c99 
#CFLAGS = -g -O3 -std=c99 -ffloat-store -ffast-math -S
CFLAGS = -g -O3 -std=c99 -ffloat-store -ffast-math 
INCLUDES = 
LDFLAGS = 
RCFLAGS = 
#LDLIBS = $(T_LDLIBS)  -lstdc++

LINK_exe = gcc -o $@ $^ $(LDFLAGS) $(LDLIBS)
LINK_con = gcc -o $@ $^ $(LDFLAGS) $(LDLIBS)
LINK_dll = gcc -o $@ $^ $(LDFLAGS) $(LDLIBS) -shared
LINK_lib = rm -f $@ && ar rcs $@ $^
COMPILE_c = gcc $(CFLAGS_C) -o $@ -c $< $(MAKEDEP) $(INCLUDES)
#COMPILE_cpp = g++ $(CFLAGS) -o $@ -c $< $(MAKEDEP) $(INCLUDES)
COMPILE_rc = windres $(RCFLAGS) -J rc -O coff -i $< -o $@ -I$(dir $<)

%.o : %.c ; $(COMPILE_c)
%.o : %.cpp ; $(COMPILE_cpp)
%.o : %.cxx ; $(COMPILE_cpp)
%.o : %.rc ; $(COMPILE_rc)
.SUFFIXES: .o .d .c .cpp .cxx .rc

all: all.before all.targets all.after

all.before :
	-
all.after : $(FIRST_TARGET)
	
all.targets : Release_target 

clean :
	rm -fv $(clean.OBJ)
	rm -fv $(DEP_FILES)

.PHONY: all clean distclean


# -----------------------------------------
# Release_target

Release_target.BIN = bin/app
Release_target.OBJ = src/main.o src/render_tools.o src/ray_tracer.o src/data.o
DEP_FILES += src/main.d src/render_tools.d src/ray_tracer.d src/data.d
clean.OBJ += $(Release_target.BIN) $(Release_target.OBJ)

Release_target : Release_target.before $(Release_target.BIN) Release_target.after_always
Release_target : CFLAGS +=  
Release_target : INCLUDES += -I/usr/include
Release_target : RCFLAGS += 
Release_target : LDFLAGS +=  -lSDLmain -lSDL -lSDL_image -lGL -L/usr/lib/i386-linux-gnu   

Release_target.before :
	
	
Release_target.after_always : $(Release_target.BIN)
	
$(Release_target.BIN) : $(Release_target.OBJ)
	$(LINK_con)
	

# -----------------------------------------
ifdef MAKE_DEP
-include $(DEP_FILES)
endif
