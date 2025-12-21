# How to build and test Praat on all platforms simultaneously

This page is relevant only if you want to build the complete Praat distribution set,
i.e. you want to speed up building and testing Praat for all of these editions:

- macOS (one executable for ARM64 and x86_64 processors together)
- Windows (separate executables for i686, x86_64 and ARM64)
- Ubuntu/Debian Linux (separate executables for x86_64 and ARM64, both with and without GUI)
- Fedora Linux (separate executables for x86_64 and ARM64)
- LinuxOne (for the big-endian s390x processor, both with and without GUI)
- ChromeBook (separate executables for x86_64 and ARM64)
- Raspberry Pi (ARMv7a processor)

That’s 15 editions. To be able to understand this page, you should be familiar with the ways
in which the separate editions are built, as described in [HOW_TO_BUILD_ONE.md](HOW_TO_BUILD_ONE.md).

At the time of writing (18 December 2025), we develop 13 of the 15 Praat editions on a single
computer, which is a 2023 M3 Macbook Pro: the Mac edition is built natively with Xcode,
the three Windows editions and the ARM64 Fedora edition are built via Parallels Desktop 26,
and the six Linux (Ubuntu) editions and the two Chromebook editions are built via OrbStack.
The Raspberry Pi edition is built separately (on a Raspberry Pi),
and the Intel64 Fedora edition is built via Parallels Desktop 20 on a 2018 Intel Macbook Pro.
We put all 15 editions into a `bin` folder on Dropbox, so that it is easy to test
the Windows and Linux editions on other computers.

In the following we assume that you want to create all of those editions as well.
We hope that our example will be useful to you.

## 1. MacOS development set-up

Your source code folders, such as `fon` and `sys`,
will reside in a folder like `~/Dropbox/Praats/src`,
where you also put `praat.xcodeproj`, as described in section 2 of [HOW_TO_BUILD_ONE.md](HOW_TO_BUILD_ONE.md).
On our 2023 Mac with Xcode 16.3, building Praat with Command-B or Command-R,
after cleaning the build folder with Shift-Command-K,
takes only 56 seconds for the ARM64 part and Intel64 part together (optimization level O3).

## 2. Windows development set-up

On a Windows 10 or Windows 11 computer, you can install MSYS2 or Cygwin,
and create some `praats` folders, as described in section 1 of [HOW_TO_BUILD_ONE.md](HOW_TO_BUILD_ONE.md).

If you work under Parallels Desktop on an ARM64 Mac,
you will want MSYS2, because it has an edition for ARM64.
Your source tree will reside on the Windows disk,
which can be much faster than building directly on the MacOS disk.
To move the source from the MacOS disk to the Windows disk,
you “mount” the MacOS disk from MSYS2 or Cygwin; this is easy:
in Parallels Desktop, choose `Windows 11 ARM` -> `Configure`,
then `Options`, then `Sharing`, then `Share Mac`, and set `Share folders` to `Home folder only`
(if this scares you, then use `Custom Folders` instead).
Your MacOS home folder (i.e. `/Users/yourname`) is now visible anywhere on Windows
as the `Z` drive (or so); from any of the three MSYS shells you can access it as `/z`,
and from the Cygwin terminal you can access it as `/cygdrive/z`.

