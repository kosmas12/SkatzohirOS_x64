OSNAME=SkatzohirOS_x64
BUILDDIR=build

mkdir -p $BUILDDIR > /dev/null
cd kernel
make
cp bin/kernel.elf ../build > /dev/null
cd ../bootloader
make > /dev/null
cp bootx64.efi ../build > /dev/null
cd ..

dd if=/dev/zero of=$BUILDDIR/$OSNAME.img bs=512 count=93750
mformat -C -f 1440 -i $BUILDDIR/$OSNAME.img ::
mmd -i $BUILDDIR/$OSNAME.img ::/EFI
mmd -i $BUILDDIR/$OSNAME.img ::/EFI/BOOT
mcopy -i $BUILDDIR/$OSNAME.img $BUILDDIR/bootx64.efi ::/EFI/BOOT
mcopy -i $BUILDDIR/$OSNAME.img $BUILDDIR/kernel.elf ::
