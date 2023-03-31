
pb = -friction->rho()*xi*fac::ndiv(phi2s, Us)&n;

if (pressureFeedback)
{
    pb -= fac::ngrad(pb*h/2.)&n;
}

pb += friction->rho()*(g*h)&n;

// Clipping
pb = Foam::max(pb, dimensionedScalar(dimPressure));

