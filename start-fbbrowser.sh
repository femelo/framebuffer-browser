#!/bin/bash
ARGS="$@"

static_settings() {
  EXECUTABLE=fbbrowser
}

load_settings() {
  echo "Loading settings..."
  if [ ! -e "config.json" ]; then
     echo -e '{\n"url":"https://google.com",\n"wsServerPort":0,\n"width": 0,\n"height": 0,\n"proxyHost": "",\n"proxyPort": 0\n}' > config.json
  fi
  if [ ! -e "$HOME/.config/fb-browser/config" ];then
    need_configure
  fi
  if [[ $(grep -L "BACKEND_DEV" "$HOME/.config/fb-browser/config") ]]; then
    need_configure
  fi
  if [[ $(grep -L "KEYBOARD_DEV" "$HOME/.config/fb-browser/config") ]]; then
    need_configure
  fi
  source <(grep = $HOME/.config/fb-browser/config)
  echo "Settings loaded"
  if [ "$BACKEND_DEV" == "fb" ];then
     echo "Selected video backend: fb"
     export QT_QPA_PLATFORM=linuxfb:fb=/dev/fb0:nographicsmodeswitch=1
     # Disable libinput support for fb
     export QT_QPA_FB_NO_LIBINPUT=1
     # Force full screen mode
     export QT_QPA_FB_FORCE_FULLSCREEN=1

  elif [ "$BACKEND_DEV" == "eglfs" ];then
     echo "Selected video backend: eglfs"
     # Enable eglfs backend
     export QT_QPA_PLATFORM=eglfs
     # Use the KMS/DRM backend.
     export QT_QPA_EGLFS_INTEGRATION=eglfs_kms
     # Disable libinput support for eglfs
     export QT_QPA_EGLFS_NO_LIBINPUT=1
     # Enable eglfs debug logging
     #export QT_QPA_EGLFS_DEBUG=1
  else
     need_configure
  fi
  if [ "$TOUCHSCREEN_DEV" != "" ];then
    # Touchscreen support, see info using utility evtest
    export QT_QPA_EVDEV_TOUCHSCREEN_PARAMETERS=/dev/input/${TOUCHSCREEN_DEV}
  fi
  if [ "$KEYBOARD_DEV" != "" ];then
    # Keyboard support, see info using utility evtest
    export QT_QPA_EVDEV_KEYBOARD_PARAMETERS=/dev/input/${KEYBOARD_DEV}:grab=1
  fi
  # debug input logging
  #export QT_LOGGING_RULES=qt.qpa.input=true
  # attempt to disable the terminal keyboard on application startup by setting the tty's keyboard mode to K_OFF
  export QT_QPA_ENABLE_TERMINAL_KEYBOARD=0
}

launch() {
  exes=0
  # where are the executable of the program ?
  if [ -e "/usr/local/bin/${EXECUTABLE}" ];then
     EXE="/usr/local/bin/${EXECUTABLE}"
  else if [ -e "build/fbbrowser" ]; then
     EXE="./build/fbbrowser"
  else
    EXE=""
  fi
     
  fi
  if [ "$BACKEND_DEV" == "fb" ];then
    # turn off tty1 cursor blinking
    setterm -cursor off > /dev/tty1
  fi
  if [ "$EXE" == "" ];then
    echo "Program executable not found, have you compiled or installed the program?"
  else
    if [ "$ARGS" == "" ];then
      $EXE
    else
      $EXE "${ARGS}"
    fi
  fi
  if [ "$BACKEND_DEV" == "fb" ];then
    # clear framebuffer
    cat /dev/zero 1>/dev/fb0 2>/dev/null
    # back on tt1 cursor blinking
    setterm -cursor on > /dev/tty1
  fi
}

