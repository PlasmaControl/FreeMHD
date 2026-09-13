<!-- [page](page.md) -->
# OpenFOAM Installation

## Step 0: Create a GitHub public-private key pair

1. Navigate to the environment and directory where you want to install OpenFOAM.

1. In the command line, enter:
    ```ssh-keygen -t ed25519 -f ~./ssh/id_ed25519 -C "{comment}"```

    Commands:

    - `-t`: Stands for “type”. Specifies the type of key to generate.
    - `-f` (optional): Stands for “file”. Specifies the filename/location where the key will be saved.
    - `-C` (optional): Stands for “comment”. Adds a label to the public key and makes it easier to distinguish later.
    - `-t ed25519`: Creates an ed25519 key pair.
    Meaning:
    - ```-f ~/.ssh/id_ed25519```: Saves the private key as ```~/.ssh/id_ed25519``` and the public key as ```~/.ssh/id_ed25519.pub```. If you omit `-f`, ssh-keygen will ask: ```Enter file in which to save the key (/home/user/.ssh/id_ed25519):```. You may enter a directory path, or press Enter to use the default directory.

1. Enter a passphrase to save the key field. Press Enter to skip the passphrase.
1. Add the ssh config file (if it does not yet exist): ```touch ~/.ssh/config```
1. ??? Start the ssh-agent in the background: ```eval "$(ssh-agent)"```
1. Add the key to the agent: ```ssh-add ~/.ssh/id_ed25519```. Enter passphrase when prompted.
1. Add the new public SSH key to your [GitHub](https://github.com/settings/keys) (or paste this link into your browser: [https://github.com/settings/keys](https://github.com/settings/keys))


    Enter `cat ~/.ssh/id_ed25519.pub`. Copy the output that starts with `ssh-ed25519`. In GitHub, create a title, selection "Key Type: Authentication Key", and paste the full ssh-ed25519 line into the "Key" box.

    
To check whether GitHub recognizes the public key, enter ```ssh -T git@github.com```. If it says “You’ve successfully authenticated, but GitHub does not provide shell access,” then your public key has been successfully added to GitHub. If it says “Permission denied,” then GitHub does not recognize the key. In this case, you may need to repeat Step 0.5-0.6.

You may visit GitHub for more information about [adding SSH keys](https://docs.github.com/en/authentication/connecting-to-github-with-ssh/generating-a-new-ssh-key-and-adding-it-to-the-ssh-agent?platform=linux) and checking for [existing SSH keys](https://docs.github.com/en/authentication/connecting-to-github-with-ssh/checking-for-existing-ssh-keys).















## Step 1: Clone the GitHub repository
Note: Make sure to clone the repository in the directory where you want to access it.
1. Go to [https://github.com/PlasmaControl/FreeMHD](https://github.com/PlasmaControl/FreeMHD)
1. In the command line, enter ```git clone git@github.com:PlasmaControl/FreeMHD.git```

    In general, you can clone a GitHub repo by entering ```git clone <repo> <directory>``` in the command line.

    [Cloning slowdown](#cloning-slowdown): If your repo cloning slows down significantly or stops, please see the [cloning slowdown issue](#cloning-slowdown).

    [Denied access](#denied-access): If you receive an error message stating that you do not have access to the repo, please see the [denied access issue](#denied-access).

3. Check to make sure the GitHub repo was cloned by entering `ls`. It should show `FreeMHD`.
    - Here is how mine appeared:

![FreeMHD GitHub Clone Confirmation](/Users/sophie/Downloads/OpenFOAM_installation_github_clone_screenshot.png)

4. (Optional) If you cloned the repo in a scratch directory, you may copy the repo from the scratch into your home directory. This may take a few minutes. Enter ```cp -r /path/to/source_directory/subdirectory /path/to/destination_directory/```. Enter `ls` to check whether the copy transferred.






## Step 2: Execute run functions
1. From within your cloned FreeMHD GitHub repo, on the command line, enter the following run functions in this order:

`module load gcc/8`

`module load openmpi/gcc/4.1.0`

`source /home/{USER}/FreeMHD/OpenFOAM-v2206/etc/bashrc`
(replace `{USER}` with your username)

`. ${WM_PROJECT_DIR:?}/bin/tools/RunFunctions`
(enter exactly as written)

[Unable to locate a modulefile](#no-gcc-openmpi): If you receive an error reading `Unable to locate a modulefile for 'gcc/8'` (or `openmpi/gcc/4.1.0`), please see the ["unable to locate modulefile" issue](#no-gcc-openmpi).

[Unable to locate `etc/bashrc`](#no-etc/bashrc): After trying to run `source /home/{USER}/FreeMHD/OpenFOAM-v2206/etc/bashrc`, if you receive an error stating that no such file/directory can be found, please see the [no `etc/bashrc` issue](#no-etc/bashrc).

















## Step 3: Perform foam checks

## Step 4: Install OpenFOAM

## Step 5: Make electric potential solver

## Step 6: Perform post-compilation checks

## [COMBINE WITH STEP 8 & FREEMHD TUTORIAL]Step 7: Test a tutorial case

## [COMBINE WITH STEP 7 & FREEMHD TUTORIAL]Step 8: Start using FreeMHD!

## Common Issues and Solutions


- <a id="denied-access"></a>Issue 1 (Denied Access): When trying to clone the GitHub repo in the command line (Step 1.2), you encounter an error stating that you do not have access to the repo.
    - Possible reason: The public-private key pair was not added properly.
    - Possible solution: Follow Step 0.5-0.6. 

- <a id="cloning-slowdown"></a>Issue 2 (Cloning Slowdown): The GitHub repo cloning slows down and perhaps stops altogether.
  - Possible reason: The repo is too large relative to the space available in the directory where it is being cloned.
        - My situation: My cloning attempt got stuck at the `Updating files` step. A few hours later, it timed out, saying `client_loop: send disconnect: Broken pipe`. When my first GitHub repo clone got stuck in my Della home directory, I created a scratch directory separate from my home directory in the Della environment. I ended up copying the scratch directory (with the clone) into my home directory.
    - Possible solution: Clone the repo in a scratch directory (if using a Princeton/PPPL HPC) or in a directory with more space available.
        - My situation: I created my own scratch directories in Della and Stellar with the path `/scratch/gpfs/EKOLEMEN/my_scratch`. Within this directory, I evaluated and added the public-private key pairinh (Step 0.5-0.6), entered my passphrase, and successfully cloned the repo using `git clone git@github.com:PlasmaControl/FreeMHD.git`.


- <a id="no-gcc-openmpi"></a>Issue 3 (No `etc`): When attempting to execute the run functions in Step 2.1, you receive an error message reading `ERROR: Unable to locate a modulefile for 'gcc/8'`. This can also occur for openmpi: `ERROR: Unable to locate a modulefile for 'openmpi/gcc/4.1.0'`.
    - Possible reason: There is no gcc/8 or openmpi/gcc/4.1.0 installed.
    - Possible solution: In the command line, enter `module avail gcc` and `module avail openmpi`. In Step 2.1, replace gcc/8 with whichever gcc is shown (mine is gcc/11), and replace openmpi/gcc/4.1.0 with the most recent openmpi/gcc shown (mine is 4.1.8).
    - Note: These version numbers will be different depending on the HPC you use.

- <a id="no-etc/bashrc"></a>Issue 4 (No `etc/bashrc`): When attempting to execute the run functions in Step 2.1, you receive a message indicating that there is no file or directory named `etc`.
    - Here is how this error appeared for me:
![No etc/bashrc](/Users/sophie/Downloads/OpenFOAM_installation_nobashrc.png)
    - Possible reason: The GitHub repo tree got mixed up during the cloning, copying, or moving of the repo.
    - Possible solution: Rename or delete the broken FreeMHD repo. Retry cloning from GitHub or copying from the directory where you initially cloned the repo.
    - Possible test: From within your FreeMHD directory, you can check which files are deleted or untracked (meaning uncommitted or unstaged by GitHub) by entering ```git status``` on the command line. Doing so outputs a long list of filed that say `deleted: {relative file path}`:
![Check OpenFOAM-v2206 files using `git status`](/Users/sophie/Downloads/OpenFOAM_installation_gitstatus.png)
    - In my case, under `Untracked files:`, `OpenFOAM-v2206` was listed. This indicated that `OpenFOAM-v2206` had its tree mixed up during copying.
        - My situation: I cloned the GitHub repo into my scratch directory. Then, I tried to copy it into my home directory because cloning it directly into my home resulted in the cloning slowdown error. The GitHub repo tree seemed mixed up in the home FreeMHD directory, but not in the scratch directory. Step 2 of the installation requires the execution of the following run function: `source /home/{user}/FreeMHD/OpenFOAM-v2206/etc/bashrc`. However, my repo did not contain any `etc` or `bashrc`. I checked the FreeMHD directory in my original scratch directory, and it had `OpenFOAM-v2206/etc/bashrc`. I added a new public-private key pair in my home directory and cloned FreeMHD GitHub again under the name `FreeMHD_new` (and saved the old one under `FreeMHD_broken`). `FreeMHD_new` contained `OpenFOAM-v2206/etc/bashrc`. From here, I continued Step 2.