When developing Praat for Windows, you just edit your files in Xcode;
do not forget to save them (as you do e.g. by building in Xcode).
Then, just as you use Command-B and Command-R in Xcode,
you will be able to type `praat-build` (which only builds) or `praat-run` (which builds and runs)
into your MSYS2 shell. To accomplish this,
add the following definitions into `/home/yourname/.bashrc` (i.e. in your MSYS2 Shell home folder),
so that `bash` will automatically execute them whenever you start your
MSYS shell or Cygwin terminal (you will need to have installed `rsync` and `make`).
On our 2023 Mac, the ARM64 edition will be the default,
but the Intel64/AMD64 and Intel32 versions will also be available.
As the same `.bashrc` file is shared among all three editions,
we use the environment variable `MSYSTEM` to differentiate between the three:

    # in MSYS2:~/.bashrc
    if [[ "$MSYSTEM" == "CLANGARM64" ]]; then
        BUILD_FOLDER="~/praats-arm64-clang"
        MAKEFILE_DEFS="makefiles/makefile.defs.msys-clang"
    elif [[ "$MSYSTEM" == "CLANG64" ]]; then
        BUILD_FOLDER="~/praats-intel64-clang"
        MAKEFILE_DEFS="makefiles/makefile.defs.msys-clang"
    elif [[ "$MSYSTEM" == "CLANG32" ]]; then
        BUILD_FOLDER="~/praats-intel32-clang"
        MAKEFILE_DEFS="makefiles/makefile.defs.msys-clang"
    elif [[ "$MSYSTEM" == "MINGW64" ]]; then
        BUILD_FOLDER="~/praats-intel64-gcc"
        MAKEFILE_DEFS="makefiles/makefile.defs.msys-mingw64"
    elif [[ "$MSYSTEM" == "MINGW32" ]]; then
        BUILD_FOLDER="~/praats-intel32-gcc"
        MAKEFILE_DEFS="makefiles/makefile.defs.msys-mingw32"
    fi
    ORIGINAL_SOURCES="/z/Dropbox/Praats/src"
    EXCLUDES='--exclude="*.xcodeproj" --exclude="Icon*" --exclude=".*" --exclude="*kanweg*"'
    alias praat-build="( cd $BUILD_FOLDER &&\
        rsync -rptvz $ORIGINAL_SOURCES/ $EXCLUDES . &&\
        cp $MAKEFILE_DEFS ./makefile.defs &&\
        make -j12 )"
    alias praat="$BUILD_FOLDER/Praat.exe"
    alias praat-run="praat-build && praat"

This also defines `praat` for running Praat without first rebuilding it.
The cycle from editing Praat on the Mac to running the new version on Windows therefore takes only two steps:

1. edit and save the source code in Xcode on your Mac;
2. type `praat-run` on your Windows 11 (under Parallels Desktop on your Mac) in one of the three MSYS2 shells.

The set-up of the Praat team is cross-compilation: the same Clang compiler running natively on the ARM64
processor of our 2023 Mac creates the object files for all three Windows editions
(for linking, which is fast anyway, the three separate ARM64/Intel64/Intel32 linkers are used).
As a result, building from scratch costs 100 seconds for ARM64,
and only 110 seconds for Intel64/AMD64 (which would be 212 seconds under Intel64/AMD64 emulation)
and 161 seconds for Intel32 (which would be 390 seconds under Intel32 emulation).

## 3. Linux development set-up

On an Ubuntu 20.04 or 22.04 computer, create a folder `praats` in your home folder,
as described in section 3 of [HOW_TO_BUILD_ONE.md](HOW_TO_BUILD_ONE.md).

If you work under Parallels Desktop (19 or later) on an Intel64 Mac,
choose `Ubuntu 20.04 or 22.04` -> `Configure`,
then `Options`, then `Sharing`, then `Share Mac`, and set `Share folders` to `Home folder only`
(or use `Custom Folders` instead).
Your MacOS home folder (i.e. `/Users/yourname`) is now visible on the Ubuntu desktop
as `Home`, and from the `Terminal` you can access it as `/media/psf/Home`.

However, on an ARM64 Mac this procedure with Parallels Desktop works only for the ARM64 edition.
With OrbStack we can instead create the Intel64/AMD64 edition as well
(and building the ARM64 edition is also faster). Your Mac home folder is known
simply as `/Users/yourname` or so.

