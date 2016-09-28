Prototype of new GENIVI HMI designs for GDP 11
---------------------------------------------------

Prerequisities
---------------
General:
* Qt 5.6+ with QtQuick
* Plugins built (and installed)

Running on the Desktop
-----------
* Open Qt Creator
* Open Project and select gdp-hmi.pro from the top level of the repository
* On the first run Qt Creator will ask you to configure your project.  The default proposal will be a Desktop configuration using already installed compilers, so you should probably be able to just hit "OK".
* In the Projects tab on the left, if not still there, make sure to disable Shadow build. This messes with the plugin installation logic and will show missing modules.
* Press the green Play button. This will take some time to compile the hmi and the various plugins which support the system.

Usage on the Desktop
-----------
* Press Spacebar to toggle between fullscreen and resizable displays

Running on the Device
-----------
* Have a GDP SDK installed
* Open Qt Creator
* Add a Kit for the GDP SDK
* Open Project and select gdp-hmi.pro from the top level of the repository
* Build the project with Qt Creator
* Copy genivi-11-hmi to /usr/bin on the device
* Copy the HmiController plugin lib and qmldir to /usr/lib/qt5/qml/com.genivi.hmicontroller
* You can easily write a script for the above and use it as a deploy step in Qt Creator
* On the device stop the old hmi with the following commands
    ```sh
    systemctl --user stop gdp-hmi-background
    systemctl --user stop gdp-hmi-panel
    systemctl --user stop gdp-hmi-launcher2
    systemctl --user stop gdp-hmi-controller
    ```
* Run /usr/bin/genivi-11-hmi
* You can easily tell Qt Creator to run this binary from the Project Tab's Run Configuration
* If the home screen and tray are empty you are missing .app files in /usr/share/applications. See below.

Integrating applications on the device
-----------
* The launcher is data driven on the device. You need to have .app files in /usr/share/applications.
* Each application that can be launched needs to have one of these .app files
* The home screen shows a pre-configured set of 6 applications. Think of them like favorites.
* These are curently fixed and an icon will not appear if an apps is missing. The expected home apps are
    - com.jlr.fmradio.app
    - com.jlr.connectedhome.app
    - com.jlr.hvac.app
    - com.jlr.media.app
    - com.jlr.browser.app
    - com.jlr.rvi.app
* The slideout tray shows an alphabetized list of all apps found in /usr/share/applications
* For a quick test you can make .app files for existing applications like EGLWLMockNavigation. The format is as follows:
    ```sh
    Name=Mock Navigation
    Icon=
    Unit=EGLWLMockNavigation.service
    Exec=/usr/bin/EGLWLMockNavigation
    ```
* This simple format is meant to be temporary until a full blown application manager can be written
    - No comments
    - No spaces around the =
* The icon is a suggestion to launcher. It may be replaced with a stylized one. The launcher does this for the above jlr apps.
* The icon should be something scalable like svg or a reasonably high res png

Plugins
-----------
* Plugins are required to be built and installed to the appropriate Qt locations, this is mostly automatic
* This can be dealt with two ways
    1) Terminal to top level of gdp-hmi project and run the equivalent of ```qmake && make && make install```
    2) In the Qt Project configuration, add an extra make step for install
* Shadow building uses a local directory as a deployment spoof, however, plugins will not install to the proper Qt directories if built with shadow build.

TroubleShooting
---------------
* If you get something like: ```com.jlr.hmicontroller``` not defined.
    *) That means your plugins are not setup just yet.

