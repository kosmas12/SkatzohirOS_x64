#include "uefi/uefi.h"
#include "../elfonashelf/elfonashelf.h"

typedef struct {
    uint64_t base;
    uint32_t pixelsPerScanLine;
    uint64_t size;
    uint32_t width;
    uint32_t height;
    uint8_t bytesPerPixel;
}Framebuffer;

efi_file_handle_t *loadFile(efi_file_handle_t *directory, wchar_t *path) {
    efi_file_handle_t *loadedFile;

    efi_loaded_image_protocol_t *loadedImage;
    efi_guid_t loadedImageGUID = EFI_LOADED_IMAGE_PROTOCOL_GUID;
    ST->BootServices->HandleProtocol(IM, &loadedImageGUID, (void**)&loadedImage);

    efi_simple_file_system_protocol_t *fileSystem;
    efi_guid_t fileSystemGUID = EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID;
    ST->BootServices->HandleProtocol(loadedImage->DeviceHandle, &fileSystemGUID, (void**)&fileSystem);

    if (!directory) {
        fileSystem->OpenVolume(fileSystem, &directory);
    }

    efi_status_t status = directory->Open(directory, &loadedFile, path, EFI_FILE_MODE_READ, EFI_FILE_READ_ONLY);
    if (status != EFI_SUCCESS) {
        return NULL;
    }
    return loadedFile;
}

void initFramebuffer(Framebuffer *framebuffer) {
    efi_gop_t *GOP;
    efi_guid_t GOP_GUID = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;

    efi_gop_mode_info_t *info = NULL;
    uintn_t infoSize = sizeof(efi_gop_mode_info_t);
    uint32_t numModes;
    efi_status_t status = BS->LocateProtocol(&GOP_GUID, NULL, (void **) &GOP);

    if (!EFI_ERROR(status) && GOP) {
        uint32_t mode = 0;
        if (GOP->Mode) {
            mode = GOP->Mode->Mode;
        }

        // Init to first mode if nothing is initialized
        status = GOP->QueryMode(GOP, mode, &infoSize, &info);
        if (!GOP->Mode) {
            status = GOP->SetMode(GOP, 0);
            ST->ConOut->Reset(ST->ConOut, 0);
            ST->StdErr->Reset(ST->StdErr, 0);
        }

        if (EFI_ERROR(status)) {
            printf("Unable to get native video mode info. Halting...\r\n");
            while (1);
        } else {
            numModes = GOP->Mode->MaxMode;

            // Look for a 1920x1080 32bpp mode
            for (int i = 0; i < numModes; ++i) {
                GOP->QueryMode(GOP, i, &infoSize, &info);

                if (info->HorizontalResolution == 1920 && info->VerticalResolution == 1080) {
                    GOP->SetMode(GOP, i);
                    unsigned int numPixels = info->HorizontalResolution * info->VerticalResolution;
                    // If already in a 1920x1080 32bpp mode exit the loop,
                    // else iterate through to the last 1920x1080 mode
                    if (numPixels == GOP->Mode->FrameBufferSize / 4) {
                        break;
                    }
                }
                else {
                    // If there is no 1920x1080 mode
                    if (i == numModes - 1) {
                        printf("No 1920x1080 video mode found, not changing.\r\n");
                    }
                }
            }
        }
        framebuffer->base = GOP->Mode->FrameBufferBase;
        framebuffer->size = GOP->Mode->FrameBufferSize;
        framebuffer->width = info->HorizontalResolution;
        framebuffer->height = info->VerticalResolution;
        framebuffer->bytesPerPixel = GOP->Mode->FrameBufferSize / (framebuffer->width * framebuffer->height);
        framebuffer->pixelsPerScanLine = GOP->Mode->Information->PixelsPerScanLine;
    }
    else {
        printf("The framebuffer could not be initialized. Halting...\r\n");
        while (1);
    }
}

