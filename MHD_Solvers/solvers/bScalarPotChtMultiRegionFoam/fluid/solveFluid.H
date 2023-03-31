if (finalIter)
{
    mesh.data::add("finalIteration", true);
}

	#include "bPotEqn.H"


if (finalIter)
{
    mesh.data::remove("finalIteration");
}
