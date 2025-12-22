# How to build and test Praat for one platform

Developing Praat means two things: *building* the Praat executable from the Praat
source code, and *testing* the correctness of the Praat executable.

**Building** is largely automated:

- On the Mac, you use an existing Xcode project (these files are included in the releases).
- On other platforms (Windows and Unixes), you use existing makefiles
  (these files are included in the source tree).

**Testing** on a platform can be done by starting up Praat on that platform,
and then go through to types of tests. Basic GUI functionality is tested as follows:

1. record a sound (New -> `Record mono Sound...`)
2. open the sound (`View & Edit`)
3. select a part of the sound (drag the mouse across the waveform)
4. play the sound (click on the rectangle below or above the selection)

The integrity of Praats’s algorithms (e.g. signal processing)
and of the Praat scripting language is tested as follows:

1. open the script `test/runAlltests.praat` (Praat -> `Open Praat script...`)
2. run the script (Run -> `Run`)
3. after 2 to 10 minutes, the Info window should contain a big “OK” graph
4. go through steps 1 through 3 for `dwtest/runAllTests.praat`
5. if you feel adventurous, try some tests in the folder `test/manually`

## 1. Developing Praat for Windows

On Windows, Praat is **built** through the makefiles provided in Praat’s source tree.

One could use Cygwin or MSYS2. As we like to provide not only an Intel64/AMD64 and Intel32 edition,
but an ARM64 edition as well, and Cygwin has no toolchains for ARM64, we work with MSYS2 instead.

After installing MSYS2, we see that a `mingw64` toolchain (for Praat’s Intel64/AMD64 edition)
and a `mingw32` toolchain (for Praat’s Intel32 edition) are already available.
Make sure you have installed at least `make`, `gcc`, `g++` and `pkg-config` to make those work.
To also install a `clangarm64` toolchain (for Praat’s ARM64 edition),
run `clangarm64.exe` to get a `clangarm64` shell. In that shell, run `pacman -Syu` to update and
`pacman -S mingw-w64-clang-aarch64-clang` to install the build tools package.
In the same way you can create a `clang64` toolchain and a `clang32` toolchain
(`pacman -S mingw-w64-clang-x86_64-clang` and `pacman -S mingw-w64-i686-clang`),
which are good alternatives to `mingw64` and `mingw32`;
in 2025, MSYS2 ended support for Intel32, so you may have to use `mingw32` for that,
unless you have an old Intel32 toolchain lying around.

Move the Praat sources folders somewhere in your `/home/yourname` tree,
perhaps even in three places, e.g. as `/home/yourname/praats-arm64`,
`/home/yourname/praats-intel64` and `/home/yourname/praats-intel32`;
the folders `fon` and `sys` should be visible within each of these folders.

If you now want to build Praat’s ARM64 edition, start the shell `clangarm64` and type

    cd ~/praats-arm64
    cp makefiles/makefile.defs.msys-clang ./makefile.defs
    make -j12

If you want to build Praat’s Intel64/AMD64 edition, start the shell `clang64` and type

    cd ~/praats-intel64
    cp makefiles/makefile.defs.msys-clang ./makefile.defs
    make -j12

or start the shell `mingw64` and type

    cd ~/praats-intel64
    cp makefiles/makefile.defs.msys-mingw64 ./makefile.defs
    make -j12

If you want to build Praat’s Intel32 edition, start the shell `clang32` and type

    cd ~/praats-intel32
    cp makefiles/makefile.defs.msys-clang ./makefile.defs
    make -j12

or start the shell `mingw32` and type

    cd ~/praats-intel32
    cp makefiles/makefile.defs.msys-mingw32 ./makefile.defs
    make -j12

(With Cygwin, you would install the Devel package `mingw64-x86_64-gcc-g++`
for Praat’s Intel64/AMD64 edition and `mingw64-i686-gcc-g++` for Praat’s Intel32 edition,
plus perhaps `make` and `pkg-config` if you dont’t have those yet.)

**Code-signing.** From version 6.4.25 on, we have signed the three Praat executables
with an “open-source code-signing certificate” (by Certum)
under the name “Paulus Boersma” (the Dutch-legal name of one of the authors).
This is designed to make it easier for Praat to pass the SmartScreen checks
on Windows 11. Early testing shows that the signature is seen by SmartScreen,
but that SmartScreen can still block Praat, so that users still have to
click “Run Anyway” (or “Unblock” under Properties).
It seems that the card reader for this certificate cannot be used yet for code-signing on ARM64 Windows
or on MacOS Sonoma/Sequoia/Tahoe, so for the moment we have to fall back
on an Intel64/AMD64 Windows 10 or 11 machine.

Thorough **testing**, including of the GUI, should ultimately be done on a Windows computer,
though `runAllTests` will successfully run on a virtual machine on an Intel64 Mac with Parallels Desktop
(for 64-bit Windows 7, Windows 8.1, Windows 10 and Windows 11),
or on an ARM64 Mac with Parallels Desktop (for Windows 11 only).
Here are a couple of issues we observed, though:

