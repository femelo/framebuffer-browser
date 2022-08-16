# Framebuffer Browser

QtWebEngine-based web browser for Linux framebuffer.



[![Browser in action](https://j.gifs.com/ywn3rg.gif)]()

You can see more videos [here](promo.md).

# Prologue

I'm a collector, owning wide range of 90's 2000's era computers, some of them running linux for some managing purposes, I need to copy software, google for some things and so on. I do not need full featured linux distro, I don't even need a Linux distro at all. All i want is plain Linux with busybox  and a decent browser. But i'm forced to install full featured distro because the small one does not come with bloatware (Xorg or Wayland), i don't need that. I need framebuffer on Linux!

As a Linux framebuffer user, I'd know the need of good browser for framebuffer, because there are not many left and no one develops new ones. For example NetSurf browser already dropped framebuffer support, the only one left was links which is terrible at loading modern web sites. Everytime i looked at the software that claims to support framebuffer, it's actually already removed or deprecated, developers intent me to use X or wayland version of their software. I do not agree, i do not want bloatware on my old computers. So i found small repo with small program called FBrowser forked it and started to implement things i need first. What i didn't liked about original FBrowser is that it was targeted only for embedded scene and it was using older libraries based on WebKit (now evolved to QtWebEngine). 

# Features
1. A simple web browser that draws on framebuffer directly, does not require X server.
2. Low memory footprint, works well on devices with >= 128MB RAM.
3. Webkit-based, supports modern HTML5 features, 100/100 on ACID3 test.
4. No OpenGL implementation required, works on embedded processors that have no GPU or GPU driver is not available. OpenGL support is also available.
5. Packaged as a rootfs including all dependencies, could be used as a standalone OS or inside a chroot container on existing Linux distributions.
6. Have basic hotkeys for navigation, mouse and keyboard support as well as custom devices which where supported by evdev and QT (touchscreens,touchpads,joysticks etc...)
7. Full screen video player support (youtube etc...)

# Install

```shell
emerge -av qtwebengine qtwidgets
```

```shell
git clone https://github.com/e1z0/Framebuffer-browser.git
cd Framebuffer-browser
mkdir build && cd build && cmake .. && make && cd ..
./run-fb
```
# Configure

All device input and other settings are described either in **run-fb** or **run-eglfs** files, also note that your local account must be in the following groups:
```
tty, audio, video, usb, input
```
- ./run-fb - uses linux framebuffer device (unaccelerated vesa mode)
- ./run-eglfs - uses linux kernel mode setting drm drivers (accelerated, your device must be supported by the kernel) https://wiki.archlinux.org/title/kernel_mode_setting#Early_KMS_start

All other parameters are described here: https://doc.qt.io/archives/qt-5.12/embedded-linux.html

# Full browser mode

Also there are some changes recently, I was able to port simplebrowser from QT examples and it works in framebuffer very nicely, you can check it out.

![FBrowser_fullbrowser_mode](https://user-images.githubusercontent.com/7213361/184993266-be8405f5-6df0-4a79-b97a-594de1b2c6a4.png)

You can compile it with "FULLBROWSER" cmake option. See more videos [here](promo.md).

# TODO

* cmake option for statically linking
* easy cross platform build options
* autobuild and package for some popular distributions

# License

This project is licensed under the LGPLv3 and commercial license.

Commercial license can be purchased by individual users or organizations.

Please note that you don't need to buy commercial license if your use of this project is compitable with LGPLv3 .
