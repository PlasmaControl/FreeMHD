volVectorField& JxB = fluidJxB[i];
volVectorField& B = fluidB[i];
volVectorField& B0 = fluidB0[i];
volVectorField& J = fluidJ[i];
volScalarField& potE = fluidPotE[i];

surfaceScalarField& elcondF = fluidElcondF[i];
surfaceScalarField& psiub = fluidPsiub[i];
surfaceScalarField& psid2t = fluidPsiD2t[i];
surfaceScalarField& jn = fluidJn[i];

dimensionedScalar elcond1 = fluidElCond1[i];
dimensionedScalar elcond2 = fluidElCond2[i];
volScalarField& elcond = fluidElCond[i];

dimensionedScalar seebcoeff1 = fluidSeebCoeff1[i];
dimensionedScalar seebcoeff2 = fluidSeebCoeff2[i];
volScalarField& seebcoeff = fluidSeebCoeff[i];

dimensionedScalar Qhf = heatFluxMagnitude[i];
volVectorField Qhf_dir = heatFluxDirection[i];
volScalarField Qhf_loc = heatFluxLocation[i];

volScalarField &alpha1f = fluidAlpha1f[i];

const label potERefCell = potERefCellFluid[i];
const scalar potERefValue = potERefValueFluid[i];


