volVectorField& JxB = fluidJxB[i];
volVectorField& J = fluidJ[i];
volVectorField& A = fluidA[i];
volVectorField& B = fluidB[i];
volVectorField& B0 = fluidB0[i];
volScalarField& potE = fluidPotE[i];

dimensionedScalar elcond = fluidElCondValue[i];
dimensionedScalar magMu = fluidMagMuValue[i];

const label potERefCell = potERefCellFluid[i];
const scalar potERefValue = potERefValueFluid[i];
