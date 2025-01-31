/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011-2017 OpenFOAM Foundation
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

\*---------------------------------------------------------------------------*/

#include "interfaceProperties.H"
#include "alphaContactAngleTwoPhaseFvPatchScalarField.H"
#include "mathematicalConstants.H"
#include "surfaceInterpolate.H"
#include "fvcDiv.H"
#include "fvc.H"
#include "fvcGrad.H"
#include "fvcSnGrad.H"
#include "unitConversion.H"

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

// Correction for the boundary condition on the unit normal nHat on
// walls to produce the correct contact angle.

// The dynamic contact angle is calculated from the component of the
// velocity on the direction of the interface, parallel to the wall.

void Foam::interfaceProperties::correctContactAngle
(
    surfaceVectorField::Boundary& nHatb,
    const surfaceVectorField::Boundary& gradAlphaf
) const
{
    const fvMesh& mesh = alpha1_.mesh();
    const volScalarField::Boundary& abf = alpha1_.boundaryField();

    const fvBoundaryMesh& boundary = mesh.boundary();

    forAll(boundary, patchi)
    {
        if (isA<alphaContactAngleTwoPhaseFvPatchScalarField>(abf[patchi]))
        {
            alphaContactAngleTwoPhaseFvPatchScalarField& acap =
                const_cast<alphaContactAngleTwoPhaseFvPatchScalarField&>
                (
                    refCast<const alphaContactAngleTwoPhaseFvPatchScalarField>
                    (
                        abf[patchi]
                    )
                );

            fvsPatchVectorField& nHatp = nHatb[patchi];
            const scalarField theta
            (
                degToRad() * acap.theta(U_.boundaryField()[patchi], nHatp)
            );

            const vectorField nf
            (
                boundary[patchi].nf()
            );

            // Reset nHatp to correspond to the contact angle

            const scalarField a12(nHatp & nf);
            const scalarField b1(cos(theta));

            scalarField b2(nHatp.size());
            forAll(b2, facei)
            {
                b2[facei] = cos(acos(a12[facei]) - theta[facei]);
            }

            const scalarField det(1.0 - a12*a12);

            scalarField a((b1 - a12*b2)/det);
            scalarField b((b2 - a12*b1)/det);

            nHatp = a*nf + b*nHatp;
            nHatp /= (mag(nHatp) + deltaN_.value());

            acap.gradient() = (nf & nHatp)*mag(gradAlphaf[patchi]);
            acap.evaluate();
        }
    }
}

void Foam::interfaceProperties::calculateK()
{

    const fvMesh& mesh = alpha1_.mesh();
    const surfaceVectorField& Sf = mesh.Sf();

    volScalarField smoothAlpha_ = alpha1_;
    
	scalar smoothItr_
    (
            alpha1_.mesh().solverDict(alpha1_.name()).get<scalar>("smoothItr")
    );
    scalar kSmoothItr_
    (
            alpha1_.mesh().solverDict(alpha1_.name()).get<scalar>("kSmoothItr")
    );
    
	Info<<"Calculating K with AlphaSmoothIters: "<<smoothItr_<<", KSmoothIters: "<<kSmoothItr_<<nl;
    
    for (int i = 0; i < smoothItr_; ++i)
	{
    	//Lafaurie smooth function
        smoothAlpha_ = fvc::average(fvc::interpolate(smoothAlpha_));
	}


    // Cell gradient of alpha
    volVectorField gradAlpha(fvc::grad(smoothAlpha_));

    // Interpolated face-gradient of alpha
    surfaceVectorField gradAlphaf(fvc::interpolate(gradAlpha));

    // Face unit interface normal
    surfaceVectorField nHatfv(gradAlphaf/(mag(gradAlphaf) + deltaN_));
    // surfaceVectorField nHatfv

    correctContactAngle(nHatfv.boundaryFieldRef(), gradAlphaf.boundaryField());

    // Face unit interface normal flux
    nHatf_ = nHatfv & Sf;

    // Simple expression for curvature
    K_ = -fvc::div(nHatf_);

	// Curvature smoothing
	if (kSmoothItr_ > 0)
	{
		volScalarField KsStar = K_;
		volScalarField Ks = K_;
		volScalarField kernel = 2.0*sqrt(mag(smoothAlpha_*(1.0-smoothAlpha_)));
		volScalarField w = sqrt(smoothAlpha_*(1.0 - smoothAlpha_) + 1e-6);
        for (int i = 0; i < kSmoothItr_; i++)
	    {
        	KsStar = fvc::average(fvc::interpolate(Ks*w))/fvc::average(fvc::interpolate(w));
	    	Ks = kernel*K_ + (1.0-kernel)*KsStar;
        }
		K_ = Ks;
	}

}



// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::interfaceProperties::interfaceProperties
(
    const volScalarField& alpha1,
    const volVectorField& U,
    const IOdictionary& dict
)
:
    transportPropertiesDict_(dict),
    cAlpha_
    (
        alpha1.mesh().solverDict(alpha1.name()).get<scalar>("cAlpha")
    ),

    sigmaPtr_(surfaceTensionModel::New(dict, alpha1.mesh())),

    deltaN_
    (
        "deltaN",
        1e-8/cbrt(average(alpha1.mesh().V()))
    ),
    

    alpha1_(alpha1),
    U_(U),

    nHatf_
    (
        IOobject
        (
            "nHatf",
            alpha1_.time().timeName(),
            alpha1_.mesh()
        ),
        alpha1_.mesh(),
        dimensionedScalar(dimArea, Zero)
    ),

    K_
    (
        IOobject
        (
            "interfaceProperties:K",
            alpha1_.time().timeName(),
            alpha1_.mesh()
        ),
        alpha1_.mesh(),
        dimensionedScalar(dimless/dimLength, Zero)
    )
{
    calculateK();
}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

Foam::tmp<Foam::volScalarField>
Foam::interfaceProperties::sigmaK() const
{
    return sigmaPtr_->sigma()*K_;
}


Foam::tmp<Foam::surfaceScalarField>
Foam::interfaceProperties::surfaceTensionForce() const
{
    return fvc::interpolate(sigmaK())*fvc::snGrad(alpha1_);
}

const Foam::surfaceVectorField& Foam::interfaceProperties::getNHatfv() const
{
    return nHatfv_;
}

const Foam::surfaceVectorField& Foam::interfaceProperties::faceUnitInterfaceNormal() const
{
    return nHatfv_;
}

Foam::tmp<Foam::volScalarField>
Foam::interfaceProperties::nearInterface() const
{
    return pos0(alpha1_ - 0.01)*pos0(0.99 - alpha1_);
}


void Foam::interfaceProperties::correct()
{
    calculateK();
}


bool Foam::interfaceProperties::read()
{
    alpha1_.mesh().solverDict(alpha1_.name()).readEntry("cAlpha", cAlpha_);
    sigmaPtr_->readDict(transportPropertiesDict_);

    return true;
}


// ************************************************************************* //
