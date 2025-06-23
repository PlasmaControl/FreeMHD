volVectorField& JxB = fluidJxB[i];
volVectorField& J = fluidJ[i];
volVectorField& A = fluidA[i];
volVectorField& B = fluidB[i];
volVectorField& B0 = fluidB0[i];
volScalarField& potE = fluidPotE[i];

dimensionedScalar elcond1 = fluidElCond1[i];
dimensionedScalar elcond2 = fluidElCond2[i];
volScalarField& elcond = fluidElCond[i];

surfaceScalarField& elcondF = fluidElcondF[i];
surfaceScalarField& psiub = fluidPsiub[i];
surfaceScalarField& jn = fluidJn[i];


dimensionedScalar magMu1 = fluidMagMu1[i];
dimensionedScalar magMu2 = fluidMagMu2[i];
volScalarField&   magMu = fluidMagMu[i];

volScalarField &alpha1f = fluidAlpha1f[i];

const label potERefCell = potERefCellFluid[i];
const scalar potERefValue = potERefValueFluid[i];
