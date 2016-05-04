##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=word_grid
ConfigurationName      :=Debug
WorkspacePath          := "/home/gsjy/src/cpp/codelite"
ProjectPath            := "/home/gsjy/src/cpp/codelite/word_grid"
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=gsjy
Date                   :=04/05/16
CodeLitePath           :="/home/gsjy/.codelite"
LinkerName             :=/usr/bin/g++
SharedObjectLinkerName :=/usr/bin/g++ -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=.o.d
PreprocessSuffix       :=.i
DebugSwitch            :=-g 
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
SourceSwitch           :=-c 
OutputFile             :=$(IntermediateDirectory)/$(ProjectName)
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="word_grid.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). $(IncludeSwitch)./src/include 
IncludePCH             := 
RcIncludePath          := 
Libs                   := 
ArLibs                 :=  
LibPath                := $(LibraryPathSwitch). 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := /usr/bin/ar rcu
CXX      := /usr/bin/g++
CC       := /usr/bin/gcc
CXXFLAGS :=  -g -O0 -Wall $(Preprocessors)
CFLAGS   :=  -g -O0 -Wall $(Preprocessors)
ASFLAGS  := 
AS       := /usr/bin/as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/main.c$(ObjectSuffix) $(IntermediateDirectory)/src_word_list.c$(ObjectSuffix) $(IntermediateDirectory)/src_word_grid.c$(ObjectSuffix) 



Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild MakeIntermediateDirs
all: $(OutputFile)

$(OutputFile): $(IntermediateDirectory)/.d $(Objects) 
	@$(MakeDirCommand) $(@D)
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	$(LinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)

MakeIntermediateDirs:
	@test -d ./Debug || $(MakeDirCommand) ./Debug


$(IntermediateDirectory)/.d:
	@test -d ./Debug || $(MakeDirCommand) ./Debug

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/main.c$(ObjectSuffix): main.c $(IntermediateDirectory)/main.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/gsjy/src/cpp/codelite/word_grid/main.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/main.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/main.c$(DependSuffix): main.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/main.c$(ObjectSuffix) -MF$(IntermediateDirectory)/main.c$(DependSuffix) -MM "main.c"

$(IntermediateDirectory)/main.c$(PreprocessSuffix): main.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/main.c$(PreprocessSuffix) "main.c"

$(IntermediateDirectory)/src_word_list.c$(ObjectSuffix): src/word_list.c $(IntermediateDirectory)/src_word_list.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/gsjy/src/cpp/codelite/word_grid/src/word_list.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_word_list.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_word_list.c$(DependSuffix): src/word_list.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_word_list.c$(ObjectSuffix) -MF$(IntermediateDirectory)/src_word_list.c$(DependSuffix) -MM "src/word_list.c"

$(IntermediateDirectory)/src_word_list.c$(PreprocessSuffix): src/word_list.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_word_list.c$(PreprocessSuffix) "src/word_list.c"

$(IntermediateDirectory)/src_word_grid.c$(ObjectSuffix): src/word_grid.c $(IntermediateDirectory)/src_word_grid.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/gsjy/src/cpp/codelite/word_grid/src/word_grid.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_word_grid.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_word_grid.c$(DependSuffix): src/word_grid.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_word_grid.c$(ObjectSuffix) -MF$(IntermediateDirectory)/src_word_grid.c$(DependSuffix) -MM "src/word_grid.c"

$(IntermediateDirectory)/src_word_grid.c$(PreprocessSuffix): src/word_grid.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_word_grid.c$(PreprocessSuffix) "src/word_grid.c"


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Debug/


