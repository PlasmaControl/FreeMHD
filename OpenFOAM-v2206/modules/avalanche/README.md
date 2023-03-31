# Avalanche Simulation Tools for OpenFOAM

**The OpenFOAM user guide can be found on https://www.openfoam.com/documentation/user-guide/**

- **Author: Matthias Rauter**
- **Mail: matthias (@) rauter (.) it**
- **Twitter: igt_matti**


## Introduction

This module provides a model and tools for the simulation of **dense snow avalanches** and other **gravitational mass flows**. A **depth-integrated flow model** is applied and solved with the **Finite Area Method**. Tools for simulation setup is provided.


## Copyright

##### Source code

Licence: GPL-3.0-or-later

##### Real case example

The digital elevation model for tutorial **wolfsgrube** is provided by [AMT DER TIROLER LANDESREGIERUNG (AdTLR) Abteilung Geoinformation](https://gis.tirol.gv.at/ogd/geografie_planung/DGM/LA_DGM10.zip)

Licence: Creative Commons Attribution 3.0 License (CC-BY).


The digital elevation model for tutorial **montereycanyon** is provided by [Monterey Bay Aquarium Research Institute (MBARI)](http://www3.mbari.org/data/mapping/Monterey_Bay/default.htm)

Licence: MBARI provides these data "as is", with no warranty, express or implied, of the data quality or consistency. Data are provided without support and without obligation on the part of the Monterey Bay Aquarium Research Institute to assist in its use, correction, modification, or enhancement.



## Solver

### faSavageHutterFoam

The implicit solver can be found in `applications/solver/faSavageHutterFoam`. It can be called with `faSavageHutterFoam`.
The solver is based on a depth-integrated flow model, similar to the Savage-Hutter model ([Savage and Hutter; 1989](https://dx.doi.org/10.1017/S0022112089000340), [1991](https://dx.doi.org/10.1007/BF01175958)).
The theory of this solver is described by [Rauter and Tukovic (2018)](https://dx.doi.org/10.1016/j.compfluid.2018.02.017). The respective preprint is availabel on [arxiv.org](https://arxiv.org/abs/1802.05229).
The application to natural terrain is described by [Rauter, Kofler, Huber and Fellin (2018)](https://doi.org/10.5194/gmd-11-2923-2018).

The **governing equations** can be expressed in terms of surface partial differential equations as


$\dfrac{\partial h}{\partial t} + \boldsymbol{\nabla}{\cdot}\left(h\,\mathbf{\overline{u}}\right) = S_e-S_d,$ (1)

$\dfrac{\partial\,\left(h\,\mathbf{\overline{u}}\right)}{\partial t} + \boldsymbol{\nabla}_{\mathrm{s}}{\cdot}\left(h\,\mathbf{\overline{u}}\,\mathbf{\overline{u}}\right) = -\dfrac{1}{\rho}\boldsymbol{\tau}_{\mathrm{b}} + h\,\mathbf{g}_{\mathrm{s}} - \dfrac{1}{2\,\rho}\boldsymbol{\nabla}_{\mathrm{s}}\,\left(h\,p_{\mathrm{b}}\right),$ (2)


$\boldsymbol{\nabla}_{\mathrm{n}}{\cdot}\left(h\,\mathbf{\overline{u}}\,\mathbf{\overline{u}}\right) = h\,\mathbf{g}_{\mathrm{n}} - \dfrac{1}{2\,\rho}\boldsymbol{\nabla}_{\mathrm{n}}\,\left(h\,p_{\mathrm{b}}\right) - \dfrac{1}{\rho}\mathbf{n}_{\mathrm{b}}\,p_{\mathrm{b}},$ (3)

with **unknown flow fields**
* depth-averaged flow velocity $\mathbf{\overline{u}}$,
* flow thickness $h$ and
* basal pressure $p_{\mathrm{b}}$.

**Constant parameters** are
* the density $\rho$ and
* the gravitational acceleration $\mathbf{g} = \mathbf{g}_{\mathrm{s}}+\mathbf{g}_{\mathrm{n}}$.

**User-selectable models** (see below) have to be provided for
* the basal friction $\boldsymbol{\tau}_{\mathrm{b}}$,
* the volumetric entrainment rate $S_e$ and
* the deposition rate $S_d$.

**Initial conditions** have to be provided for
* the flow thickness $h$,
* the depth-integrated flow velocty $\mathbf{\overline{u}}$ and
* the mountain snow cover thickness $h_{\mathrm{msc}}$ (only if entrainment/deposition is active).

**The classic surface pressure equation** (e.g. [Fischer et al. 2013](dx.doi.org/10.1016/j.coldregions.2012.01.005)) can be applied by
* deactivating the second term on the right hand side of Equation (3) (switch pressureFeedback in tansportProperties).

**Spatial differential operators**

$\boldsymbol{\nabla}_{\mathrm{n}} = \left(\mathbf{n}_{\mathrm{b}}\,\mathbf{n}_{\mathrm{b}}\right)\boldsymbol{\cdot}\boldsymbol{\nabla}$

and

$\boldsymbol{\nabla}_{\mathrm{s}} = \left(\mathbf{I}-\mathbf{n}_{\mathrm{b}}\,\mathbf{n}_{\mathrm{b}}\right)\boldsymbol{\cdot}\boldsymbol{\nabla}$

are described in detail by [Rauter and Tukovic (2018)](https://arxiv.org/abs/1802.05229).

The numerical solution is based on the **Finite Area Method**.



#### Friction models

The friction model describes the friction $\boldsymbol{\tau}_{\mathrm{b}}$ between flowing mass (avalanche) and the terrain. The direction of the friction vector always aligns with the velocity vector $\mathbf{\overline{u}}$.

The friction model has to be set in the file `constant/transportProperties` (see below).
Friction models can be found in the folder `src/avalanche/friction`.
To implement a new friction model, copy an existing one, rename it and modify it.

Currently there are the following friction models available:
* DarcyWeisbach (Liquid)
* ManningStrickler (Liquid)
* MuI (Granular flow, avalanche)
* PoliquenForterre (Granular flow, avalanche)
* Voellmy (Granular flow, avalanche)
* kt (Granular flow, avalanche)

###### Darcy-Weisbach

`DarcyWeisbach`

Weisbach (1845) [(wikipedia.org)](https://en.wikipedia.org/wiki/Darcy%E2%80%93Weisbach_equation):

This friction model was applied in comparisons with analytical solutions in [Rauter and Tukovic (2018)](https://arxiv.org/abs/1802.05229), section 6.1.3.

$|\boldsymbol{\tau}_{\mathrm{b}}|  = C_f^2\,\rho\,g\,|{\bf u}|^2$

```
frictionModel       DarcyWeisbach;
DarcyWeisbachCoeffs
{
    Cf              Cf [0 -1 2 0 0 0 0] 0.000625;

    g               g  [0 1 -2 0 0 0 0] 9.81;
}
```

###### Manning-Strickler

`ManningStrickler`

Manning (1890) [(wikipedia.org)](https://en.wikipedia.org/wiki/Manning_formula):

$|\boldsymbol{\tau}_{\mathrm{b}}| = n^2\,\rho\,g\,\dfrac{|{\bf u}|^2}{h^{1/3.}}$

Parameters:
```
frictionModel       ManningStrickler;
ManningStricklerCoeffs
{
    n               n [ 0 -0.333333333333333 1 0 0 0 0] 1.0;

    g               g [0 1 -2 0 0 0 0] 9.81;
}
```

###### Voellmy

`Voellmy`

Friction model following [Voellmy (1955)](dx.doi.org/10.5169/seals-61891):

This friction model was applied in [Rauter, Kofler, Huber and Fellin (2018)](https://doi.org/10.5194/gmd-11-2923-2018).

$|\boldsymbol \tau| = \mu\,p + \dfrac{\rho\,g}{\xi}\,|{\bf u}|^2$

Parameters (see also tutorial `wolfsgrube`):
```
frictionModel       Voellmy;
VoellmyCoeffs
{
    mu              mu [0 0 0 0 0 0 0] 0.25;        //dry friction coefficient

    xi              xi [0 1 -2 0 0 0 0] 10000;      //voellmy turbulence coefficient
}
```

###### Kinetic Theory

`kt`

Simplified Kinetic Theory following [Rauter et al. (2016)](https://www.nat-hazards-earth-syst-sci.net/16/2325/2016/).

$|{\boldsymbol \tau}| = \mu\,p + \dfrac{\rho\,g}{\chi}\dfrac{|{\bf u}|^2}{h^2}$

Parameters:
```
frictionModel       kt;
ktCoeffs
{
    mu              mu [0 0 0 0 0 0 0] 0.25;       //dry friction coefficient

    chi             chi [0 -1 -2 0 0 0 0] 10000;   //turbulent friction coefficient
}
```

###### mu(I)

`muI`

Popular mu(I) following [Jop et al. (2006)](dx.doi.org/10.1038/nature04801) (see also [wikipedia.org](https://en.wikipedia.org/wiki/%CE%9C%28I%29_rheology))

Shear rate at the base following Bagnold Profile:

$ \dot{\gamma} = \dfrac{5}{2}\,\dfrac{|{\bf u}|}{h}$

Inertia number:

$ I = \dfrac{\dot{\gamma}\,d}{\sqrt{p/\rho_p}}$

Friction coefficient depending on inertia number:

$ \mu = \mu_s + \dfrac{\mu_2+\mu_s}{I_0/I+1}$

Basal friction:

$|{\boldsymbol \tau}| = \mu(I)\,p$

Parameters (see also tutorial `simpleslope`):
```
frictionModel       MuI;
MuICoeffs
{
    d               d [ 0 1 0 0 0 0 0] 0.005;        //particle diameter

    rho_p           rho_p [ 1 -3  0 0 0 0 0 ] 2500.; //particle density

    mu_s            mu_s [0 0 0 0 0 0 0 ] 0.38;      //friction coefficient (low limit)

    mu_2            mu_2 [0 0 0 0 0 0 0 ] 0.65;      //friction coefficient (high limit)

    I_0             I_0 [0 0 0 0 0 0 0 ] 0.30;       //reference inertia number
}
```
###### Poliquen Forterre (2008)

`PoliquenForterre`

First mu(I)-rheology following [Pouliquen & Forterre (2002)](dx.doi.org/10.1017/S0022112001006796). Similar to `muI`, however with the parameterisation of [Pouliquen & Forterre (2002)](dx.doi.org/10.1017/S0022112001006796).
See also [Johnson and Gray, (2011)](http://www.maths.manchester.ac.uk/~ngray/Papers/JFM_675_2011.pdf).

$Fr = \dfrac{|{\bf u}|}{\sqrt{h\,g_n}}$

$h_{s} = h\dfrac{\beta}{Fr}$

$\mu_{stop} = \tan(\zeta_1)+\dfrac{\tan(\zeta_2)-\tan(\zeta_1)}{1+h_s/L} $

$\mu_{start} = \tan(\zeta_3)+(\tan(\zeta_2)-\tan(\zeta_1))\,\exp(-h_s/L)$

$\mu = \left(\dfrac{Fr}{\beta}\right)^{\gamma} (\mu_{stop}-\mu_{start})+\mu_{start}$

$|{\boldsymbol \tau}| = \mu(I)\,p$

Parameters:
```
frictionModel       PoliquenForterre;
PoliquenForterreCoeffs
{
    L               L [ 0 1 0 0 0 0 0 ] 0.010;

    zeta1           zeta1 [ 0 0 0 0 0 0 0 ] 21;

    zeta2           zeta2 [ 0 0 0 0 0 0 0 ] 30.7;

    zeta3           zeta3 [ 0 0 0 0 0 0 0 ] 22.2;

    beta            beta [ 0 0 0 0 0 0 0 ] 0.136;

    gamma           gamma [0 0 0 0 0 0 0] 1e-3;
}
```

#### Entrainment models

Entrainment describes the erosion of the intact snow cover and intake of the respective material $S_e\,(\mathrm{m/s})$ into the avalanche. It is assumed that the snow cover has the same density as the avalanche.

The entrainment model has to be set in the file `constant/transportProperties` (see below).

Entrainment models can be found in the folder `src/avalanche/entrainment`. Currently there are the following available:

* entrainmentOff
* Front
* Erosionenergy
* Medina
* Ramms

###### No entrainment

`entrainmentOff`

Choose to turn off entrainment ($S_e = 0$), no parameters.

###### Front

`Front`

Simple front entrainment. Entrainment of the total mountain snow cover within a cell is triggered when $h > h_{trigger}$.

Parameters:
```
entrainmentModel    Front;
FrontCoeffs
{
    htrigger        htrigger [ 0 1 0 0 0 0 0 ] 0.01;
}
```

###### Erosionenergy

`Erosionenergy`

Entrainment model following SamosAT, see e.g., [Rauter et al. (2016)](https://www.nat-hazards-earth-syst-sci.net/16/2325/2016/).

The entrainment rate $S_e$ is calculated as

$S_e = \dfrac{{\boldsymbol \tau}\cdot{\bf u}}{\rho\,e_b} $

Parameters:
```
entrainmentModel    Erosionenergy;
ErosionenergyCoeffs
{
    eb              eb [0 2 -2 0 0 0 0] 11500;      //specific erosion energy
}
```

###### Medina

`Medina`

Entrainment model following the approach of [Medina et al. (2008)](https://doi.org/10.1007/s10346-007-0102-3).
Entrainment is calculated from stability considerations of the basal layer.

The entrainment rate $S_e$ is calculated as

$S_e = \dfrac{|\tau_b| - \tau_r}{\rho\,(|g_n|-\mu|g_s|)}$,

with the resisting shear strength

$\tau_r = \tau_c + \mu\,p_b$.

The cohesion $\tau_c$ and the friction coefficient $\mu$ of the basal layer are input parameters.
To improve stability, entrainment can be relaxed:
$S_e = \text{relaxation Factor} \cdot S_{m,\text{trial}}$.

Parameters:
```
entrainmentModel    Medina;
MedinaCoeffs
{
    tauc            tauc [1 -1 -2 0 0 0 0]        100;

    mu              mu [0 0 0 0 0 0 0]            0.45;

    relax 0.1;
}
```

###### Ramms

`Ramms`

Entrainment model following the approach from RAMMS, see e.g., [Christen et al. (2016)](http://dx.doi.org/10.1016/j.coldregions.2010.04.005).

The entrainment rate $S_e$ is calculated as

$S_e = \kappa\,|\mathbf{\overline{u}}|$.

Parameters:
```
entrainmentModel    Ramms;
RammsCoeffs
{
    kappa           kappa [0 0 0 0 0 0 0]       0.001;
}
```

#### Deposition models

Deposition takes into account that mass is gradually lost in the avalanche during deceleration.

The deposition model has to be set in the file `constant/transportProperties` (see below).

Deposition models can be found in the folder `src/avalanche/deposition` (because of the similarity to entrainment). Currently there is one deposition model available:

###### No deposition

`depositionOff`

Choose to turn off deposition.

###### Stoppingprofile

`Stoppingprofile`
A deposition model derived from a decelerating velocity profile.

Rauter and Köhler (2019), "Constraints on entrainment and deposition models in avalanche simulations from high-resolution radar data", Geosciences, 2019

Deposition model based on a decelerating Bagnold profile.

$S_{d} = \dfrac{a}{|\overline{\mathbf{u}}|}\,\dfrac{\mathrm{d}\,(h|\overline{\mathbf{u}}|)}{\mathrm{d} t}$

with

$
a = \left\{\begin{array}{clc}
\left(\dfrac{u_{dep}-|\mathbf{\overline{u}}|}{u_{dep}}\right)^{a_{dep}} & \text{for} & |\mathbf{\overline{u}}| \leq u_{dep} \  \land \  \dfrac{\mathrm{d}|\mathbf{\overline{u}}|}{\mathrm{d} t} < 0\\
0 & \text{else} &
\end{array}\right.$

and the parameter $u_{dep}$ and $a_{dep}$.


Parameters:
```
depositionModel     Stoppingprofile;
StoppingprofileCoeffs
{
    ud              ud [0 1 -1 0 0 0 0]         1.5;

    ad              ad [0 0  0 0 0 0 0]         1.0;
}
```


### faParkerFukushimaFoam

This solver implements the Turbidity Current model of [Parker et al. (1986)](https://doi.org/10.1017/S0022112086001404).
It can be found in `applications/solver/faParkerFukushimaFoam`. It can be called with `faParkerFukushimaFoam`.

$\dfrac{\partial\, h}{\partial t} + \nabla\cdot\left(h\,\mathbf{\overline{u}}\right) = e^{(w)}$
$\dfrac{\partial\, h\,\mathbf{\overline{u}}}{\partial t} + \nabla_{\mathrm{s}}\cdot\left(h\,\mathbf{\overline{u}}\otimes\mathbf{\overline{u}}\right) = R\,\mathbf{g}_{\mathrm{s}}\,\overline{c}\,h-\dfrac{1}{2}\,R\,g_{\mathrm{n}}\,\nabla_{\mathrm{s}}\left(\overline{c}\,h^2\right)-\boldsymbol{\tau}_{\mathrm{b}}$
$\dfrac{\partial\, \overline{c}\,h}{\partial t}  + \nabla\cdot\left(\overline{c}\,h\,\mathbf{\overline{u}}\right) = e^{(s)}-d^{(s)}$

with **unknown flow fields**
* depth-averaged flow velocity $\mathbf{\overline{u}}$,
* flow thickness $h$ and
* depth-averaged sediemnt concentration $\overline{c}$.

**Constant parameters** are
* the density ration between sediment and water $R$ and
* the gravitational acceleration $\mathbf{g} = \mathbf{g}_{\mathrm{s}}+\mathbf{g}_{\mathrm{n}}$.

**Non-user-selectedable models** are used for
* the water entrainment rate $e^{(w)}$:
$e^{(w)} = \dfrac{0.00153}{0.0204+Ri}|\mathbf{\overline{u}}|$
with
$Ri = \dfrac{R\,g_{\mathrm{n}}\,\overline{c}\,h}{|\mathbf{\overline{u}}|^2}$
* the sediment deposition rate $d^{(s)}$:
$d^{(s)} = v_{\mathrm{s}}\,r_0\,\overline{c}$
with
$r_0 = 1+31.5\,\mu^{-1.46},$
$\mu = \dfrac{\sqrt{|\boldsymbol{\tau}_{\mathrm{b}}|}}{v_{\mathrm{s}}}$
$v_{\mathrm{s}} = \dfrac{R\,g_{\mathrm{n}}\,d_{\mathrm{s}}}{18\,\nu}$

**User-selectable models** (see below) have to be provided for
* the basal friction $\boldsymbol{\tau}_{\mathrm{b}}$ and
* the volumetric sediment entrainment rate $e^{(s)}$

**Initial conditions and boundary conditions** have to be provided for
* the flow thickness $h$,
* the depth-integrated flow velocty $\mathbf{\overline{u}}$ and
* the depth-integrated sediment concentration $\overline{c}$
* the entrainable sediment thickness $h_{\mathrm{entrain}}$

#### Friction models (suspensionFrictionModels)

The friction model describes the friction $\boldsymbol{\tau}_{\mathrm{b}}$ between flowing mass (turbidity current) and the bottom. The direction of the friction vector always aligns with the velocity vector $\mathbf{\overline{u}}$.

The friction model has to be set in the file `constant/transportProperties` (see below).
Friction models can be found in the folder `src/avalanche/friction`.
To implement a new friction model, copy an existing one, rename it and modify it.

Currently there are the following friction models available:
* laminar (Parker et al., 1986: 3-Equations model)
* turbulent (Parker et al., 1986: 4-Equations model)

###### laminarSuspension

`laminarSuspension`

[Parker et al. (1986)](https://doi.org/10.1017/S0022112086001404).

This friction model can be used to get the 3-Equation model of Parker et al. (1987).

$|\boldsymbol{\tau}_{\mathrm{b}}|  = c_{\mathrm{D}}\,|\mathbf{\overline{u}}|\,\mathbf{\overline{u}}$

```
suspensionFrictionModel laminarSuspension;
laminarSuspensionCoeffs
{
    cd      cd      [0 0 0 0 0 0 0] 0.0006;
}
```

###### turbulentSuspension

`turbulentSuspension`

[Parker et al. (1986)](https://doi.org/10.1017/S0022112086001404).

This friction model can be used to get the 4-Equation model of Parker et al. (1987).

$|\boldsymbol{\tau}_{\mathrm{b}}|  =  \alpha\,\overline{k}$

with the depth-averaged kinetic energy $\overline{k}$, calculated with the PDE

$\dfrac{\partial\,\overline{k}\,h}{\partial t}  + \nabla\cdot\left(\overline{k}\,h\,\mathbf{\overline{u}}\right) = \boldsymbol{\tau}_{\mathrm{b}}\cdot\mathbf{\overline{u}}+\dfrac{1}{2}|\mathbf{\overline{u}}|^2\,e^{(w)}-\epsilon\,h-R\,g_{\mathrm{n}}\,v_{\mathrm{s}}\,\overline{c}\,h-\dfrac{1}{2}R\,g_{\mathrm{n}}\,h\left(e^{(s)}-d^{(s)}\right)$

with

$\epsilon = \beta\dfrac{\overline{k}^{3/2}}{h}$

$Ri = \dfrac{R\,g_{\mathrm{n}}\,\overline{c}\,h}{|\mathbf{\overline{u}}|^2}$

$\beta$ is an **optional parameter**. If not given, it will be calculated as

$\beta = \dfrac{2\,\dfrac{e^{(w)}}{\mathbf{\overline{u}}}\left(1-Ri-2\,\dfrac{c_{\mathrm{D}}}{\alpha}\right)+c_{\mathrm{D}}}{\left(\dfrac{c_{\mathrm{D}}}{\alpha}\right)^{3/2}}$


```
suspensionFrictionModel turbulentSuspension;
turbulentSuspensionCoeffs
{
    cd      cd      [0 0 0 0 0 0 0] 0.01;

    alpha   alpha   [0 0 0 0 0 0 0] 0.1;

    beta    beta    [0 0 0 0 0 0 0] 0.7;

    R       R       [ 0 0 0 0 0 0 0] 1.65;

    Ds      Ds      [ 0 1 0 0 0 0 0] 0.00005;

    kmin    kmin    [0 2 -2 0 0 0 0] 1e-7;

    nu      nu      [0 2 -1 0 0 0 0] 1e-6;
}
```

#### Entrainment models


The entrainment model has to be set in the file `constant/transportProperties` (see below).

Entrainment models can be found in the folder `src/avalanche/entrainment`. Currently there are the following available:

* ParkerFukushimaEntrainment

###### ParkerFukushimaEntrainment

`ParkerFukushimaEntrainment`

[Parker et al. (1986)](https://doi.org/10.1017/S0022112086001404).

This model implements the standard entrainment model as used by Parker et al. (1986).

$e^{(s)} = v_{\mathrm{s}}\,\begin{cases}
0.3 &\text{for } Z > Z_{\r{m}},\\[8pt]
3\cdot10^{-12}\,Z^{10}\left(1-\dfrac{Z_{\mathrm{c}}}{Z}\right)&\text{for }  Z_{\mathrm{c}} < Z < Z_{\mathrm{m}},\\[8pt]
0 &\text{for } Z < Z_{\mathrm{c}},
\end{cases}$

with

$Z = \sqrt{R_{\mathrm{s}}}\,\mu$

$R_{\mathrm{s}} = \dfrac{\sqrt{R\,g_{\mathrm{n}}\,d_{\mathrm{s}}^3}}{\nu}$

$\mu = \dfrac{\sqrt{|\boldsymbol{\tau}_{\mathrm{b}}|}}{v_{\mathrm{s}}}$

$v_{\mathrm{s}} = \dfrac{R\,g_{\mathrm{n}}\,d_{\mathrm{s}}}{18\,\nu}$


```
suspensionEntrainmentModel ParkerFukushimaEntrainment;
ParkerFukushimaEntrainmentCoeffs
{
    R       R       [ 0 0 0 0 0 0 0] 1.65;

    Ds      Ds      [ 0 1 0 0 0 0 0] 0.00005;

    Zc      Zc      [ 0 0 0 0 0 0 0] 0.5;

    Zm      Zm      [ 0 0 0 0 0 0 0] 13.2;

    nu      nu      [0 2 -1 0 0 0 0] 1e-6;
}
```

## functionObjects

Various functionObjects are available and can be added to the execution of a solver to extend its capabilities. They are added in the subDict `functions` in `system/controlDict`:

```
functions
{
    gridfileWrite
    {
        type        gridfileWrite;
        ...
        ...
    }
}
```

### shapefileWrite

This functionObject writes one shapefile in each timestep that is marked for writing. The shapefile contains a selection of OpenFOAM fields and can be openend in e.g. QGSI.

Parameters:

* `fields`: A list of fields that will be added to the shapefile. Accepts regular expressions, e.g. ".*" to export all fields.
* `writeOption`: `autoWrite`/`anyWrite`. Write only fields that are marked by the solver for writing or wirte any fields in the given list.
* `prefix`: The first part of the filename. Will be extended with the time.
* `offset`: Mapping back to world coordinates from simulation coordinates (see gridToSTL/releaseAreaMapping)

```
shapefileWrite
{
    type    shapefileWrite;
    libs    (faAvalanche);

    //fields     (".*"); //All fields
    fields      (h Us);

    writeOption autoWrite;
    //writeOption anyWrite;

    prefix  "polys";

    offset (5000.0 -220000.0 0);
}
```

### gridfileWrite

This functionObject generates gridfiles out of fields in each timestep that is marked for writing.

The parameters of the gridfile can be given in 3 ways:

* standard parameters `dx`, `dy`, `ncols`, `nrows`, `xllcorner`, `yllcorner`.
* grid-extends and cellsize `xmin`, `xmax`, `ymin`, `ymax`, `dx`, `dy`.
* cellsize  `dx`, `dy` and the extends will be calculated automatically from the mesh.

Parameters:

* `fields`: A list of fields that will be written as grid file. Accepts regular expressions, e.g. ".*" to export all fields.
* `writeOption`: `autoWrite`/`anyWrite`. Write only fields that are marked by the solver for writing or wirte any fields in the given list.
* `dx`: Cell size of the gridfile - x-direction
* `dy`: Cell size of the gridfile - y-direction
* `ncols`: Number of columns in the gridfile (optional)
* `nrows`: Number of rows in the gridfile (optional)
* `xllcorner`: Position of lower left cell in gridfile (optional)
* `yllcorner`: Position of lower left cell in gridfile (optional)
* `xmin`: extend in x-direction (optional)
* `xmax`: extend in x-direction (optional)
* `ymin`: extend in y-direction (optional)
* `ymax`: extend in y-direction (optional)
* `postfix`: The postfix of the files.
* `offset`: Mapping back to world coordinates from simulation coordinates (see gridToSTL/releaseAreaMapping)


```
gridfileWrite
{
    type    gridfileWrite;
    libs    (faAvalanche);

    fields  (h Us);

    secondOrder on;

    writeOption autoWrite;
    //writeOption anyWrite;

    ncols        426;
    nrows        258;
    xllcorner    -803;
    yllcorner    -83;
    dx           10;
    dy           10;

    postfix  ".asc";

    offset (5000.0 -220000.0 0);
}
```

### autoAreaToVolumeMapping

The native Paraview reader is not able to read areaFields. This functionObjects maps areaFields to volumeFields so Paraview can read them.

Parameters:

* `fields`: A list of fields that will be written as grid file. Accepts regular expressions, e.g. ".*" to export all fields.
* `writeOption`: `autoWrite`/`anyWrite`. Write only fields that are marked by the solver for writing or wirte any fields in the given list.
* `prefix`: prefix for filenames so they don't overwrite the areaFields.

```
autoAreaToVolumeMapping
{
    type    autoAreaToVolumeMapping;
    libs    (faAvalanche);

    fields  (".*");

    writeOption autoWrite;
    //writeOption anyWrite;

    prefix  "fa_";
}
```


## Utilities

### slopeMesh

`slopeMesh`

This simple utility can be used to create simple slopes as used in many of the tutorials.

For an example see `tutorials/simpleslope/`.
Parameters are set in `constant/slopeMeshDict`.
The source code can be found in `applications/utilities/slopeMesh`.

### gridToSTL

This utility allows to generate STL-files from topography data (ESRI grid files). STL-files can then be used to generate the mesh required for simulations.

For an example see `tutorials/wolfsgrube`.
Parameters are set in `system/gridToSTLDict`.
The source code can be found in `applications/utilities/slopeMesh`.

Example reading topography from grid file (*.asc) and boundary polygon from shape file (*.shp, *.shx, *.dbf)

```
stlname "constant/surface.stl";          //output file

gridname "constant/gisdata/dem.asc";     //topography file

boundary fromShape;                      //read boundary of the meshed area from shapefile

shapeBoundary "constant/gisdata/aoi";    //shapefile containing the boundary polygon

divisions 300;                           //number of points a polygon side is divided into

domainHeight 500.0;                      //height of the volume mesh

offset (5000.0 -220000.0 0);             //offset the final mesh by this value
```

Example reading topography from grid file (*.asc) and boundary polygon from dictionary

```
stlname "constant/surface.stl";          //output file

gridname "constant/gisdata/dem.asc";     //topography file

boundary fromPoints;                     //read boundary of the meshed area from list below

boundaryPoints                           //list with vertices describing the meshed area
(
    (-4666.57 221593 0)
    (-4259.78 222062 0)
    (-3749.64 221983 0)
    (-3020.48 220056 0)
    (-3642.49 220017 0)
);

divisions 300;                           //number of points a polygon side is divided into

domainHeight 500.0;                      //height of the volume mesh

offset (5000.0 -220000.0 0);             //offset the final mesh by this value
```


### releaseAreaMapping

`releaseAreaMapping`

This utility can be used to set the initial condition (e.g. the initial release zone). This utility reads input from the file `constant/releaseArea`.
Usually the initial condition is set on `0/h` and `0/hentrain`.
The initial velocity `0/Us` is usually set to `constant (0,0,0)`.

Three following forms of release areas can be created:
 * Sphere
 * Polygon
 * Shapefile

##### Sphere

`sphere`

**Spherical release area**, as often used for small scale experiments.
For an example see `tutorials/releaseAreaMapping`.
Setting a spherical mass on a slope:
```
fields
{
    h                                      //name of the field
    {
        default 0;                         //default value outside the release area
        regions                            //regions can contain many release areas
        (
            sphereExample                  //name of the release area
            {
                type       sphere;         //type sphere for spherical release
                center     (2.0 0.0 3.5);  //centre of the sphere
                r          2.0;            //radius of the sphere
                scale      1.0;            //scale up/down the resulting field
            }
        );
    }
}
```

##### Polygon

`polygon`

**Classic slab release**.
For an example see `tutorials/releaseAreaMapping` and `tutorials/entrainment`.
Setting a slab release on a slope:
```
fields
{
    h                                     //name of the field
    {
        default 0;                        //default value outside the release area
        regions                           //regions can contain many release areas
        (
            PolygonWithConstantValue      //name of the release area
            {
                type polygon;             //type polygon for a simple slab
                filltype constant;        //filltype: either constant or linear
                vertices                  //list of vertices defining the polygon
                (
                    (0 0 0)
                    (2 0 0)
                    (2 2 0)
                    (0 2 0)
                );
                value 0.5;                //value within the polygon (filltype constant)
                offset (5 13 0);          //offset of the polygon
                projectToNormal no;       //convert from vertical height to thickness
            }

            PolygonWithLinearValue        //name of the release area
            {
                type polygon;             //type polygon for a simple slab
                filltype linear;          //filltype: either constant or linear
                vertices                  //list of vertices defining the polygon
                (
                    (0 0 0)
                    (2 0 0)
                    (2 2 0)
                    (0 2 0)
                );
                valueAtZero 0.2;          //value if field at (x0, y0, z0)
                x0 15.;                   //reference point x-coordinate
                y0 13.;                   //reference point y-coordinate
                z0 0.;                    //reference point z-coordinate
                dfdx 0.1;                 //change of field with x
                dfdy 0.1;                 //change of field with y
                dfdz 0.;                  //change of field with z
                offset (15 13 0);         //offset of the polygon
                projectToNormal no;       //convert from vertical height to thickness
            }
        );
    }
}
```

##### Shapefile

`shapefile`

**Classic slab release**.
For an example see `tutorials/releaseAreaMapping` and `tutorials/wolfsgrube`.
Setting a slab release on a slope:
```
fields
{
    h                                     //name of the field
    {
        default 0;                        //default value outside the release area
        regions                           //regions can contain many release areas
        (
            ShapefileWithEmbeddedValues   //name of the release area
            {
                type shapefile;           //type shapefile to read POLYGON from an ESRI shapefile
                filltype shapefile;       //fill the polygon. filltype shapefile reads values from shapefile
                filename "data/h0";       //filename of the shapefile without filetype
                fieldname "h0";           //the name of the field in the shapefile which is used for filling
                offset (0 5 0);           //offset of the polygon
                projectToNormal no;       //convert from vertical height to thickness
            }

            ShapefileWithConstantValue    //name of the release area
            {
                type shapefile;           //type shapefile to read POLYGON from an ESRI shapefile
                filltype constant;        //filltype constant fills the polygon with a constant value
                filename "data/h0";       //filename of the shapefile without filetype
                value 0.4;                //constant value to fill the polygin
                offset (0 10 0);          //offset of the polygon
                projectToNormal no;       //convert from vertical height to thickness
            }

            ShapefileWithLinearValue     //name of the release area
            {
                type shapefile;           //type shapefile to read POLYGON from an ESRI shapefile
                filltype linear;          //filltype constant fills the polygon with a linear value
                filename "data/h0";       //filename of the shapefile without filetype
                valueAtZero 0.1;          //value if field at (x0, y0, z0)
                x0 7;                     //reference point x-coordinate, other coordinates are 0
                dfdx 0.05;                //change of field with x, dfdy = dfdz = 0
                offset (10 5 0);          //offset of the polygon
                projectToNormal no;       //convert from vertical height to thickness
            }
        );
    }
}
```

##### Rasterfile

`shapefile`

**Import of rasterfiles**.

```
fields
{
    h
    {
        default default [0 1 0 0 0 0 0] 0;
        regions
        (
            RasterfileNN
            {
                type rasterfile;
                interpolation nearestneighbor;
                filename "data/h0.asc";
                offset (0 0 0);
            }
    }
}
```


The source code can be found in `applications/utilities/releaseAreaMapping`.


## Solver Settings

### Simulation Time, Timesteps

Simulation time and time stepping controls can be found in the file `system/control`. See [OpenFOAM User Manual](https://cfd.direct/openfoam/user-guide/controldict/) for details. Most important settings:
* `endTime`: Time until the solver runs
* `writeInterval`: Intervalls at which results are saved
* `maxCo`: Maximum Courant-number. Recommended value is 1.

### Initial and Boundary Conditions

Initial and boundary conditions can be set in the files `0/h`, `0/Us` and `0/hentrain`, similar as in other OpenFOAM solver. The tool `ReleaseAreaMapping` can be used to create appropriate initial conditions.

### Transport Properties

Most physical constants can be found in `constant/transportProperties`. Example:

```
/*--------------------------------*- C++ -*----------------------------------*\
| =========                 |                                                 |
| \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox           |
|  \\    /   O peration     | Version:  avalanche                             |
|   \\  /    A nd           | https://develop.openfoam.com/Community/avalanche|
|    \\/     M anipulation  |                                                 |
\*---------------------------------------------------------------------------*/
FoamFile
{
    version     2.0;
    format      ascii;
    class       dictionary;
    object      transportProperties;
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

pressureFeedback    off;                            //turn curvature and lateral pressure interaction on/off

explicitDryAreas    on;                             //eliminate dry cells in the linear system

hmin                hmin [ 0 1 0 0 0 0 0]    1e-5;  //binding the height

rho                 rho [ 1 -3  0 0 0 0 0 ]  200.;  //avalanche/snow cover density

u0                  u0 [ 0 1 -1 0 0 0 0]     1e-4;  //small value for the velocity used in divisions

h0                  h0 [ 0 1 0 0 0 0 0]      1e-4;  //small value for the flow height used in divisions

xi                  xi [ 0 0 0 0 0 0 0]      1;     //shape factor for the velocity profile

frictionModel       Voellmy;                        //define the friction model

entrainmentModel    Erosionenergy;                  //define the entrainment model

depositionModel     depositionOff;                  //define the deposition model

VoellmyCoeffs                                       //parameter for the friciton model (Voellmy)
{
    mu              mu [0 0 0 0 0 0 0]       0.26;

    xi              xi [0 1 -2 0 0 0 0]      8650;
}

ErosionenergyCoeffs                                 //parameter for the entrainment moidel (Eroisionenergy)
{
    eb              eb [0 2 -2 0 0 0 0]      11500;
}

// ************************************************************************* //
```

### Numerical Schemes

See `system/faSchemes` and the [OpenFOAM User Manual](https://cfd.direct/openfoam/user-guide/fvSchemes/). Note that this solver is based on the Finite Area Method. Therefore, the numerical schemes are found in the file **fa**Schemes (instead of **fv**Schemes).

### Numerical Solver

See `system/faSolution` and the [OpenFOAM User Manual](https://cfd.direct/openfoam/user-guide/fvSolution/). Note that this solver is based on the Finite Area Method. Therefore, the numerical solution algorithms are found in the file **fa**Solution (instead of **fv**Solution).

## Tutorials

All tutorials contain a `Allrun` script which will conduct all steps required to run a simulation.


### simpleslope

Demonstrates:
* faSavageHutterFoam

Example from [Rauter and Tukovic (2018)](https://arxiv.org/abs/1802.05229), section 6.3. This example demonstrates a popular shallow granular flow test case on a simply curved slope.

### wolfsgrube

Demonstrates:
* faSavageHutterFoam
* natural terrain

Example from [Rauter et al. (2018)](https://doi.org/10.5194/gmd-11-2923-2018).

This tutorial shows the recalculation of a real scale avalanche in Tirol/Austria. Find out more about this avalanche in [Fischer et al. (2015)](https://www.researchgate.net/publication/286843194_Multivariate_parameter_optimization_for_computational_snow_avalanche_simulation).
This tutorial applies the solver and some tools of this package. Moreover it is using some python scripts which can be found in the folder `scripts`.


### montereycanyon

Demonstrates:
* faParkerFukushimaFoam
* natural terrain

Simplified example similar to [Rauter et al. (2019)](https://www.esi-group.com/sites/default/files/resource/other/8230/rauter_studentabstract_openfoam_2019.pdf) of a turbdity current in Monterey Canyon.
