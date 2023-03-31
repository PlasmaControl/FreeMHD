Build and Install
#################

Obtaining the source
********************

To obtain the Catalyst source locally, clone the official
code repository using `Git`_.

.. code-block:: bash

    git clone https://gitlab.kitware.com/paraview/catalyst.git


Building
********

Catalyst uses CMake to generate build system scripts and projects, such as
Makefiles or Ninja build files. While IDE generators (Xcode and Visual Studio)
are supported, `Ninja`_ is highly recommended.

To do a fresh build, start with an empty directory as follows:

.. code-block:: bash

    mkdir catalyst-build
    cd catalyst-build
    ccmake -G Ninja [path to catalyst source directory]

    # do the build
    ninja

    # optionally, run tests
    ctest

    # do the install
    ninja install


``ccmake`` is a graphical GUI that lets you specify various options for CMake.
Alternately, those options can be specified on command line to ``cmake`` using
``-Doption:type=value`` (or ``-Doption=value``) parameters as follows:

.. code-block:: bash

    cmake -G Ninja -DCATALYST_BUILD_TESTING:BOOL=ON ... [path to catalyst src dir]


Using ``-G Ninja`` results in CMake generating build files for Ninja. You can
switch to using any other supported generator of your choice. See `CMake Docs`_
for details. In that case, ``ninja`` will be replaced by the appropriate build
tool in the steps above.

Supported CMake Options
=======================

Important `CMake`_ options that affect how Catalyst is built are:

* ``CATALYST_BUILD_SHARED_LIBS`` (default: ``ON``): choose whether to build static
  or shared libraries for Catalyst. To support switching of Catalyst
  implementation at runtime, you must build with ``CATALYST_BUILD_SHARED_LIBS``
  set to ``ON`` (default).

* ``CATALYST_BUILD_STUB_IMPLEMENTATION`` (default: ``ON``): choose whether to build
  the stub Catalyst implementation. When building Catalyst only to develop
  another Catalyst API implementation, you may turn this option to ``OFF``. If
  ``OFF``, no ``catalyst`` library will be built.

* ``CATALYST_BUILD_TESTING`` (default: ``ON``): enable/disable testing. Running the
  tests using ``ctest`` after a build has succeeded is a good way to verify that
  your build is functional.

* ``CMAKE_BUILD_TYPE`` (default: ``Debug``): this is used to choose whether to add
  debugging symbols to the build. Supported values are ``Debug``, ``Release``,
  ``MinSizeRel``, and ``RelWithDebInfo``.

* ``CMAKE_INSTALL_PREFIX``: path where to install the libraries and headers when
  requested.


.. _`Git`: https://git-scm.com/
.. _`CMake`: https://cmake.org
.. _`CMake Docs`: https://cmake.org/documentation/
.. _`Ninja`:https://ninja-build.org
