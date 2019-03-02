#!/usr/bin/env bash
# set -v
# we need bash 4 for associative arrays
if [ "${BASH_VERSION%%[^0-9]*}" -lt "4" ]; then
  echo "BASH VERSION < 4: ${BASH_VERSION}" >&2
  exit 1
fi

# define colors
GRAY='\033[1;30m'; RED='\033[0;31m'; LRED='\033[1;31m'; GREEN='\033[0;32m'; LGREEN='\033[1;32m'; ORANGE='\033[0;33m'; YELLOW='\033[1;33m'; BLUE='\033[0;34m'; LBLUE='\033[1;34m'; PURPLE='\033[0;35m'; LPURPLE='\033[1;35m'; CYAN='\033[0;36m'; LCYAN='\033[1;36m'; LGRAY='\033[0;37m'; WHITE='\033[1;37m';

echo -e "\n########################################################################";
echo -e "${YELLOW}INSTALLING ARDUINO IDE"
echo "########################################################################";

# if .travis.yml does not set version
if [ -z $ARDUINO_IDE_VERSION ]; then
export ARDUINO_IDE_VERSION="1.8.7"
echo "NOTE: YOUR .TRAVIS.YML DOES NOT SPECIFY ARDUINO IDE VERSION, USING $ARDUINO_IDE_VERSION"
fi

# if newer version is requested
if [ ! -f $HOME/arduino_ide/$ARDUINO_IDE_VERSION ] && [ -f $HOME/arduino_ide/arduino ]; then
echo -n "DIFFERENT VERSION OF ARDUINO IDE REQUESTED: "
shopt -s extglob
cd $HOME/arduino_ide/
rm -rf *
if [ $? -ne 0 ]; then echo -e """$RED""\xe2\x9c\x96"; else echo -e """$GREEN""\xe2\x9c\x93"; fi
cd $OLDPWD
fi

# if not already cached, download and install arduino IDE
echo -n "ARDUINO IDE STATUS: "
if [ ! -f $HOME/arduino_ide/arduino ]; then
echo -n "DOWNLOADING: "
wget --quiet https://downloads.arduino.cc/arduino-$ARDUINO_IDE_VERSION-linux64.tar.xz
if [ $? -ne 0 ]; then echo -e """$RED""\xe2\x9c\x96"; else echo -e """$GREEN""\xe2\x9c\x93"; fi
echo -n "UNPACKING ARDUINO IDE: "
[ ! -d $HOME/arduino_ide/ ] && mkdir $HOME/arduino_ide
tar xf arduino-$ARDUINO_IDE_VERSION-linux64.tar.xz -C $HOME/arduino_ide/ --strip-components=1
if [ $? -ne 0 ]; then echo -e """$RED""\xe2\x9c\x96"; else echo -e """$GREEN""\xe2\x9c\x93"; fi
touch $HOME/arduino_ide/$ARDUINO_IDE_VERSION
else
echo -n "CACHED: "
echo -e """$GREEN""\xe2\x9c\x93"
fi

# link test library folder to the arduino libraries folder
#ln -s $TRAVIS_BUILD_DIR $HOME/arduino_ide/libraries/Adafruit_Test_Library

# add the arduino CLI to our PATH
# export PATH="$HOME/arduino_ide:$PATH"

echo -e "\n########################################################################";
echo -e "${YELLOW} ADDING PACKAGES"
echo "########################################################################";

# install the teensyduino board packages
echo -n "ADD PACKAGE INDEX: "
DEPENDENCY_OUTPUT=$(arduino --pref "boardsmanager.additional.urls=https://github.com/newdigate/teensy-build/raw/master/package_teensyduino_index.json" --save-prefs 2>&1)
if [ $? -ne 0 ]; then echo -e """$RED""\xe2\x9c\x96"; else echo -e """$GREEN""\xe2\x9c\x93"; fi

echo -n "TEENSYDUINO: "
# arduino --install-boards teensyduino:avr
DEPENDENCY_OUTPUT=$(arduino --install-boards teensyduino:avr 2>&1)
if [ $? -ne 0 ]; then echo -e "\xe2\x9c\x96 OR CACHED"; else echo -e """$GREEN""\xe2\x9c\x93"; fi

echo -e "\n########################################################################";
echo -e "${YELLOW} PATCH gcc-arm-none-eabi libs"
echo "########################################################################";

cd /home/travis/.arduino15/packages/teensyduino/tools/gcc-arm-none-eabi/5.4.1-2016q2/bin
wget --quiet https://raw.githubusercontent.com/PaulStoffregen/precompile_helper/master/precompile_helper.c
gcc precompile_helper.c -o precompile_helper
cd $OLDPWD

mkdir /home/travis/arm-none-eabi-teensy-libs
cd /home/travis/arm-none-eabi-teensy-libs
git clone https://github.com/newdigate/arm-none-eabi-teensy-libs.git .
cd $OLDPWD

cd /home/travis/.arduino15/packages/teensyduino/tools/gcc-arm-none-eabi/5.4.1-2016q2/arm-none-eabi/lib
rm -r *
cp -r /home/travis/arm-none-eabi-teensy-libs/* . 
cd $OLDPWD

echo -e "\n########################################################################";
echo -e "${YELLOW} dpkg --add-architecture i386 libc6:i386"
echo "########################################################################";
echo -e $PWD

sudo dpkg --add-architecture i386
sudo apt-get update
sudo apt-get install libc6:i386

# install random lib so the arduino IDE grabs a new library index
# see: https://github.com/arduino/Arduino/issues/3535
echo -n "UPDATE LIBRARY INDEX: "
DEPENDENCY_OUTPUT=$(arduino --install-library USBHost > /dev/null 2>&1)
if [ $? -ne 0 ]; then echo -e """$RED""\xe2\x9c\x96"; else echo -e """$GREEN""\xe2\x9c\x93"; fi

# set the maximal compiler warning level
echo -n "SET BUILD PREFERENCES: "
DEPENDENCY_OUTPUT=$(arduino --pref "compiler.warning_level=all" --save-prefs 2>&1)
if [ $? -ne 0 ]; then echo -e """$RED""\xe2\x9c\x96"; else echo -e """$GREEN""\xe2\x9c\x93"; fi