configure_auto() {
  echo "Detecting input devices..."
  KEYBOARD=`cat /proc/bus/input/devices | awk '/[Kk][Ee][Yy][Bb][Oo][Aa][Rr][Dd]/{for(a=0;a>=0;a++){getline;{if(/kbd/==1){ print $NF;exit 0;}}}}'`
  echo "Keyboard found as: $KEYBOARD"
  MOUSE=`cat /proc/bus/input/devices | awk '/[Kk][Ee][Yy][Bb][Oo][Aa][Rr][Dd]/{for(a=0;a>=0;a++){getline;{if(/mouse/==1){ print $NF;exit 0;}}}}'`
  echo "Mouse/Touchpad found as: $MOUSE"
  save_setting "KEYBOARD_DEV" ${KEYBOARD}
  save_setting "MOUSE_DEV" ${MOUSE}
  save_setting "BACKEND_DEV" fb
  echo "We detected only these devices, keep in mind that devices might be detected incorrectly, depending on your configuration"
  echo "If you have many devices, like running laptop with touchscreen, touchpad and mouse or keyboard attached, please configure manually"
  echo "Also note that depending on configuration, linux can mix devices names (/dev/input/eventX) on every boot"
  echo "It's very adviceable to use udev rules, making the devices symlinked with the static names"
  echo "Please read more about static device name: https://www.emanuelepapa.dev/assign-static-device-name-using-udev-rules/"
  echo "Thank you for understanding!"
  read -n 1 -s -r -p "Press any key to continue"
}


confirm_dialog() {
  while true; do
      echo 'Are you sure "y/n" ?:'
      read confirmation
      if [ "$confirmation" == "y" ]; then
          echo "$confirmation"
          break
      elif [ "$confirmation" == "n" ];then
          echo "$confirmation"
          break
      else
          echo 'Please enter valid answer "y/n"'
      fi
  done
}

