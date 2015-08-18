# Nim QtCreator plugin
A QtCreator plugin for editing Nim programming language files.
At the moment the plugin offers:
* Basic syntax highlighting
* Basic automatic indentation
* Basic project manager

# Build Instructions and install
* Grab the QtCreator source code and build it
* Clone this repo somewhere in your system
* export the following variables
    * export QTC_SOURCE=path/to/qtcreator/source_dir
    * export QTC_BUILD=path/to/qtcreator/build_dir
* mkdir build
* cd build
* qmake ../path/to/nimeditor/source_dir
* make
* If everything works you should have the plugin available in QtCreator inside the QTC_BUILD directory
