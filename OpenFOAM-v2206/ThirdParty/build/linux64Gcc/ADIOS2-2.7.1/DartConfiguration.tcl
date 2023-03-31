# This file is configured by CMake automatically as DartConfiguration.tcl
# If you choose not to use CMake, this file may be hand configured, by
# filling in the required variables.


# Configuration directories and files
SourceDirectory: /home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1
BuildDirectory: /home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/build/linux64Gcc/ADIOS2-2.7.1

# Where to place the cost data store
CostDataFile: 

# Site is something like machine.domain, i.e. pragmatic.crd
Site: stellar-intel.princeton.edu

# Build name is osname-revision-compiler, i.e. Linux-2.4.2-2smp-c++
BuildName: Linux-mpicxx

# Subprojects
LabelsForSubprojects: 

# Submission information
SubmitURL: http://open.cdash.org/submit.php?project=ADIOS

# Dashboard start time
NightlyStartTime: 01:00:00 UTC

# Commands for the build/test/submit cycle
ConfigureCommand: "/usr/bin/cmake" "/home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1"
MakeCommand: /usr/bin/cmake --build . --config "${CTEST_CONFIGURATION_TYPE}"
DefaultCTestConfigurationType: Release

# version control
UpdateVersionOnly: 

# CVS options
# Default is "-d -P -A"
CVSCommand: 
CVSUpdateOptions: 

# Subversion options
SVNCommand: 
SVNOptions: 
SVNUpdateOptions: 

# Git options
GITCommand: 
GITInitSubmodules: 
GITUpdateOptions: 
GITUpdateCustom: 

# Perforce options
P4Command: 
P4Client: 
P4Options: 
P4UpdateOptions: 
P4UpdateCustom: 

# Generic update command
UpdateCommand: 
UpdateOptions: 
UpdateType: 

# Compiler info
Compiler: /usr/local/openmpi/4.1.0/gcc/bin/mpicxx
CompilerVersion: 8.5.0

# Dynamic analysis (MemCheck)
PurifyCommand: 
ValgrindCommand: 
ValgrindCommandOptions: 
DrMemoryCommand: 
DrMemoryCommandOptions: 
CudaSanitizerCommand: 
CudaSanitizerCommandOptions: 
MemoryCheckType: 
MemoryCheckSanitizerOptions: 
MemoryCheckCommand: /usr/bin/valgrind
MemoryCheckCommandOptions: 
MemoryCheckSuppressionFile: /home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/scripts/dashboard/nightly/valgrind-suppressions.txt

# Coverage
CoverageCommand: /usr/bin/gcov
CoverageExtraFlags: -l

# Testing options
# TimeOut is the amount of time in seconds to wait for processes
# to complete during testing.  After TimeOut seconds, the
# process will be summarily terminated.
# Currently set to 25 minutes
TimeOut: 1500

# During parallel testing CTest will not start a new test if doing
# so would cause the system load to exceed this value.
TestLoad: 

UseLaunchers: 
CurlOptions: 
# warning, if you add new options here that have to do with submit,
# you have to update cmCTestSubmitCommand.cxx

# For CTest submissions that timeout, these options
# specify behavior for retrying the submission
CTestSubmitRetryDelay: 5
CTestSubmitRetryCount: 3