# Save setting
save_setting() {
  local name=${1//\//\\/}
  local value=${2//\//\\/}

  if [ ! -d "$HOME/.config/fb-browser" ];then
     mkdir -p "$HOME/.config/fb-browser"
  fi
  if [ ! -e "$HOME/.config/fb-browser/config" ];then
     echo "" >  "$HOME/.config/fb-browser/config"
  fi
    sed -i \
        -e '/^#\?\(\s*'"${name}"'\s*=\s*\).*/{s//\1'"${value}"'/;:a;n;ba;q}' \
        -e '$a'"${name}"'='"${value}" "$HOME/.config/fb-browser/config"
}

# del setting based on device name
del_setting() {
 local SETTING="$1"
 echo "Deleting setting $SETTING"
 sed -i '/$SETTING/d' "$HOME/.config/fb-browser/config"
}

config_device() {
 local dev=$1
 name=`cat /sys/class/input/event${1}/device/name`
 echo "Configuring ${name} ID: ${dev}"
   while true; do
      echo 'Enter ID or b (back), q (quit):'
      echo '0 -> None (remove setting if exists)'
      echo '1 -> Keyboard'
      echo '2 -> Mouse'
      echo '3 -> Touchpad'
      echo '4 -> Touchscreen'
      read devtip
      if [ -n "$devtip" ] && [ "$devtip" -eq "$devtip" ] 2>/dev/null; then
          confirm_dialog
          if [ $confirmation == "y" ];then
             echo "You confirmed and we are proceeding to save the setting.."
             # remove setting
             if [ "$devtip" == 0 ];then
                 del_setting "event${dev}"
                 echo "Settings deleted!"
             # keyboard
             elif [ "$devtip" == 1 ];then
                 echo "You selected keyboard"
                 save_setting "KEYBOARD_DEV" event${dev}
                 echo "Settings saved!"
             # mouse
             elif [ "$devtip" == 2 ];then
                 echo "You have selected Mouse"
                 save_setting "MOUSE_DEV" event${dev}
                 echo "Settings saved!"
             # touchpad
             elif [ "$devtip" == 3 ];then
                 echo "You have selected Touchpad"
                 save_setting "TOUCHPAD_DEV" event${dev}
                 echo "Settings saved!"
             # touchscreen
             elif [ "$devtip" == 4 ];then
                 echo "You have selected Touchscreen"
                 save_setting "TOUCHSCREEN_DEV" event${dev}
                 echo "Settings saved!"
             else
                 echo "Wrong device type specified, aborting.."
             fi
          fi
          break
      elif [ "$devtip" == "b" ];then
          break
      elif [ "$devtip" == "q" ];then
          exit 1
      else
          echo 'Please enter valid number or b (back), q (quit)'
          echo "You wrote: $devtip"
      fi
  done
}

configure_manual() {
  echo -e "\n\n\nSelect video backend:"
  echo "1 -> linux framebuffer (default fb)"
  echo "2 -> egls implementation using kernel mode setting and video card drivers (works only with supported video cards)"
  if [[ $(grep "BACKEND_DEV" "$HOME/.config/fb-browser/config") ]]; then
    echo "3 -> continue without (use previous settings)"
  fi
  while true; do
      echo 'Enter ID:'
      read vga
      if [ "$vga" == 1 ];then
          echo "You have selected fb"
          save_setting "BACKEND_DEV" "fb"
          echo "Settings saved!"
          break
      elif [ "$vga" == 2 ];then
          echo "You have selected eglfs"
          save_setting "BACKEND_DEV" "eglfs"
          echo "Settings saved!"
          break
      elif [ "$vga" == 3 ];then
          break
      else
          echo 'Please enter valid number'
          echo "You wrote: $vga"
      fi
  done
  echo -e  "\n\n\nInput device list: "
  while true; do
    find /dev/input/ -maxdepth 1 -exec basename {} \;|grep -o '[[:digit:]]\+'|sort -un|awk '{dev=$1;printf "ID: " $0" Name: ";system("cat /sys/class/input/event"dev"/device/name");}'
    echo "Enter device ID (number) which you want to configure, q to exit: "
    while true; do
      echo 'Enter ID:'
      read dev
      if [ -n "$dev" ] && [ "$dev" -eq "$dev" ] 2>/dev/null; then
          config_device $dev
          break
      elif [ "$dev" == "b" ];then
          break
      elif [ "$dev" == "q" ];then
          exit 1
      else
          echo 'Please enter valid number or q (quit)'
          echo "You wrote: $dev"
      fi
    done
  done
}


need_configure() {
  echo -e  "\n\n\nWe where unable to load application settings!!!"
  echo "You will have to configure it"
  echo "Make your choice:"
  echo "1 -> Configure automatically"
  echo "2 -> Configure manually"
  echo "3 -> Reset config"
  echo "4 -> Exit"
  while true; do
      echo 'Enter ID:'
      read ans
      if [ "$ans" == 1 ];then
          configure_auto
          break
      elif [ "$ans" == 2 ];then
          configure_manual
          break
      elif [ "$ans" == 3 ];then
          if [ ! -d "$HOME/.config/fb-browser" ];then
             mkdir -p "$HOME/.config/fb-browser"
          fi
          echo "" > $HOME/.config/fb-browser/config
          echo "Configuration reset done!"
      elif [ "$ans" == 4 ];then
          exit 0
      else
          echo 'Please enter valid number 1, 2, 3 or 4 (quit)'
          echo "You wrote: $ans"
      fi
  done
}

test_hardware() {
  echo "Not implemented yet!"
}

test_devices() {
#type evtest >/dev/null 2>&1 || { echo >&2 "The required tool 'evtest' was not found.  Aborting."; exit 1; }
 while true; do
    find /dev/input/ -maxdepth 1 -exec basename {} \;|grep -o '[[:digit:]]\+'|sort -un|awk '{dev=$1;printf "ID: " $0" Name: ";system("cat /sys/class/input/event"dev"/device/name");}'
    echo "Enter device ID (number) which you want to test, q to exit: "
    while true; do
      echo 'Enter ID:'
      read dev
      if [ -n "$dev" ] && [ "$dev" -eq "$dev" ] 2>/dev/null; then
          evtest /dev/input/event${dev}
          break
      elif [ "$dev" == "q" ];then
          exit 1
      else
          echo 'Please enter valid number or q (quit)'
          echo "You wrote: $dev"
      fi
    done
  done
}

banner() {
  echo "Welcome to Framebuffer browser (c) 2022 e1z0"
  echo "https://github.com/e1z0/Framebuffer-browser"
  echo -e "\n\nThe following parameters are available:"
  echo "$0 configure -> manually configure the browser"
  echo "$0 autoconfigure -> try to automatically configure the browser"
  echo "$0 reset -> reset configuration"
  echo "$0 testinput -> test device input capabilities"
  echo "$0 test -> test hardware capabilities"
  exit 0
}

main() {
  if [ ! -d "$HOME/.config/fb-browser" ];then
    need_configure
  fi
  load_settings
  launch
}

static_settings
if [ "$1" = "-h" -o "$1" = "--help" ]; then
  banner
  exit 0
fi
if [ "$1" = "configure" ];then
  configure_manual
  exit 0
fi
if [ "$1" = "autoconfigure" ];then
  configure_auto
  exit 0
fi
if [ "$1" = "reset" ];then
  echo "" > $HOME/.config/fb-browser/config
  echo "Done"
  exit 0
fi
if [ "$1" = "test" ];then
   test_hardware
   exit 0
fi
if [ "$1" = "testinput" ];then
   test_devices
   exit 0
fi
main