When developing Praat for Linux, you just edit and save your files in Xcode.
You will be able to type `praat-build` (which only builds) or `praat-run` (which builds and runs)
into your `Terminal` after you add the following definitions into
`/home/yourname/.bash_aliases` in your Ubuntu home folder
(this will be run automatically by `.bashrc` whenever you start a `Terminal` window,
assuming that it uses the `bash` shell; please note the subtle but crucial difference
between `/Users/yourname` and `/home/yourname`):

    # in Ubuntu|Debian:/home/yourname/.bash_aliases
    # in Fedora:/home/yourname/.bashrc.d/bash_aliases
    ORIGINAL_SOURCES="/Users/yourname/Dropbox/Praats/src"
    EXCLUDES='--exclude="*.xcodeproj" --exclude="Icon*" --exclude=".*" --exclude="*kanweg*"'
    alias praat-build="( cd ~/praats &&\
        rsync -rptvz $ORIGINAL_SOURCES/ $EXCLUDES . &&\
        cp makefiles/makefile.defs.linux.pulse-clang makefile.defs &&\
        make -j15 )"
    alias praat="~/praats/praat"
    alias praat-run="praat-build && praat"

In OrbStack, if you don’t have a GUI, try `praat-run --version` instead;
but note that you can have a GUI by running XQuartz. With XQuartz running,
you type something like `xhost +192.168.1.99` (if that’s the local IP address of your computer)
into the XQuartz terminal window (or put `xhost +192.168.1.99` and `exec quartz-wm` into your `.xinitrc` file),
and type something like `export DISPLAY=192.168.1.99:0` (depending on your local IP address)
in your OrbStack window (or into your `.bashrc` file), followed by `praat` into your OrbStack window;
the Praat-for-Linux Objects and Picture windows will then show up on your Mac screen.

On our 2023 Mac, building Praat this way from scratch takes 42 seconds for the ARM64 edition
and 130 seconds (under emulation) for the Intel64/AMD64 edition (optimization level O3).

To build `praat_barren`, create a folder `praatsb`, and define

    # in Ubuntu|Debian:~/.bash_aliases
    # in Fedora:~/.bashrc.d/bash_aliases
    alias praatb-build="( cd ~/praatsb &&\
        rsync -rptvz $ORIGINAL_SOURCES/ $EXCLUDES . &&\
        cp makefiles/makefile.defs.linux.barren-clang makefile.defs &&\
        make -j15 )"
    alias praatb="~/praatsb/praat_barren"
    alias praatb-run="praatb-build && praatb"

You test `praat_barren` briefly by typing

    # on Ubuntu|Fedora|Debian command line
    praatb --version

To build Praat for Chrome64 (64-bit Intel Chromebooks only),
create a folder `praatc`, and define

    # in Ubuntu|Debian:~/.bash_aliases
    # in Fedora:~/.bashrc.d/bash_aliases
    alias praatc-build="( cd ~/praatsc &&\
        rsync -rptvz $ORIGINAL_SOURCES/ EXCLUDES . &&\
        cp makefiles/makefile.defs.chrome64 makefile.defs &&\
        make -j15 )"
    alias praatc="~/praatsc/praat"
    alias praatc-run="praatc-build && praat"

To test Praat for Chrome64, you can just run it on Ubuntu by typing `praatc`,
or you transfer it to a Chromebook for the real test.

## 4. Chromebook development set-up

Parallels Desktop 19 has no emulator for Chrome, so the choice is between
building Praat on a Chromebook directly or building Praat on Ubuntu 20.04 or 22.04.
On a 2019 HP Chromebook with Intel processor, building Praat takes
a forbidding 27 minutes.

So we choose to build Praat on Ubuntu (under Parallels Desktop on an Intel64 Mac),
because building the Intel Chrome64 edition on OrbStack Ubuntu 20.04 takes only
63 seconds (ARM64) or 215 seconds (Intel64/AMD64). If you have the Linux set-up described in 4.3,
you can do this with the `praatc-build` command.

