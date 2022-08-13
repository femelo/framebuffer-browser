# Framebuffer Browser

QtWebEngine-based web browser for Linux framebuffer.

# Prologue

I'm a collector, owning wide range of 90's 2000's era computers, some of them running linux for some managing purposes, I need to copy software, google for some things and so on. I do not need full featured linux distro, I don't even need a Linux distro at all. All i want is plain Linux with busybox  and a decent browser. But i'm forced to install full featured distro because the small one does not come with bloatware (Xorg or Wayland), i don't need that. I need framebuffer on Linux!

As a Linux framebuffer user, I'd know the need of good browser for framebuffer, because there are not many left and no one develops new ones. For example NetSurf browser already dropped framebuffer support, the only one left was links which is terrible at loading modern web sites. Everytime i looked at the software that claims to support framebuffer, it's actually already removed or deprecated, developers intent me to use X or wayland version of their software. I do not agree, i do not want bloatware on my old computers. So i found small repo with small program called FBrowser forked it and started to implement things i need first. What i didn't liked about original FBrowser is that it was targeted only for embedded scene and it was using older libraries based on WebKit (now evolved to QtWebEngine). 

# Features
1. A simple web browser that draws on framebuffer directly, does not require X server.
2. Low memory footprint, works well on devices with >= 128MB RAM.
3. Webkit-based, supports modern HTML5 features, 100/100 on ACID3 test.
4. No OpenGL implementation required, works on embedded processors that have no GPU or GPU driver is not available.
5. Packaged as a rootfs including all dependencies, could be used as a standalone OS or inside a chroot container on existing Linux distributions.
6. Have basic hotkeys for navigation, mouse and keyboard support as well as custom devices which where supported by evdev and QT
7. Full screen video player support (youtube etc...)

# Install

```shell
emerge -av qtwebengine qtwidgets
```

```shell
git clone https://github.com/e1z0/Framebuffer-browser.git
cd Framebuffer-browser
mkdir build && cd build && cmake .. && make
./FBrowser --platform linuxfb
```

If there is a problem with keyboard to not responding, you can specify keyboard device, take a look at your devices by issuing:
```shell
ls -lsah /dev/input/by-path/
```

And then use the desired device:
```shell
./FBrowser --platform linuxfb --plugin evdevkeyboard:/dev/input/event3
```
Mouse also can be specified with the evdevmouse parameter


# License

This project is licensed under the LGPLv3 and commercial license.

Commercial license can be purchased by individual users or organizations.

Please note that you don't need to buy commercial license if your use of this project is compitable with LGPLv3 .
