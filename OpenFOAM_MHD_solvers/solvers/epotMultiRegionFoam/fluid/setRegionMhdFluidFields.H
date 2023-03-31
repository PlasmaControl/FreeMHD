volVectorField& JxB = fluidJxB[i];
volVectorField& J = fluidJ[i];
volVectorField& B = fluidB[i];
volScalarField& potE = fluidPotE[i];

dimensionedScalar elcond = fluidElCondValue[i];

const label potERefCell = potERefCellFluid[i];
const scalar potERefValue = potERefValueFluid[i];
