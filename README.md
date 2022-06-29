Velo C Service Library
======================

This library extracts some of the more common service oriented components needed
for building C system services so they can be re-used between services.

Building
========

The [`vc-toolchain`][vc-toolchain-url] project needs to be installed in the
`/opt/vctoolchain` subdirectory.  If a different installation directory is used,
then the `TOOLCHAIN_DIR` environment variable should be set to that directory
instead.

[vc-toolchain-url]: https://github.com/VeloPayments/vc-toolchain

This project uses [Meson][meson-url] as the build tool and requires
[ninja][ninja-url] to perform the actual build.

[meson-url]: https://mesonbuild.org
[ninja-url]: https://ninja-build.org

To build (from this directory):

    mkdir build
    cd build
    meson ..
    ninja

To run the unit test suite:

    ninja test

To run the model checking suite:

    ninja model-check
