# FreeMHD
FreeMHD is an open source magnetohydrodynamics (MHD) solver, recently developed for free-surface liquid metal (LM) flows under strong magnetic fields. The extreme heat fluxes > 10 MW/m2 in the divertor region of tokamaks may require an alternative to solid plasma-facing components, for the extraction of heat and the protection of the surrounding walls. However, codes to simulate the behavior of free-surface LM under fusion-relevant conditions are not available. Previous numerical studies have mainly used steady-state, 2D, or simplified models for internal flows and have not been able to adequately model free-surface LM experiments. Therefore, FreeMHD, aims to compute incompressible free-surface flows with multi-region coupling for the investigation of MHD phenomena involving fluid and solid domains. The objectives of the project involve validation and verification of FreeMHD to solve fully 3D transient MHD flows. The model utilizes the electric potential formulation to solve the inductionless MHD equations, implemented using the open-source, finite-volume OpenFOAM framework.

## FreeMHD Test Cases
[Link to Test Cases in Google Drive](https://drive.google.com/drive/folders/1gspbY8Nj1d3FsCjqjXhegnQi5WljMWrM?usp=drive_link)
1. Shercliff Flow
2. Dam Breaking
3. Pipe Flow Fringing B 
4. LMX-U


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
