# FreeMHD
## Steps for openfoam compile/install (as of 04/24/23): 


1. Clone repository from https://github.com/PlasmaControl/FreeMHD
2. Execute run functions
```
module load gcc/8
module load openmpi/gcc/4.1.0
source /home/{user}/FreeMHD/OpenFOAM-v2206/etc/bashrc
. ${WM_PROJECT_DIR:?}/bin/tools/RunFunctions
```
3. Perform checks (run in command line):
```
foam
```
→ should take you to your OpenFoam-v2206 directory
```
foamSystemCheck
```
→ should say System check: PASS
4. Can continue to OpenFOAM installation
Run this under OpenFoam-v2206 directory (where the foam function takes you)
```
Run ./Allwmake -s -l 
```
5. Make electric potential solver

Go to `MHD_Solvers/solvers/epotMultiRegionInterFoam/`
run `wmake`

6. Post-compilation checks:

Validate the build (not supported for cross-compilation) by running
`foamInstallationTest`

7. Test a tutorial case
```
foamTestTutorial -full incompressible/simpleFoam/pitzDaily 
```
8. Installation is complete