// Normally one would need a self-made memcmp, but POSIX-UEFI gives us one already.
/*int memcmp(const void *chunk1, const void *chunk2, size_t numBytes) {
    uint8_t *chunk1Bytes = (uint8_t *)chunk1;
    uint8_t *chunk2Bytes = (uint8_t *)chunk2;
    for (int i = 0; i < numBytes; ++i) {
        if (chunk1Bytes[i] < chunk2Bytes[i]) {
            return -1;
        }
        else if (chunk1Bytes[i] > chunk2Bytes[i]) {
            return 1;
        }
    }
    return 0;
}*/

int main(int argc, char **argv) {
    ST->BootServices->SetWatchdogTimer(0, 0, 0, NULL);

    efi_file_handle_t *kernel = NULL;
    kernel = loadFile(NULL, L"kernel.elf");
    if (!kernel) {
        ST->ConOut->OutputString(ST->ConOut, L"Could not open kernel file\r\n");
        return !EFI_SUCCESS;
    }
    ST->ConOut->OutputString(ST->ConOut, L"Kernel file opened successfully\r\n");


    Elf64_Ehdr header;
    uintn_t size = sizeof(header);
    kernel->Read(kernel, &size, &header);

    if (
            memcmp(&header.e_ident[EI_MAG0], ELFMAG, SELFMAG) != 0 ||
            header.e_ident[EI_CLASS] != ELFCLASS64 ||
            header.e_ident[EI_DATA] != ELFDATA2LSB ||
            header.e_type != ET_EXEC ||
            header.e_machine != EM_X86_64 ||
            header.e_version != EV_CURRENT
            ) {
        ST->ConOut->OutputString(ST->ConOut, L"There is an issue with the kernel ELF header. Halting...\r\n");
        while (1);
    }
    else {
        ST->ConOut->OutputString(ST->ConOut, L"Kernel ELF header verified\r\n");
    }

    Elf64_Phdr *programHeaders;
    kernel->SetPosition(kernel, header.e_phoff);
    printf("Program Header Table offset of 0x%X. %d entries, %d bytes large each.\r\n", header.e_phoff, header.e_phnum, header.e_phentsize);
    size = header.e_phnum * header.e_phentsize;
    ST->BootServices->AllocatePool(EfiLoaderData, size, (void **)&programHeaders);
    if (kernel->Read(kernel, &size, programHeaders) != EFI_SUCCESS) {
        ST->ConOut->OutputString(ST->ConOut, L"There was an error reading the Program Header Table. Halting...\r\n");
        while (1);
    }

    for (Elf64_Phdr *programHeader = programHeaders;
    (char*)programHeader < (char*)programHeaders + header.e_phnum * header.e_phentsize;
    programHeader = (Elf64_Phdr*)((char*)programHeader + header.e_phentsize)) {
        switch (programHeader->p_type) {
            case PT_LOAD:
                // Allocate memory for the segment where it expects to be loaded
                int pages = (programHeader->p_memsz + 0x1000 - 1) / 0x1000;
                uint64_t segment = programHeader->p_paddr;
                ST->BootServices->AllocatePages(AllocateAddress, EfiLoaderData, pages, &segment);

                kernel->SetPosition(kernel, programHeader->p_offset);
                uintn_t segmentSize = programHeader->p_filesz;
                if(kernel->Read(kernel, &segmentSize, (void*)segment) != EFI_SUCCESS) {
                    ST->ConOut->OutputString(ST->ConOut, L"There was an error reading a program header. Halting...\r\n");
                    while (1);
                }
                break;
        }
    }

    ST->ConOut->OutputString(ST->ConOut, L"Kernel loaded\r\n");

    __attribute__((sysv_abi)) int (*kernelEntry)(Framebuffer) = ((__attribute__((sysv_abi)) int (*)(Framebuffer) ) header.e_entry);
    printf("Kernel entry point is 0x%X\r\n", header.e_entry);

    Framebuffer framebuffer;

    initFramebuffer(&framebuffer);

    printf("Kernel returned %d\r\n", kernelEntry(framebuffer));

    return EFI_SUCCESS;
}
