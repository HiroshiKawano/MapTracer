MapTracer
=========

Raspberry Pi用のGoogle Map及びGoogle StreetViewを表示するアプリです。

Linuxのシェル上から直接実行出来ます。(X上からも実行出来ます)
アプリのイベント管理、イメージ表示にSDLライブラリを使用しています。

How to build this project
---------------------
You can build this project on Raspbian "Wheezy".Cross-build is now preparing now.

0) Preparation

$ sudo apt-get install curl libcurl4-openssl-dev libsdl-dev

$ sudo ln -s libSDL_image-1.2.so.0 libSDL_image.so (If needed)

1) cd src

2) make

wait 1 minute.

3) If the complation is succeeded,there should be MapTracer executable.

4) run

(ex) $ ./MapTracer -p /dev/ttyUSB0


How to launch this program
-----------------------
This program needs some options to launch,just execute like "./MapTracer" and you can see the usage.

*-p [tty_dev]*

>Specify tty-device to communicate with GPS.

*-c*

>Specify CVBS(Composite video output) resolution mode

>If you launch MapTracer without specifying -c with CVBS connection,the program is crashed soon(happens internally SDL-library).This is ToDo to fix to detect automatically whether the video connection is HDMI or not,or maybe fixed in later's Raspbian "wheezy" release(in SDL?).

*-n*

> No-GUI mode to debug with console message.

*-d*

> DEMO mode.If you specified this option,the center of the map is always pointed at the center of OSAKA Station.The point is fixed and not move even the GPS informs you are moving.

How to use this program
---------------------

After the program is launched,soon Google map is shown up on screen when the GPS catched the current position.

Push [2] to switch to Google StreetView mode.

Push [1] to switch back to Google Maps mode.

Push [0] to exit this program.

Contacts
-------
Email : ishii.yasuhiro@gmail.com

Facebook : http://facebook.com/ishiiyasu

Twitter : @yishii


