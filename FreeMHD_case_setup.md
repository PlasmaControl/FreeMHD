<!-- [page](page.md) -->
# FreeMHD Case Setup

## Basic Workflow:

1. Create the mesh (this sets box dimension $L$)
1. Change parameters:
   
   2a. Change the physical parameters: ${B_0, \, U, \, \mu, \, \rho, \, \sigma, \, \alpha}$
   
   2b. Change the simulation parameters:

   - number of total cores
   - number of nodes
   - number of cores per node
   - simulation run time
   - writeout intervals
   - etc.


### Physical Parameters: 
- $B_0$ (`B0`): background **B**-field (T). Format: ```(B0_x, B0_y, B0_z)```
  - Where to change: ```{casehome}/system/fluid/changeDictionaryDict```
  - Our example: ```B0 = (0 0.01 0)``` for both `internalField` and `boundary`.
- $U$ (`U`): fluid velocity (m/s). In the lid-driven case, $U$ is nonzero only at `boundaryField` --> `topOpening` [type: `fixedValue`, value: `uniform (Ux Uy Uz)`]
  - Where to change: ```{casehome}/system/fluid/changeDictionaryDict```
  - Our example: ```U = (10 0 0)```
- $\mu$ (`mu`): dynamic viscosity (${\text{kg} \cdot \text{m}^{-1} \text{s}^{-1}}$). ${\mu = \rho \, \nu}$, where $\rho$ = mass density, and $\nu$ = kinematic viscosity.
  - Measures internal resistance to flow and deformation.
  - Where to change: ```/constant/fluid/thermoPhysicalProperties.liquidMetal```
  - Our example: $\mu$ = 1.
- $\rho$ (`rho`): mass density (${\text{kg} \cdot \text{m}^3}$)
  - To change: ```/constant/fluid/thermoPhysicalProperties.liquidMetal```
  - Our example: $\rho$ = 2000.
- $\sigma$ (`elcond`): electrical conductivity (${\text{S} \cdot \text{m}^{-1}}$)
  - To change: ```/constant/fluid/thermoPhysicalProperties.liquidMetal```
  - Our example: $\sigma$ = $10^8$. In our case, ${\sigma_{\text{wall}} = \sigma_{\text{internal fluid}}}$.
- $\alpha$ (`alpha.liquidMetal`): Liquid metal percent of a region (dimensionless). So $\alpha$ = 1.0 means the entire region is liquid metal; $\alpha$ = 0 means the entire region is gas (no liquid metal).
  - To change: `/constant/fluid/changeDictionaryDict`
  - Our example: $\alpha$ = 1 everywhere.
  - To change the shape of a region of $\alpha$, go to `/system/fluid/setExprFieldsDict` and change `alpha.liquidMetal` in `expressions`.
    - Our example for a liquid metal droplet:

```
expressions
(

    alpha.liquidMetal
    {
        field       alpha.liquidMetal;
        dimensions  [0 0 0 0 0 0 0];
        variables
        (   
            "xPos = pos().x()"
            "yPos = pos().y()"
            "zPos = pos().z()"
            "Xc = 0.055"          // X center of the sphere
            "Yc = 0.055"          // Y center of the sphere
            "Zc = 0.0525"         // Z center of the sphere (example: 50 mm)
            "R0 = 20E-3"         // Sphere radius (example: 5 mm)
            "Zh = 0.01"         // Height threshold below which alpha = 1
        );
        expression
        #{
            ( (sqrt(pow(xPos - Xc, 2) + pow(yPos - Yc, 2) + pow(zPos - Zc, 2)) <= R0) || (zPos < Zh) ) ? 1 : 0
        #};
    }
```

- Note: `dimensions [1 2 3 4 5 6 7]` sets the dimensions of parameters corresponding to the following format:
![FreeMHD Dimensions](/Users/sophie/Downloads/FreeMHD_dimensions.png)
- The number in each space indicates the power to which the unit is raised.
  - Example: `[ 0 1 -1 0 0 0 0 ]` means ${\text{length}^1 * \text{time}^{-1} = \text{metre/second} = \text{velocity}}$
  - Each physical parameter (${\alpha, \, B0, \, U, }$ etc.) has different dimensions defined in this way in `/0/fluid/{parameter}`.


### Simulation Parameters:
<!-- I think -->These are specific to Princeton's/PPPL's high-performance computing (HPC) cluster:
- \# cores per node: Number of cores in each computing node. Can be up to 96. 
  - To change, go to `startRun` and change at the top at `ntasks-per-node`.
- \# nodes: Number of parallel nodes that the HPC uses to process your run.
  - To change, go to `startRun` and change at the top at `nodes`.
- \# total cores: Total number of cores to use for your run.
  - \# total cores = (\# cores/node) * (\# nodes)
  - To change, must change in all of the following locations:
    - `system/decomposeParDict`
    - `system/{each region}/decomposeParDict`
      - For example: in our case, we would have to change \# total cores in `system/fluid/decomposeParDict`, `system/solid/decomposeParDict`, and `system/solid2/decomposeParDict`
    - `startRun` (at the bottom, at `srun -n {\# total cores} ... runLog`)
      - For example: In our case, we have 9 nodes with 96 cores/node. So \# total cores = (96 cores/node) * (9 nodes) = 864 total cores. So in our `startRun`, we have `srun -n 864 epotMultiRegionInterFoam -parallel 2>&1 | tee ./runLog`



## Returning to the Basic Workflow:
Unless otherwise stated, all commands should be run from the base directory of your case directory, AKA from `{username}/{home/scratch/etc.}/{case directory}`

