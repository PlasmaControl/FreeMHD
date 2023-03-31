"""
A simple example of a Python simulation code working with Catalyst.
It depends on numpy and mpi4py being available. The environment
variables need to be set up properly to find Catalyst when running directly
from python. For Linux
and Mac machines they should be:
export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:<Catalyst build dir>/lib
export PYTHONPATH=<Catalyst build dir>/lib:<Catalyst build dir>/lib/site-packages

Alternatively, pvbatch or pvpython can be used which will automatically set up
system paths for using Catalyst.

When running, Catalyst scripts must be added in on the command line. For example:
</path/to/pvpython> fedriver.py cpscript.py
mpirun -np 4 </path/to/pvbatch> --sym fedriver.py cpscript.py
"""
import numpy
import sys
from mpi4py import MPI

comm = MPI.COMM_WORLD
rank = comm.Get_rank()

import fedatastructures

grid = fedatastructures.GridClass([10, 12, 10], [2, 2, 2])
attributes = fedatastructures.AttributesClass(grid)
doCoprocessing = True

if doCoprocessing:
    import coprocessor
    coprocessor.initialize()
    for i in sys.argv[1:]:
        if rank == 0:
            print('Using Catalyst script', i)
        coprocessor.addscript(i)

for i in range(100):
    attributes.Update(i)
    if doCoprocessing:
        coprocessor.coprocess(i, i, grid, attributes)

if doCoprocessing:
    coprocessor.finalize()
