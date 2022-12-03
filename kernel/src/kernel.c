//
// Created by kosmas on 29/10/2022.
//

#include <stdint.h>

typedef struct {
    uint64_t base;
    uint32_t pixelsPerScanLine;
    uint64_t size;
    uint32_t width;
    uint32_t height;
    uint8_t bytesPerPixel;
}Framebuffer;

static inline void plotPixel32(unsigned int x, unsigned int y, uint32_t color, Framebuffer *framebuffer) {
    *((uint32_t*)(framebuffer->base + 4 * framebuffer->pixelsPerScanLine * y + 4 * x)) = color;
}

static inline uint32_t hexToRGBA(uint8_t red, uint8_t green, uint8_t blue) {
    uint32_t final = 0;
    final |= red << 16;
    final |= green << 8;
    final |= blue;
    return final;
}

int _start(Framebuffer framebuffer) {

    unsigned int width = framebuffer.width;
    unsigned int height = framebuffer.height;

    for (unsigned int x = 0; x < width; ++x) {
        for (unsigned int y = 0; y < height; ++y) {
            // Alternatively, a nice blue: 0x09, 0x79, 0xE8, 0xFF
            plotPixel32(x, y, hexToRGBA(0x35, 0x03, 0x59), &framebuffer);
        }
    }

    while (1);

    return 0;

}
