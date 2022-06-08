#ifndef MAPPER_H
#define MAPPER_H

#include <SDL2/SDL.h>
#include <iostream>

#define PRG_BANK_SIZE 0x4000

using namespace std;

// Possible mirroring modes for the nametables.
enum Mirroring {
    // VERTICAL
    // |--------|--------|
    // |-- 01 --|-- 02 --|
    // |--------|--------|
    // |=================|-V
    // |--------|--------|
    // |-- 01 --|-- 02 --|
    // |--------|--------|
    MIRROR_VERTICAL,
    // HORIZONTAL
    //          H
    // |--------|--------|
    // |-- 01 --|-- 01 --|
    // |--------|--------|
    // |=================|
    // |--------|--------|
    // |-- 02 --|-- 02 --|
    // |--------|--------|
    MIRROR_HORIZONTAL,
    // HORIZONTAL
    // |--------|--------|
    // |-- 01 --|-- 01 --|
    // |--------|--------|
    // |=================|
    // |--------|--------|
    // |-- 01 --|-- 01 --|
    // |--------|--------|
    MIRROR_SINGLE_LOWER,
    // HORIZONTAL
    // |--------|--------|
    // |-- 02 --|-- 02 --|
    // |--------|--------|
    // |=================|
    // |--------|--------|
    // |-- 02 --|-- 02 --|
    // |--------|--------|
    MIRROR_SINGLE_UPPER
};


// Abstract class used to store general information for every mapper.
class Mapper {
    public:
        uint8_t id;             // The id of the current Mapper.
        uint8_t PRG_banks;      // The amount of PRG banks.
        uint8_t CHR_banks;      // The amount of CHR banks.

        enum Mirroring mirroring;   // Mirroring type for the nametables.
        bool prg_ram_enabled;       // Whether PRG RAM is enabled, determines whether the cartridge uses persistent saves.

        // Contructor / Destructor.
        Mapper(uint8_t nPRG = 0, uint8_t nCHR = 0);
        ~Mapper();

        // Map a requested address to the actual address inside the cartridge for the CPU and PPU.
        // The argument 'write' is used to determine whether there is a read or write ongoing.
        virtual uint32_t cpuMap(uint16_t address, bool write, uint8_t value = 0x00) = 0;
        virtual uint32_t ppuMap(uint16_t address, bool write, uint8_t value = 0x00) = 0;
};


#endif
