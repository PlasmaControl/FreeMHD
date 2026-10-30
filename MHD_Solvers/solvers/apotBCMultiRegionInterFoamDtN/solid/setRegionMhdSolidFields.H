volScalarField& potE = solidPotE[i];
volScalarField& psi = solidPsi[i];
volVectorField& J = solidJ[i];
volVectorField& A = solidA[i];
volVectorField& dAdt = solidDADt[i];
volVectorField& curlA = solidCurlA[i];
volScalarField& divA = solidDivA[i];

volScalarField& elcond = solidElcond[i];
volScalarField& magMu = solidMagMu[i];

const label potERefCell = potERefCellSolid[i];
const scalar potERefValue = potERefValueSolid[i];