3.  Run `unvAndMeshSplitSet` to "upload" the mesh. Make sure to include the mesh you want to use in `unvAndMeshSplitSet` in `ideasUnvToFoam`. This converts the mesh file (.unv) into [[THE OPENFOAM FORMAT (IN CONSTANT/POLYMESH FOLDER)]]//a .foam file that FreeMHD can read//.
- Example: ```ideasUnvToFoam /scratch/gpfs/username/FOAM_RUN/FreeMHD_Tutorials/UNVs/LM_Box1_Mesh2mm_0.2MCells.unv```

4. (Optional) Refine the mesh.

   a. It can be helpful to view the mesh in Paraview to visualize which section(s) you are refining. To view the mesh in Paraview from a Princeton/PPPL HPC:
   - Sign in to [Stellar](mystellar.princeton.edu) or [Della](mydella.princeton.edu) (or whichever HPC you are using) with your Princeton University/PPPL credentials.
     - Click on "Interactive Apps", then "Desktop on Stellar Vis1 node". Set the number of hours for which you would like your session to run, click "Launch", and wait a moment for the session to start. Click "Launch Desktop on Stellar vis1 node".
     - Open the terminal in the top bar next to the Firefox logo.
     - Load the latest version of Paraview (mine is 5.13.1): `module load paraview/5.13.1`
     - Open Paraview by entering `paraview`
     - Go to File --> Open and enter your case directory name. To view the mesh, open `{your case directory name}/Box.foam`
       - If there is no `Box.foam` file in your case directory, create one. This tells Paraview the file type it is trying to display.
     - Select only the region you want to view.
       - Example: `/fluid/internalMesh`
     - Select "vtkBlockColors" and "Surface With Edges".
     - Create a slice. Click "Apply", then click on the eye symbol next to "Box.foam" so that the eye is open.
     - In the left panel, make sure "Triangulate the slice" is unchecked.
     - Adjust the plane origin and normal to determine the spatial dimensions of the mesh region you would like to refine. It may be helpful to write down these dimensions as ```(x_min y_min z_min)(x_max y_max z_max)```.

   b. In your code editor (like Visual Studio Code) go to `{your_case_directory}/system` and open `topoSetDict`.
   - Enter the ```(x_min y_min z_min)(x_max y_max z_max)``` for the mesh region you want to refine. These are the spatial dimensions you determined in Paraview. Write them with parentheses and without commas. Save your changes.
   - Then, navigate to your base case directory. In the command line, enter ```sbatch refineMesh```. This refines the mesh. You can refine it multiple times in different areas, if you wish.
   - Your case will not run with these refinements until you enter ```sbatch meshSplitSetFields```.
     - DO NOT RUN `unvAndMeshSplitSet` after refining the mesh because running this will set your mesh back to its original version (without refinements or scaling).
   - Note: The refinements made in this case directory are saved into in this case directory and its subsequent copies.
  
    c. (Optional) Mesh scaling: To scale your mesh (and its refinements) up or down, open `transformPointsToScale` from the case's base directory. Change `transformPoints -scale'(A B C)'`, where A, B, and C are the factors by which the x, y, and z dimensions of your mesh will be scaled, respectively. Then, run `sbatch transformPointsToScale`.
   - Example: To scale your mesh up by 2, run `transformPoints -scale'(2 2 2)'`. To make your mesh smaller by a factor of 10 (decreasing the dimensions from 2m x 2m x 2m to 0.2m x 0.2m x 0.2m), run `transformPoints -scale'(0.1 0.1 0.1)'`.

5. Run `sbatch meshSplitSetFields` (if you have not already) to save/apply the mesh refinements and scaling.
6. After ensuring that you have set all of the physical, simulation, and mesh parameters as you want them, you may run your case by entering `sbatch startRun`.

<!-- [page](page.md) -->
## Common Issues and Solutions

- OOM (Out of Memory): OOM means 'out of memory'.
  - Example: `error: Detected 1 oom_kill event in StepId=2922739.batch. Some of the step tasks have been OOM Killed.`
  - Possible reason: The job requires more RAM than is available in the system.
    - For instance, if using Slurm to schedule jobs, the OOM error may indicate that your job requires more memory than Slurm allocated to it. <!-- May be specific to Princeton/PPPL-->
  - Possible solution: Open the file you would like to run. Toward the top, allocate more memory by increasing `#SBATCH --mem-per-cpu=4G` (example: `#SBATCH --mem-per-cpu=8G`)

- hello 2
<!-- - <a id="denied-access"></a>Issue 1 (Denied Access): When trying to clone the GitHub repo in the command line (Step 1.2), you encounter an error stating that you do not have access to the repo.
    - Possible reason: The public-private key pair was not added properly.
    - Possible solution: Follow Step 0.5-0.6. 

- <a id="cloning-slowdown"></a>Issue 2 (Cloning Slowdown): The GitHub repo cloning slows down and perhaps stops altogether.
  - Possible reason: The repo is too large relative to the space available in the directory where it is being cloned.
        - My situation: My cloning attempt got stuck at the `Updating files` step. A few hours later, it timed out, saying `client_loop: send disconnect: Broken pipe`. When my first GitHub repo clone got stuck in my Della home directory, I created a scratch directory separate from my home directory in the Della environment. I ended up copying the scratch directory (with the clone) into my home directory.
    - Possible solution: Clone the repo in a scratch directory (if using a Princeton/PPPL HPC) or in a directory with more space available.
        - My situation: I created my own scratch directories in Della and Stellar with the path `/scratch/gpfs/EKOLEMEN/my_scratch`. Within this directory, I evaluated and added the public-private key pairinh (Step 0.5-0.6), entered my passphrase, and successfully cloned the repo using `git clone git@github.com:PlasmaControl/FreeMHD.git`.

-->
