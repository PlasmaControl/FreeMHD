Catalyst for Implementation Developers
######################################

Developers can develop custom implementations for the Catalyst API to support
a wide variety of use-cases. In most cases, however, if your goal is to use
ParaView for in situ data processing, it may be easier to simply use
**ParaView Catalyst**. It support several ways for describing computational
meshes and field arrays including `Mesh Blueprint`_ and `ADIS`_.

This section describes the workflow for those who want to implement a custom
implementation for the Catalyst API.


Prerequisites
=============

* To build a custom Catalyst implementation, your project needs to be a
  CMake-based project i.e. use CMake as the build system generator. While
  it is technically feasible to use a non-CMake based project, it is highly
  recommended to prefer to use CMake.


CMake Setup
===========

The following sample CMakeLists.txt shows how to build a Catalyst
implementation.

.. code-block:: cmake
    :linenos:

    # When implementing the Catalyst API, as against using
    # it to invoke Catalyst, one must use the component
    # ``SDK`` in ``find_package`` call. This ensures that all necessary
    # libraries etc. are available.
    find_package(catalyst
                 REQUIRED
                 COMPONENTS SDK)

    # use this function call to create a Catalyst API implementation.
    catalyst_implementation(
      TARGET  MyCustomCatalystImpl
      NAME    MyImplName
      SOURCES MyCustomCatalystImpl.cxx)

That is it! ``catalyst_implementation`` creates the library with the appropriate
CMake target-properties on the library including setting its name and version
number. This function is only available when the ``SDK`` component is explicitly
requested in the ``find_package(catalyst .. )`` call.

Implementing Catalyst API
=========================

Providing an implementation for the Catalyst API implies providing code for the
five ``catalyst_`` functions that are part of the Catalyst API,
``catalyst_initialize_MyImplName``, ``catalyst_finalize_MyImplName``,
``catalyst_execute_MyImplName``, ``catalyst_about_MyImplName`` and
``catalyst_results_MyImplName``.

To do that, simply include ``catalyst.h`` and ``catalyst_impl_MyImplName.h``
headers in your implementation file and add definitions for these functions.
Definitions for all the four functions must be provided. You can choose to
invoke the default stub implementation for any of the functions by including
the ``catalyst_stub.h`` header and then calling ``catalyst_stub_initialize``,
``catalyst_stub_finalize``, ``catalyst_stub_execute``, ``catalyst_stub_about`` or
``catalyst_stub_results`` in your implementations for the corresponding methods.

If your custom implementation is using C++, you can include
``c/conduit_cpp_to_c.hpp`` headers to convert the ``conduit_node`` pointer to a
``conduit::Node`` instance pointer using ``conduit::cpp_node()``. Then you can use
the ``conduit::Node`` API which is generally friendlier than the C API.

.. code-block:: c++
    :linenos:

    #include <catalyst.h>
    #include <conduit.hpp>            // for conduit::Node
    #include <conduit_cpp_to_c.hpp>   // for conduit::cpp_node()

    ...

    enum catalyst_status catalyst_about_MyImplName(conduit_node* params)
    {
      // convert to conduit::Node
      conduit::Node &cpp_params = (*conduit::cpp_node(params));

      // now, use conduit::Node API.
      cpp_params["catalyst"]["capabilities"].append().set("adaptor0");
    }


On successful build of your project, you should get a shared library named
``libcatalyst-ImplName.so``, ``libcatalyst-ImplName.so``, or
``catalyst-ImplName.dll`` on Linux, macOS, and Windows respectively.

Using your Catalyst implementation
==================================

Now, to use your implementation with any simulation or code built with the stub
Catalyst implementation, all you need to do is to make sure your Catalyst
library is found and loaded by ``catalyst_initialize``.



.. _`Mesh Blueprint`: https://llnl-conduit.readthedocs.io/en/latest/blueprint_mesh.html#mesh-blueprint

.. _`ADIS`: https://gitlab.kitware.com/vtk/adis