Next, you need a way to get the executable `praat` from Mac/Ubuntu to your Chromebook.
The distributors of Praat do this via an intermediary university computer;
let’s call this computer-in-the-middle `fon.hum.uva.nl`
(not coincidentally, that’s the name of one of the computers that host the Praat website).
If you have an account on that computer (say it’s called `yourname`),
then you can access that account with `ssh`, and it is best to do that without
typing your password each time. To accomplish this, type

    # on Ubuntu command line
    ssh-keygen

on your Ubuntu. This gives you a file `~/.ssh/id_rsa.pub` on your Ubuntu,
which contains your public `ssh` key. You should append the contents of this `id_rsa.pub`
to the file `~/.ssh/authorized_keys` on your intermediary computer. From that moment on,
your intermediary computer will accept `rsync -e ssh` calls from your Ubuntu.
On the intermediary computer, create a folder `~/builds`, and a folder `chrome64` inside that.
If you now define

    # in Ubuntu:~/.bash_aliases
    praatc-put="rsync -tpvz ~/praatsc/praat yourname@fon.hum.uva.nl:~/builds/chrome64"
    praatc-mid="praatc-build && praatc-put"

you can build and send Praat for Chrome to the intermediary computer by just typing

    # on Ubuntu command line
    praatc-mid

On your Chromebook, start up Linux (see the Chromebook download page for details),
create a directory `~/praats` there, and define the following:

    # in Chromebook:~/.bash_aliases
    alias praat-get="( cd ~/praats &&\
        rsync -tpvz yourname@fon.hum.uva.nl:~/builds/chrome64/praat . )"
    alias praat="~/praats/praat"
    alias praat-run="praat-get && praat"

From then on, you can use

    # on Chromebook command line
    praat-run

to fetch Praat from the intermediary computer and run it.

The cycle from editing Praat on the Mac to running it on your Chromebook therefore takes only three steps:

1. edit and save the source code in Xcode on your Mac;
2. type `praatc-mid` on your Ubuntu (under Parallels Desktop on your Mac);
3. type `praat-run` on your Chromebook.

For edits in a `cpp` file (no changes in header files), this whole cycle can be performed within 15 seconds.

## 5. Raspberry Pi development set-up

One could perhaps create the Raspberry Pi edition by cross-compiling on Ubuntu 20.04 or 22.04.
If any reader of these lines has precise instructions, we would like to know about it
(the main problem is how to install the GTK etc libraries in the Raspberry Pi toolchain,
or how to get `dpkg` under Ubuntu-buster to actually find `armhf` libraries).

