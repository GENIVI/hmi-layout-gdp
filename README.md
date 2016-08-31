Prototype of new GENIVI HMI designs for GDP 11
---------------------------------------------------

Prerequisities
---------------
General:
* Qt 5.6+ with QtQuick
* Plugins built

Running
-----------
* Open Qt Creator
* Open Project and select gdp-hmi.pro from the top level of the repository
* On the first run Qt Creator will ask you to configure your project.  The default proposal will be a Desktop configuration using already installed compilers, so you should probably be able to just hit "OK".
* In the Projects tab on the left, if not still there, make sure to disable Shadow build. This messes with the plugin installation logic and will show missing modules.
* Press the green Play button. This will take some time to compile the hmi and the various plugins which support the system.

Usage
-----------
* Press Spacebar to toggle between fullscreen and resizable displays

