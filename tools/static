#!/bin/sh

cd ..
wget https://download.qt.io/official_releases/qt/5.15/5.15.5/single/qt-everywhere-opensource-src-5.15.5.tar.xz -O qt.xz
tar -xf qt.xz
cd qt
./configure -opensource -confirm-license -nomake examples -nomake tests \
 -no-compile-examples -no-sql-sqlite -no-sql-odbc -skip qtdoc \
 -static -skip qt3d -skip qtcanvas3d -webengine-alsa -alsa -no-gstreamer -evdev \
 -skip qtserialport -skip qtqa -skip qtscript -no-opengl -no-feature-pulseaudio -no-feature-xlib -no-feature-xcb -no-feature-xkbcommon \
 -skip qtwayland -skip qtconnectivity -skip qtgraphicaleffects \
 -skip qtimageformats -skip qtandroidextras -skip qtx11extras \
 -skip qtmacextras -no-rpath -verbose -kms -linuxfb \
 -prefix ../qt5 -hostprefix ../qt
gmake && gmake install
