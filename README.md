Oculus
=========
A KL extension for the Oculus Rift SDK

Oculus allows you to read the head tracking information from the Oculus Rift, and adds functionality to the InlineDrawing realtime rendering for two eyed warped display.

extension status
================

This extension has been built and tested against Fabric Engine 1.14. Higher release might work, but this hasn't been validated. Feel free to use this code for reference or training purposes, it's meant to be an example of how to integrate custom libraries into KL.

structure
=========

The Oculus extension uses the KL2EDK tool to generate the required C++ headers. Since the number of callbacks is quite small, they are handcrafted and don't use KL2EDK's advanced codegen functionality.

The latest KL2EDK documentation can be found here: http://documentation.fabricengine.com/FabricEngine/latest/HTML/ExtensionAuthoringGuide/kl2edk.html

supported platforms
===================

Oculus is currently only supported on Windows.
For Fabric Engine 1.14 the extension has to be built using Visual Studio 2010 (through scons). For future versions VS 2013 might be used.

building
========

A scons (http://www.scons.org/) build script is provided. Oculus depends on
* The Oculus Rift SDK (0.7.0 or higher)
* A static build of boost (1.55.0 or higher)

To inform scons where to find the Fabric Engine includes as well as the thirdparty libraries, you need to set the following environment variables:

* FABRIC_DIR: Should point to Fabric Engine's installation folder.
* OCULUS_DIR: Should point to the root of the Oculus SDK.
* BOOST_INCLUDE_DIR: Should point to Boost include folder.
* BOOST_LIBRARY_DIR: Should point to Boost library folder.

The temporary files will be built into the *build* folder, while the structured output files will be placed in the *stage* folder. Optionally you can define the FABRIC_EXTS_INSTALL_DIR environment variable, to which the extension will be installed.

Note: The fabric.sh exists purely for Fabric Engine internal use.

testing
=======

The extension repository also contains a series of unit tests, with which you can validate a successful build, or simply use them for reference on how to use the extension. to launch all unit tests simply use python like this:

    python test/test_all.py

known issues
============

The positional tracking sometimes may not work in cases where you have another camera (webcam) setup. In these kind of cases please experiment with disconnecting the camera or changing the USB ports so that the Oculus tracking camera is seen first by the USB controller.

samples
=======

The splice files in the samples folder can be run with the splice standalone command line tool.

Some samples might require the RazerHydra or the XBox extension, which are also available as public repositories.

license
==========

The license used for this extensions is the new BSD license. You can find it in the root folder of the repository.