Till then, you build on the Raspberry Pi itself. Your could do that via an intermediary computer
(analogously to what we described above for Chromebook), but you can also do it directly
if you include your Raspberry Pi in the same local network as your Mac and switch on SSH
on your Raspberry Pi (via Raspberry ->  `Preferences` -> `Raspberry Pi Configuration`
-> `Interfaces` -> `SSH` -> `Enable`. You add your Mac’s public SSH key to your Raspberry Pi with

    # on Mac command line
    ssh-keygen   # only if you have no SSH key yet
    ssh-copy-id pi@192.168.1.2   # or whatever your Pi’s static IP address is

On your Raspberry Pi, you create a folder `~/praats`,
after which you can push the sources from your Mac to your Raspberry Pi with

    # in Mac:~/.bash_profile
    ORIGINAL_SOURCES="~/Praats/src"
    EXCLUDES='--exclude="*.xcodeproj" --exclude="Icon*" --exclude=".*" --exclude="*kanweg*"'
    alias praats-putpi="rsync -rptvz -e ssh $EXCLUDES \
        $ORIGINAL_SOURCES/ pi@192.168.1.2:~/praats"

On the Raspberry Pi, you define

    # in RaspberryPi:~/.bash_aliases
    alias praat-build="( cd ~/praats &&\
        cp makefiles/makefile.defs.linux.rpi makefile.defs &&\
        make -j4 )"
    alias praat="~/praats/praat"
    alias praat-run="praat-build && praat"

after which you can build and run Praat with

    # on Raspberry Pi command line
    praat-run

Thus, the cycle from editing Praat on the Mac to running it on your Raspberry Pi therefore takes three steps:

1. edit and save the source code in Xcode on your Mac;
2. type `praats-putpi` on your Mac;
3. type `praat-run` on your Raspberry Pi, perhaps via `ssh -X pi@192.168.1.2` in your Mac terminal.

From clean sources this takes around 19 minutes (on a Raspberry Pi 4B),
but if no header files change, then it can be done in approximately 20 seconds.

## 6. s390x development set-up on LinuxONE

Once you have a (permanent) open-source LinuxONE account (https://community.ibm.com/zsystems/form/l1cc-oss-vm-request/),
you will probably have an SSH key generated in a `*.pem` file,
which you moved for instance to `~/Dropbox/Praats/ssh/mylinux1key.pem`.

On your LinuxONE virtual machine, you create folders `~/praats` and `~/praatsb`,
after which you can push the sources from your Mac to your LinuxONE VM with

    # in Mac:~/.bash_profile
    ORIGINAL_SOURCES="~/Praats/src"
    EXCLUDES='--exclude="*.xcodeproj" --exclude="Icon*" --exclude=".*" --exclude="*kanweg*"'
    alias praats-putone="rsync -rptvz -e \"ssh -i ~/Dropbox/Praats/ssh/mylinux1key.pem\" $EXCLUDES \
        $ORIGINAL_SOURCES/ linux1@199.199.99.99:~/praats"

where instead of `199.199.99.99` you use the IP address that the LinuxONE owners sent to you.
In your LinuxONE VM, you define

    # in LinuxONE:~/.bash_profile
    alias praat-build="( cd ~/praats &&\
        cp makefiles/makefile.defs.linux.s390.pulse makefile.defs &&\
        make -j4 )"
    alias praat="~/praats/praat"
    alias praat-run="praat-build && praat"
    alias praatb-build="( cd ~/praatsb &&\
        cp makefiles/makefile.defs.linux.s390.barren makefile.defs &&\
        make -j4 )"
    alias praatb="~/praatsb/praat_barren"
    alias praatb-run="praatb-build && praatb"

after which you can build and run Praat with

    # on LinuxONE command line
    praat-run

Thus, the cycle from editing Praat on the Mac to running it on your LinuxONE VM therefore takes three steps:

1. edit and save the source code in Xcode on your Mac;
2. type `praats-putone` on your Mac;
3. type `praat-run` on your LinuxONE VM,
   perhaps via `ssh -X -i ~/Dropbox/Praats/ssh/mylinux1key.pem linux1@199.199.99.99` in your Mac terminal.

## 7. Distributing Praat

If you want to distribute your version of Praat, you can do so on GitHub and/or on a website
(at least, that’s how the main authors do it). Both of these venues require that you have
all the executables in one place. The guide below refers to the creation of packages
for all platforms for Praat version 9.9.99, although your version number will be different.
The packages will be collected in the directory `~/Praats/www` on the Mac.

If you follow the location mentioned in the `.xcodeproj` file, the Mac binary will reside
in a place like `~/Dropbox/Praats/bin/Configuration64`.

After notarizing the Mac binary (see section 2 of [HOW_TO_BUILD_ONE.md](HOW_TO_BUILD_ONE.md)),
you include the executable in a `.dmg` disk image, with the following commands:

    # on Mac command line
    PRAAT_WWW="~/Dropbox/Praats/www"
    PRAAT_VERSION=9999
    cd ~/Dropbox/Praats/bin/macos/Configuration64
    hdiutil create -fs HFS+ -ov -srcfolder Praat.app -volname Praat_${PRAAT_VERSION} praat_${PRAAT_VERSION}.dmg
    hdiutil convert -ov -format UDZO -o ${PRAAT_WWW}/praat${PRAAT_VERSION}_mac.dmg praat_${PRAAT_VERSION}.dmg
    rm praat_${PRAAT_VERSION}.dmg

You also need to distribute the `.xcodeproj` file, which is actually a folder, so that you have to zip it:

    # on Mac command line
    ORIGINAL_SOURCES="~/Dropbox/Praats/src"
    cd $ORIGINAL_SOURCES
    zip -r $PRAAT_WWW/praat${PRAAT_VERSION}_xcodeproj.zip praat.xcodeproj

The Windows executables have to be sent from your Cygwin terminal or MSYS shell to your Mac.
It is easiest to do this without a version number (so that you have to supply the number only once),
so you send them to the intermediate Mac folders `~/Dropbox/Praats/bin/win-intel64`
and `~/Dropbox/Praats/bin/win-intel32` and `~/Dropbox/Praats/bin/win-arm64`.
On MSYS you can define:

    # in MSYS:~/.bashrc
    alias praat-dist="praat-build && rsync -t ~/praats/Praat.exe /z/Dropbox/Praats/bin/win-arm64"
    alias praat64-dist="praat64-build && rsync -t ~/praats64/Praat.exe /z/Dropbox/Praats/bin/win-intel64"
    alias praat32-dist="praat32-build && rsync -t ~/praats32/Praat.exe /z/Dropbox/Praats/bin/win-intel32"

so that you can “upload” the two executables to the Mac with

    # on MSYS command line
    praat-dist
    praat64-dist
    praat32-dist

The three Linux executables have to be sent from your Ubuntu terminal to your Mac,
namely to the folder `~/Dropbox/Praats/bin/linux_intel64` or `~/Dropbox/Praats/bin/linux_arm64`
(each of which will contain `praat` and `praat_barren`), and to the folder
`~/Dropbox/Praats/bin/chrome_intel64` or `~/Dropbox/Praats/bin/chrome_arm64`
(which will contain only `praat`).
On Ubuntu you can define

    # in MSYS2 Intel64/AMD64 Ubuntu:~/.bash_aliases
    alias praat-dist="praat-build && rsync -t ~/praats/praat /Users/yourname/Dropbox/Praats/bin/linux-intel64"
    alias praatb-dist="praatb-build && rsync -t ~/praatsb/praat_barren /Users/yourname/Dropbox/Praats/bin/linux-intel64"
    alias praatc-dist="praatc-build && rsync -t ~/praatsc/praat /Users/yourname/Dropbox/Praats/bin/chrome-intel64"

    # in MSYS2 ARM64 Ubuntu:~/.bash_aliases
    alias praat-dist="praat-build && rsync -t ~/praats/praat /Users/yourname/Dropbox/Praats/bin/linux-arm64"
    alias praatb-dist="praatb-build && rsync -t ~/praatsb/praat_barren /Users/yourname/Dropbox/Praats/bin/linux-arm64"
    alias praatc-dist="praatc-build && rsync -t ~/praatsc/praat /Users/yourname/Dropbox/Praats/bin/chrome-arm64"

so that you can “upload” the three executables to the Mac with

    # on Ubuntu or Fedora command line
    praat-dist
    praatb-dist
    praatc-dist

You can fetch the Raspberry Pi edition directly from your Raspberry Pi:

    # on Mac command line
    rsync -tpvz -e ssh pi@192.168.1.2:~/praats/praat ~/Dropbox/Praats/bin/rpi-armv7

and the s390x edition directly from your LinuxONE account:

    # on Mac command line
    rsync -tpvz -e "ssh -i ~/Dropbox/Praats/ssh/mylinux1key.pem" linux1@199.199.99.99:~/praats/praat ~/Dropbox/Praats/bin/linux-s390x
    rsync -tpvz -e "ssh -i ~/Dropbox/Praats/ssh/mylinux1key.pem" linux1@199.199.99.99:~/praatsb/praat_barren ~/Dropbox/Praats/bin/linux-s390x

When the folders under `~/Dropbox/Praats/bin`, namely `win-intel64`, `win-intel32`, `win-arm64`,
`linux-intel64`, `linux-arm64`, `chrome-intel64`, `chrome-arm64` and `rpi-armv7`
all contain enough new executables (there should be 1, 1, 1, 3, 3, 1, 1 and 1, respectively),
you can issue the following commands to create the packages and install them in `~/Dropbox/Praats/www`:

    # on Mac command line
    zip $PRAAT_WWW/praat${PRAAT_VERSION}_win-intel64.zip ~/Dropbox/Praats/bin/win-intel64/Praat.exe
    zip $PRAAT_WWW/praat${PRAAT_VERSION}_win-intel32.zip ~/Dropbox/Praats/bin/win-intel32/Praat.exe
    zip $PRAAT_WWW/praat${PRAAT_VERSION}_win-arm64.zip ~/Dropbox/Praats/bin/win-arm64/Praat.exe
    ( cd ~/Dropbox/Praats/bin/linux-intel64 &&\
      tar cvf praat${PRAAT_VERSION}_linux-intel64.tar praat &&\
      gzip praat${PRAAT_VERSION}_linux-intel64.tar &&\
      mv praat${PRAAT_VERSION}_linux-intel64.tar.gz $PRAAT_WWW )
    ( cd ~/Dropbox/Praats/bin/linux-intel64 &&\
      tar cvf praat${PRAAT_VERSION}_linux-intel64-barren.tar praat_barren &&\
      gzip praat${PRAAT_VERSION}_linux-intel64-barren.tar &&\
      mv praat${PRAAT_VERSION}_linux-intel64-barren.tar.gz $PRAAT_WWW )
    ( cd ~/Dropbox/Praats/bin/chrome-intel64 &&\
      tar cvf praat${PRAAT_VERSION}_chrome-intel64.tar praat &&\
      gzip praat${PRAAT_VERSION}_chrome-intel64.tar &&\
      mv praat${PRAAT_VERSION}_chrome-intel64.tar.gz $PRAAT_WWW )
    ( cd ~/Dropbox/Praats/bin/linux-arm64 &&\
      tar cvf praat${PRAAT_VERSION}_linux-arm64.tar praat &&\
      gzip praat${PRAAT_VERSION}_linux-arm64.tar &&\
      mv praat${PRAAT_VERSION}_linux-arm64.tar.gz $PRAAT_WWW )
    ( cd ~/Dropbox/Praats/bin/linux-arm64 &&\
      tar cvf praat${PRAAT_VERSION}_linux-arm64-barren.tar praat_barren &&\
      gzip praat${PRAAT_VERSION}_linux-arm64-barren.tar &&\
      mv praat${PRAAT_VERSION}_linux-arm64-barren.tar.gz $PRAAT_WWW )
    ( cd ~/Dropbox/Praats/bin/chrome-arm64 &&\
      tar cvf praat${PRAAT_VERSION}_chrome-arm64.tar praat &&\
      gzip praat${PRAAT_VERSION}_chrome-arm64.tar &&\
      mv praat${PRAAT_VERSION}_chrome-arm64.tar.gz $PRAAT_WWW )
    ( cd ~/Dropbox/Praats/bin/rpi-armv7 &&\
      tar cvf praat${PRAAT_VERSION}_rpi-armv7.tar praat &&\
      gzip praat${PRAAT_VERSION}_rpi-armv7.tar &&\
      mv praat${PRAAT_VERSION}_rpi-armv7.tar.gz $PRAAT_WWW )
    ( cd ~/Dropbox/Praats/bin/linux-s390x &&\
      tar cvf praat${PRAAT_VERSION}_linux-s390x.tar praat &&\
      gzip praat${PRAAT_VERSION}_linux-s390x.tar &&\
      mv praat${PRAAT_VERSION}_linux-s390x.tar.gz $PRAAT_WWW )

Finally, you can update your website and/or create a new release on GitHub.
