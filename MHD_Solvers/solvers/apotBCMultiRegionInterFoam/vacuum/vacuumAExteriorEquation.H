//Foam::vector sum_A_boundary(0.0, 0.0, 0.0);
for(int j=0; j<2*L*L; j++)
{
	a_lm_x_Re[j] = 0.;
	a_lm_x_Im[j] = 0.;
	a_lm_y_Re[j] = 0.;
	a_lm_y_Im[j] = 0.;
	a_lm_z_Re[j] = 0.;
	a_lm_z_Im[j] = 0.;
}
forAll(mesh.boundary(), patch)
{
	if (patch != mesh.boundary().findPatchID("vacuumExterior"))
	{
		continue;
	}
	forAll(mesh.boundary()[patch], face)
	{
		const vector& Cf = mesh.boundary()[patch].Cf()[face];
		const scalar xf = Cf.x();
		const scalar yf = Cf.y();
		const scalar zf = Cf.z();
		const scalar x = xf - mesh.boundary()[patch].delta().ref()[face].x();
		const scalar y = yf - mesh.boundary()[patch].delta().ref()[face].y();
		const scalar z = zf - mesh.boundary()[patch].delta().ref()[face].z();
		const scalar r = Foam::sqrt(x*x + y*y + z*z);
		const scalar theta = Foam::acos(z / r);
		const scalar phi = Foam::atan2(y, x);

		scalar A_at_cntr_x = A.boundaryField().boundaryInternalField()[patch][face][0];
		scalar A_at_cntr_y = A.boundaryField().boundaryInternalField()[patch][face][1];
		scalar A_at_cntr_z = A.boundaryField().boundaryInternalField()[patch][face][2];

		for(int l = 0; l < L; l++)
		{
			for(int m=-l; m<=l; m++)
			{
				scalar Y_lm_Re = EvalSHSlow_Re(l, m, phi, theta);
				scalar Y_lm_Im = EvalSHSlow_Im(l, m, phi, theta);
				a_lm_x_Re[2*L*l+m+L] += A_at_cntr_x * Y_lm_Re * mesh.boundary()[patch].magSf()[face] / r / r;
				a_lm_x_Im[2*L*l+m+L] -= A_at_cntr_x * Y_lm_Im * mesh.boundary()[patch].magSf()[face] / r / r;
				a_lm_y_Re[2*L*l+m+L] += A_at_cntr_y * Y_lm_Re * mesh.boundary()[patch].magSf()[face] / r / r;
				a_lm_y_Im[2*L*l+m+L] -= A_at_cntr_y * Y_lm_Im * mesh.boundary()[patch].magSf()[face] / r / r;
				a_lm_z_Re[2*L*l+m+L] += A_at_cntr_z * Y_lm_Re * mesh.boundary()[patch].magSf()[face] / r / r;
				a_lm_z_Im[2*L*l+m+L] -= A_at_cntr_z * Y_lm_Im * mesh.boundary()[patch].magSf()[face] / r / r;
			}
		}
	}
}

reduce(a_lm_x_Re, sumOp<scalarField>());
reduce(a_lm_x_Im, sumOp<scalarField>());
reduce(a_lm_y_Re, sumOp<scalarField>());
reduce(a_lm_y_Im, sumOp<scalarField>());
reduce(a_lm_z_Re, sumOp<scalarField>());
reduce(a_lm_z_Im, sumOp<scalarField>());

forAll(mesh.boundary(), patch)
{
	if (patch != mesh.boundary().findPatchID("vacuumExterior"))
	{
		continue;
	}
	forAll(mesh.boundary()[patch], face)
	{
		scalar ax = 0;
		scalar ay = 0;
		scalar az = 0;
		scalar dAx = 0;
		scalar dAy = 0;
		scalar dAz = 0;
		const vector& Cf = mesh.boundary()[patch].Cf()[face];

		const scalar xf = Cf.x();
		const scalar yf = Cf.y();
		const scalar zf = Cf.z();
		const scalar dx = mesh.boundary()[patch].delta().ref()[face].x();
		const scalar dy = mesh.boundary()[patch].delta().ref()[face].y();
		const scalar dz = mesh.boundary()[patch].delta().ref()[face].z();
		const scalar x = xf - dx;
		const scalar y = yf - dy;
		const scalar z = zf - dz;
		const scalar r = Foam::sqrt(x*x + y*y + z*z);
		const scalar theta = Foam::acos(z / r);
		const scalar phi = Foam::atan2(y, x);

		for(int l=0; l<L; l++)
		{
			for(int m=-l; m<=l; m++)
			{
				scalar Y_lm_Re = EvalSHSlow_Re(l, m, phi, theta);
				scalar Y_lm_Im = EvalSHSlow_Im(l, m, phi, theta);
				dAx -= a_lm_x_Re[2*L*l+m+L] * Y_lm_Re * (l + 1) / r;
				dAx -= a_lm_x_Im[2*L*l+m+L] * Y_lm_Im * (l + 1) / r;
				dAy -= a_lm_y_Re[2*L*l+m+L] * Y_lm_Re * (l + 1) / r;
				dAy -= a_lm_y_Im[2*L*l+m+L] * Y_lm_Im * (l + 1) / r;
				dAz -= a_lm_z_Re[2*L*l+m+L] * Y_lm_Re * (l + 1) / r;
				dAz -= a_lm_z_Im[2*L*l+m+L] * Y_lm_Im * (l + 1) / r;
			}
		}
		scalar A_at_cntr_x = A.boundaryField().boundaryInternalField()[patch][face][0];
		scalar A_at_cntr_y = A.boundaryField().boundaryInternalField()[patch][face][1];
		scalar A_at_cntr_z = A.boundaryField().boundaryInternalField()[patch][face][2];
		const scalar dr = Foam::sqrt(dx*dx + dy*dy + dz*dz);

		fvPatchField<vector> &Av = A.boundaryFieldRef()[patch];
		Av[face].x() = A_at_cntr_x + dr * dAx;
		Av[face].y() = A_at_cntr_y + dr * dAy;
		Av[face].z() = A_at_cntr_z + dr * dAz;
	}
}