- For the Intel32 edition, `runAllTests.praat` doesn’t succeed on (macOS) ARM64 hardware,
  because of floating-point imprecisions; to test the Intel32 edition, use Intel64 hardware instead.
- In December 2025, two-finger horizontal scrolling in the Sound window worked correctly in Parallels Desktop
  but not on a Windows computer; to reliably test the GUI, use a Windows computer instead.

## 2. Compiling for Macintosh

To **build** Praat on the Mac, extract the *praatXXXX_xcodeproj.zip* file
from [Praat’s latest release](https://github.com/praat/praat.github.io/releases)
into the folder that contains `sys`, `fon`, `dwtools` and so on (e.g. `~/Dropbox/Praats/src`).
Then open the project `praat.xcodeproj` in Xcode 26.1.1 (or later),
and edit the Intermediate and Product build paths to something that suits you
(Xcode -> Settings... -> Locations -> Derived Data -> Advanced... -> Custom -> Absolute,
then type something after Products, e.g. `~/Dropbox/Praats/bin/macos`,
as well as something after Intermediates, e.g. `~/builds/mac_intermediates`, then click Done).
After this preliminary work, choose Build or Run for the target `praat_mac`.
You can compile with the 14.2 SDK, which will work as far back as macOS 10.11 El Capitan,
which is our deployment target, and will look good even on macOS 26 Tahoe.

If you get an error message like “Code Signing Identity xxx does not match any valid, non-expired,
code-signing certificate in your keychain”, then select the target `praat_mac`, go to Info → Build,
and switch “Code Signing Identity” to “Don’t Code Sign”,
or sign with your own certificate if you have one as a registered Apple developer.

If you get lots of errors saying “Expected unqualified-id” or “Unknown type name NSString”,
then you may have to switch the Type of some .cpp file from “C++ Source” to “Objective-C++ Source”
(under “Identity and Type” in the righthand sidebar).

If you want to build Praat as a library instead of as an executable,
try the target `praat_mac_a` (static) or `praat_mac_so` (dynamic).

**Notarization.** If you want others to be able to use your Mac app,
you will probably have to not only *sign* the executable, but also *notarize* it. To this end,
do Xcode (version 16) -> Product -> Archive -> Distribute App -> Developer ID -> Upload ->
Automatically manage signing -> Upload -> ...wait... (“Package Approved”) ...wait...
(“Ready to distribute”) -> Export Notarized App). If your Praat.app was built into
`~/Dropbox/Praats/bin/macos/Configuration64`, then you can save the notarized
`Praat.app` in `~/Dropbox/Praats/bin/macos`, then drag it in the Finder to
`~/Dropbox/Praats/bin/macos/Configuration64`, overwriting the non-notarized
Praat.app that was already there. If on the way you receive an error
“App Store Connect Operation Error -- You must first sign the relevant contracts online”,
or “Couldn’t communicate with a helper application“,
you will have to log in to `developer.apple.com` and do Review Agreement -> Agree;
you may then also have to either wait for a couple of minutes,
and or go (or log in) to App Store Connect, then Business (or Agreements, Tax, and Banking)
-> Paid Apps Agreement -> View and Agree to Terms (even if you have no paid apps).

**Testing** on multiple Intel64/AMD64 platform versions can be done on older Intel64 Macs,
using virtual machines with Parallels Desktop. For instance, a 2013 Macbook Pro can handle
OS X 10.11 El Capitan, 10.12 Sierra, 10.13 High Sierra, macOS 10.14 Mojave, 10.15 Catalina,
and macOS 11 Big Sur, while a 2018 Macbook Pro can handle macOS 10.14 Mojave, 10.15 Catalina,
macOS 11 Big Sur, macOS 12 Monterey, and macOS 14 Sonoma natively.
Testing on multiple ARM64 platform versions can be done on an older ARM64 Mac,
using virtual machines with Parallels Desktop. For instance, a 2020 Mac Mini could handle
macOS 11 Big Sur, and macOS 12 Monterey, and macOS 13 Ventura (and macOS 14 Sonoma natively),
while a 2023 Macbook Pro can do macOS 15 Sequoia, or macOS 26 Tahoe natively.

## 3. Compiling on Linux and other Unixes

To set up the system libraries required for **building** with the Clang or GCC compiler,
install the necessary build tools as well as some graphics and sound packages:

    # on Ubuntu or Debian command line (Intel64/AMD64 or ARM64 processor)
    sudo apt install make rsync pkg-config
    # either:
        sudo apt install clang libc++-dev libc++abi-dev
    # or:
        sudo apt install gcc g++
    sudo apt install libgtk-3-dev
    sudo apt install libasound2-dev
    sudo apt install libpulse-dev
    sudo apt install libjack-dev

