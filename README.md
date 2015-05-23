# QtCreator NimEditor plugin
A QtCreator plugin for editing Nim files

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
* make install

By default the plugin should be installed in your local QtCreator plugin dir.
In particular:
* on windows, "%LOCALAPPDATA%\QtProject\qtcreator"
* on linux, "$XDG_DATA_HOME/data/QtProject/qtcreator" or "~/.local/share/data/QtProject/qtcreator"
* on Mac, "~/Library/Application Support/QtProject/Qt Creator"
