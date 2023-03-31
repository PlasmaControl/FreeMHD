volVectorField& JxB = fluidJxB[i];
volVectorField& B = fluidB[i];
volVectorField& B0 = fluidB0[i];
volVectorField& J = fluidJ[i];
volScalarField& potE = fluidPotE[i];

surfaceScalarField& elcondF = fluidElcondF[i];
surfaceScalarField& psiub = fluidPsiub[i];
surfaceScalarField& jn = fluidJn[i];

dimensionedScalar elcond1 = fluidElCond1[i];
dimensionedScalar elcond2 = fluidElCond2[i];
volScalarField& elcond = fluidElCond[i];

volScalarField &alpha1f = fluidAlpha1f[i];


const label potERefCell = potERefCellFluid[i];
const scalar potERefValue = potERefValueFluid[i];