On Fedora you would do instead:

    # on Fedora command line:
    sudo dnf install make rsync pkg-config
    # either:
        sudo dnf install clang libcxx-devel libcxxabi-devel
    # or:
        sudo dnf install gcc g++
    sudo dnf install gtk3-devel
    sudo dnf install pulseaudio-libs-devel
    sudo dnf install alsa-lib-devel
    sudo dnf install pipewire-jack-audio-connection-kit-devel
          # i.e. normaly *not* jack-audio-connection-kit-devel

On Centos you would do something like:

    # on Centos command line:
    sudo dnf install dnf-plugins-core           # Centos-specific preparation
    sudo dnf config-manager --set-enabled crb   # Centos-specific preparation
    sudo dnf install epel-release               # Centos-specific preparation
    sudo dnf install make rsync pkg-config
	sudo dnf install gcc g++
    sudo dnf install gtk3-devel
    sudo dnf install pulseaudio-libs-devel
    sudo dnf install alsa-lib-devel
    sudo dnf install pipewire-jack-audio-connection-kit-devel

To set up your source tree for Linux, go to Praat's sources directory (where the folders `fon` and `sys` are)
and type one of the four following commands:

    # on Ubuntu or Fedora command line (Intel64/AMD64 or ARM64 processor)
    # either:
        cp makefiles/makefile.defs.linux.pulse-clang ./makefile.defs
    # or:
        cp makefiles/makefile.defs.linux.pulse-gcc ./makefile.defs

    # on Centos command line (Intel64/AMD64 or ARM64 processor)
    cp makefiles/makefile.defs.linux.pulse-gcc ./makefile.defs

    # on Ubuntu command line (s390x processor)
    cp makefiles/makefile.defs.linux.s390x.pulse ./makefile.defs

    # on Chromebook command line
    cp makefiles/makefile.defs.chrome64 ./makefile.defs

    # on Raspberry Pi command line
    cp makefiles/makefile.defs.linux.rpi ./makefile.defs
    
    # on FreeBSD command line
    cp makefiles/makefile.defs.freebsd.alsa ./makefile.defs

To build the Praat executable, type `make -j15` or so.
If your Unix isn’t Linux, you may have to edit the library names in the makefile
(you may need pthread, gtk-3, gdk-3, atk-1.0, pangoft2-1.0, gdk_pixbuf-2.0, m, pangocairo-1.0,
cairo-gobject, cairo, gio-2.0, pango-1.0, freetype, fontconfig, gobject-2.0, gmodule-2.0, 
gthread-2.0, rt, glib-2.0, asound, jack).

When compiling Praat on an external supercomputer or so, you will not have sound.
If you do have `libgtk-3-dev` or `gtk3-devel` (and its dependencies), do

    # on Ubuntu or Fedora command line (Intel64/AMD64 or ARM64 processor)
    cp makefiles/makefile.defs.linux.silent ./makefile.defs

Then type `make -j12` or so to build the program. If your Unix isn’t Linux,
you may have to edit the library names in the makefile (you may need pthread, gtk-3, gdk-3, atk-1.0,
pangoft2-1.0, gdk_pixbuf-2.0, m, pangocairo-1.0, cairo-gobject, cairo, gio-2.0, pango-1.0, 
freetype, fontconfig, gobject-2.0, gmodule-2.0, gthread-2.0, rt, glib-2.0).

When compiling Praat for use as a server for commands from your web pages,
you may not need sound, a GUI, amd graphics. In that case, do

    # on Ubuntu or Fedora command line (Intel64/AMD64 or ARM64 processor)
    # either:
        cp makefiles/makefile.defs.linux.barren-clang ./makefile.defs
    # or:
        cp makefiles/makefile.defs.linux.barren-gcc ./makefile.defs

    # on Ubuntu command line (s390x processor)
    cp makefiles/makefile.defs.linux.s390x.barren ./makefile.defs

which creates the executable `praat_barren`. Then type `make` or `make -j15` to build the program.
If your Unix isn’t Linux, you may have to edit the library names in the makefile.

The above works exactly the same for Intel64/AMD64 and ARM64 processors, with the same makefiles.

**Testing** on multiple platform versions can be done with virtual machines.
On an Intel64 Mac with Parallels Desktop 20, we test with virtual machines for
e.g. Ubuntu 20.04, Ubuntu 22.04, Fedora 38, Mint 20.2,
Debian GNU Linux 10.10, Debian GNU Linux 12, CentOS 8.4, and CentOS Stream 9.
On an ARM64 Mac with Parallels Desktop 26, we test with virtual machines for
e.g. Ubuntu 22.04, Ubuntu 24.04, Fedora 38, Fedora 40, Fedora 42, and Debian GNU Linux 12 ARM64.

## 4. Developing Praat on all platforms simultaneously

For this, see [HOW_TO_BUILD_ALL.md](HOW_TO_BUILD_ALL.md).
