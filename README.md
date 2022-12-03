# SkatzohirOS_x64
Welcome to the repository of SkatzohirOS_x64!

# What is this?
SkatzohirOS_x64 is a version of SkatzohirOS that
is (at least currently) designed to run on PCs
that are based on the x86_64 architecture. It
currently only supports running on systems that
use EFI firmwares.

# Licensing
All code under the `bootloader` and `kernel`
directories is licensed under MIT. Please see
`LICENSE` for details about the MIT license. 

The bootloader also uses 
[POSIX-UEFI](https://gitlab.com/bztsrc/posix-uefi), 
which is also licensed under the MIT license.

Code under `elfonashelf` is licensed under...
well, nothing. It was built purely
off of common, open knowledge available on the
internet, and you can't really put a license on that.
So, if you don't feel like making your own ELF
library, feel free to use ELF on a shelf with no
terms at all.

# How to compile and use
SkatzohirOS_x64 currently only supports booting
on EFI systems. This means that a computer with a
regular Legacy BIOS will not run it. It will also
not boot with CSM on an EFI computer. This is because
the bootloader is only available as an EFI app,
which cannot be booted by a BIOS computer or through
CSM. If your computer is set to use only CSM for
booting, please set it to only UEFI or Auto mode
to run SkatzohirOS_x64.

The requirements for building are:
- `clang` for creating the object files
- `lld` to link the object files into an executable
- GNU `mtools` and `dd` to create a hard disk image
- GNU `make` for the build system

**NOTE: The Makefile uses Clang as its C compiler.
However, you can use any cross-compiler that supports
a bare metal x86_64 ELF target. Just make sure to
change the value of the CC variable in the Makefile
for the kernel and the flags if required.**

Building is simple: Just run the `build.sh` script.
If all goes well, this will create the `build`
directory with 3 files: `kernel.elf`, `bootx64.efi`
and `SkatzohirOS_x64.img`. `SkatzohirOS_x64.img`
is the hard drive image. You can use it on an
emulator like QEMU or burn/flash/write it to
physical media to boot on a real computer.

To remove all compiled files, run `clean.sh`.

The provided CMakeLists.txt file is only used
for compatibility with the assistance tools of
some IDEs and can **not** actually be used for
building SkatzohirOS_x64 binaries or images.

# Layout and technical details
`bootloader` contains the code for the bootloader.
Specifically, it contains the main bootloader code
in `bootloader.c` and the `uefi` subdirectory 
contains the UEFI library that I used,
[POSIX-UEFI](https://gitlab.com/bztsrc/posix-uefi).
Note that it is mostly only used for easier UEFI 
builds. The libc APIs that it provides are not used 
much. Most of the work is done with direct UEFI calls.
However, in certain places, `printf` is used for
printing some debugging data and the types and
global variables provided by POSIX-UEFI are used
all across the code. Porting it to a different
UEFI library should not be difficult, however.

`elfonashelf` contains ELF on a shelf, a simple
library that provides some basic data structures
and defines to work with ELF executables in a
freestanding environment, where such a library
can't be expected to exist. Its usage currently is 
the exact same as the `elf.h` header that you can find 
on a lot of POSIX systems. However, a lot of features 
are not yet implemented. The only things that are
defined currently are ELF64 program and file headers. 
This is prone to change as the OS needs more 
advanced things.

`kernel` contains the kernel code. Not much to say
here.
