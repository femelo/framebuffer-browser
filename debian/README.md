# How to install .deb package ?

```bash
dpkg -i *.deb
```


# How to resolve dependencies automatically ?

```bash
apt -f install # resolve dependencies
dpkg -i *. # install again
```

# What Debian Distro versions are supported ?

* BullsEye
* BookWorm

# What architecture are supported ?

* x86_64
* i386

# What are the executable of the program ?

```
fbrowser
```

# Mouse/Touchpad/Keyboard/Touchscreen does not work ?

Use **evtest** tool to determine the input event devices and set in bash script
