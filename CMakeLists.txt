cmake_minimum_required(VERSION 3.0)
project(SkatzohirOS_x64 C)

set(CMAKE_C_STANDARD 23)

add_executable(SkatzohirOS_x64 bootloader/bootloader.c kernel/src/kernel.c elfonashelf/elfonashelf.h)
