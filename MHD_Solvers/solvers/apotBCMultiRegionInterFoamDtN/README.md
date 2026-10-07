# apotBCMultiRegionInterFoamDtN

Multi-region vector-potential MHD solver based on `apotBCMultiRegionInterFoam`, with a componentwise spherical
Dirichlet-to-Neumann (DtN) closure for the exterior vacuum and several options for the induction path.

## Changes relative to apotBCMultiRegionInterFoam

- **Componentwise DtN closure** on a spherical outer boundary Gamma (`VSH_cartDtN`). Each Cartesian component
  of the induced vector potential on Gamma is projected onto the scalar spherical harmonics up to degree `VSH_L`
  (inclusive; degree 0 retained), and the decaying radial derivative -(l+1)/R is imposed, together with the
  tangential derivatives of the same expansion, as the boundary-normal gradient. This closes all three decaying
  vector-spherical-harmonic families admitted by the componentwise vacuum equation; on a compatible solenoidal
  trace it coincides, in the continuum, with the two-family closure. With `VSH_cartDtN false` the earlier
  two-family evaluation is used, for which `VSH_L` is an exclusive bound on the VSH degree.
- **Tabulated harmonics** (`VSH_cacheY`): the scalar harmonics at the fixed faces of Gamma are evaluated once and
  reused. With it on and off, regression runs gave bitwise-identical fields.
- **Sign fix** for the theta derivative of the spherical harmonics (`VSH_dThetaFix`); the default reproduces the
  earlier sign.
- **Closure on the conductor surface** without a vacuum mesh (`VSH_conductorDtN`): the operator acts on a conductor
  patch named `vacuumExterior` and is iterated with the vector-potential solve (`nABCiter`, `VSH_abcTol`).
- **Induction path for prescribed flows** (`VSH_fullAPath`): the fluid region uses the induction solve of
  `mhdAEqn.H`, with the source assembled as a face flux, instead of the `ePotEqn.H`/`aPotEqn.H` pair.
- **Potential source** (`VSH_potEdAdt`): the `ePotEqn.H` path includes the -sigma dA/dt term, with dA/dt frozen
  once per time step.
- **Settable boundary-loop tolerance** in the vacuum (`VSH_abcTol`, previously fixed at 1e-3).
- **Experimental options**, off by default: curl-curl vacuum equation (`VSH_vacuumCurlCurl`, `VSH_ccRelax`),
  vacuum-only Coulomb projection (`VSH_vacGaugeIter`), gradient form of the gauge cleaner (`VSH_gaugeGradForm`),
  and closure variants (`VSH_gaugeDecay`, `VSH_gainA`, `VSH_dropB`, `VSH_ortho`, `VSH_betaFit`).
- **Diagnostics**: VSH spectrum of the trace (`VSH_spectrum`), mismatch between the imposed and the freshly
  evaluated gradient (`VSH_mismatchEvery`), coefficient and loop reports (`VSH_abcReport`, `VSH_bcDiag`),
  manufactured-solution test modes (`VSH_mms`, `VSH_mmsFamily`, `VSH_mmsM`), and a report of the operator's CPU
  time.

## Build

Tested with OpenFOAM v2206 from the `OpenFOAM-v2206` tree of this repository, GCC 8.5.0 and Open MPI 4.1.0:

```bash
source OpenFOAM-v2206/etc/bashrc          # from the repository root
cd MHD_Solvers/solvers/apotBCMultiRegionInterFoamDtN
wmake
```

The executable is `apotBCMultiRegionInterFoamDtN` in `$FOAM_USER_APPBIN`.

## Running a case

```bash
decomposePar -allRegions -force
mpirun -np <N> apotBCMultiRegionInterFoamDtN -parallel > log.run 2>&1
```

The magnetic energy history is written to `magEnergy.dat`.

## Switches (system/controlDict)

The defaults keep the behaviour of earlier versions of this branch. **For the componentwise closure set
`VSH_enable`, `VSH_cartDtN` and `VSH_dThetaFix` to `true`.**

| switch | default | meaning |
|---|---|---|
| `VSH_enable` | `false` | apply the exterior operator on Gamma |
| `VSH_cartDtN` | `false` | componentwise closure (`false`: two-family evaluation) |
| `VSH_L` | `1` | highest scalar degree retained by the componentwise closure; must span the trace |
| `VSH_dThetaFix` | `false` | corrected sign of the theta derivative |
| `VSH_alpha` | `0.1` | under-relaxation of the imposed boundary gradient |
| `VSH_cacheY` | `true` | tabulate the boundary harmonics (componentwise closure) |
| `VSH_abcTol` | `1e-3` | exit tolerance of the boundary loop |
| `VSH_conductorDtN` | `false` | closure on the conductor patch `vacuumExterior`, no vacuum mesh |
| `VSH_fullAPath` | `false` | induction solve of `mhdAEqn.H` for prescribed flows |
| `VSH_potEdAdt` | `false` | -sigma dA/dt in the potential source of `ePotEqn.H` |

The outer-corrector count `nCorr` and the inner iteration cap `nAEiter` couple the regions; check convergence in
`nCorr` for each configuration.

## Known limitation

In conducting solid regions the solver advances the transient vector-potential equation and solves
`laplacian(sigma, potE) = 0`, omitting the source `-div(sigma dA/dt)` of the potential equation; the insulating
condition on the conductor surface is approached through a lagged boundary-value update. The velocity-bearing
path retains the source.

## Licence

MIT, as for the rest of this repository (see `LICENSE` at the top level).
