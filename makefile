# Edit these variables if more directories are needed.
# Separate each entry by spaces.
CXX_DIRS=src

# This is likely specific to *nix.
PROJ=$(shell basename $(CURDIR))


CXX=g++
LD=g++

DEFINES=-DQT_NO_DEBUG -DQT_GUI_LIB -DQT_CORE_LIB -DQT_SHARED

BASE_FLAGS=-Wall -O3 -march=native $(DEFINES)
CXX_FLAGS=-std=c++14 -I/usr/include $(BASE_FLAGS) `pkg-config --cflags Qt5Core Qt5Gui Qt5Widgets`
S_FLAGS=
LD_FLAGS=-lm `pkg-config --libs Qt5Core Qt5Gui Qt5Widgets` 

OBJDIR=objs
DEPDIR=deps
OBJDIR_TEMP=objs_temp
MOC_SOURCE_DIR=moc_sources

# No, this stuff isn't portable to non-GNU versions of Make.
CXX_SOURCES=$(foreach DIR,$(CXX_DIRS),$(notdir $(wildcard $(DIR)/*.cpp)))
CXX_HEADERS=$(foreach DIR,$(CXX_DIRS),$(notdir $(wildcard $(DIR)/*.hpp)))
CXX_MOC_SOURCES=$(patsubst %.hpp,$(MOC_SOURCE_DIR)/%.moc.cpp,$(CXX_HEADERS))

export VPATH	:=	$(foreach DIR,$(CXX_DIRS),$(CURDIR)/$(DIR))


CXX_OFILES=$(patsubst %.cpp,$(OBJDIR)/%.o,$(CXX_SOURCES))
CXX_MOC_OFILES=$(patsubst $(MOC_SOURCE_DIR)/%.moc.cpp,$(OBJDIR)/%.moc.o,$(CXX_MOC_SOURCES))
OFILES=$(CXX_MOC_OFILES) $(CXX_OFILES)

CXX_PFILES=$(patsubst %.cpp,$(DEPDIR)/%.P,$(CXX_SOURCES))
PFILES=$(CXX_PFILES)

CXX_OFILES_TEMP=$(patsubst %.cpp,$(OBJDIR_TEMP)/%.o,$(CXX_SOURCES))
OFILES_TEMP=$(CXX_OFILES_TEMP)


all : all_pre $(OFILES)
	$(LD) $(OFILES) -o $(PROJ) $(LD_FLAGS)

debug : all_pre $(OFILES)
	$(LD) $(OFILES) -o $(PROJ) $(LD_FLAGS) -g

all_objs : all_pre $(OFILES)
	@#

all_pre :
	mkdir -p $(OBJDIR) $(DEPDIR) $(MOC_SOURCE_DIR)

$(CXX_OFILES) : $(OBJDIR)/%.o : %.cpp
	@#echo "Generating dependency information for "$@"...."
	@echo $@" was updated or has no object file.  (Re)Compiling...."
	$(CXX) $(CXX_FLAGS) -MMD -c $< -o $@
	@cp $(OBJDIR)/$*.d $(DEPDIR)/$*.P
	@sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
		-e '/^$$/ d' -e 's/$$/ :/' < $(OBJDIR)/$*.d >> $(DEPDIR)/$*.P
	@rm -f $(OBJDIR)/$*.d

-include $(PFILES)

$(CXX_MOC_SOURCES) : $(MOC_SOURCE_DIR)/%.moc.cpp : %.hpp
	moc-qt5 $(DEFINES) $< -o $@

$(CXX_MOC_OFILES) : $(OBJDIR)/%.moc.o : $(MOC_SOURCE_DIR)/%.moc.cpp
	$(CXX) $(CXX_FLAGS) -c $< -o $@


#¯\(°_o)/¯

.PHONY : clean
clean :
	rm -rfv $(OBJDIR) $(DEPDIR) $(MOC_SOURCE_DIR) $(PROJ) tags *.taghl

.PHONY : clean_objs_with_no_source
clean_objs_with_no_source :
	@mkdir -p $(OBJDIR_TEMP)
	@#ls $(OBJDIR)
	@echo "Removing object files that don't have corresponding source files...."
	@for objfile in $(OFILES); \
	do \
		if [ -f $$objfile ]; \
		then \
			mv $$objfile $(OBJDIR_TEMP); \
		fi; \
	done;
	@#ls $(OBJDIR_TEMP)
	@rm -rf $(OBJDIR)
	@mkdir -p $(OBJDIR)
	@for objfile in $(OFILES_TEMP); \
	do \
		if [ -f $$objfile ]; \
		then \
			mv $$objfile $(OBJDIR); \
		fi; \
	done;
	@#ls $(OBJDIR)
	@rmdir $(OBJDIR_TEMP)
	
	
	@#rm -rfv $(OBJDIR_TEMP)


