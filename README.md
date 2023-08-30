# FreeMHD
## Steps for openfoam compile/install (as of 04/24/23): 

### 0. Set up github key pair on cluster
(Note: This step is not perfect, using the steps that worked on 6/27/23): 
 following https://docs.github.com/en/authentication/connecting-to-github-with-ssh/generating-a-new-ssh-key-and-adding-it-to-the-ssh-agent?platform=linux
  * Generate a public/private key pair with `ssh-keygen -t ed25519 -C "bw0594@della.princeton.edu"`
* Enter a filename and passphrase to save key file
* Add ssh config file if it doesn't yet exist `touch ~/.ssh/config`
* Move new private key file into `~/.ssh/key1`
* Then add to agent with `eval "$(ssh-agent)"` and then `ssh-add ~/.ssh/key1` and enter passphrase to add idenitity
* Add new public SSH key to your github at https://github.com/settings/keys
   
   
### 1. Clone repository from https://github.com/PlasmaControl/FreeMHD using 
   `git clone <repo> <directory>` for example,
   ```
   git clone git@github.com:PlasmaControl/FreeMHD.git
   ```


### 2. Execute run functions
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
→ should say "System check: PASS"

### 4. Can continue to OpenFOAM installation. 

Run this under OpenFoam-v2206 directory (where the foam function takes you)
```
./Allwmake -s -l 
```
### 5. Make electric potential solver

Go to `MHD_Solvers/solvers/epotMultiRegionInterFoam/`

run `wmake`

### 6. Post-compilation checks:

Validate the build (not supported for cross-compilation) by running
`foamInstallationTest`

### 7. Test a tutorial case
```
foamTestTutorial -full incompressible/simpleFoam/pitzDaily 
```
### 8. If post-compilation checks all pass, installation steps are complete
