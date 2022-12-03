rm -r kernel/bin kernel/objs > /dev/null
rm -r build > /dev/null
cd bootloader
rm bootloader.o bootx64.efi > /dev/null
cd uefi
make clean > /dev/null
