# Cube-PS5

This simple payload draws a cube on the screen using software rendering. Developed with SDL2/C for jailbroken PS5 (3.xx-4xx).

Looking at the cube for a long time can cause an optical illusion...

## Prerequisites
- elfldr.elf
- shsrv.elf
- ftpsrv.elf

## How to Use
1. Send the elfldr.elf payload to port 9020.
2. Send the shsrv.elf payload to port 9021.
3. Send the ftpsrv.elf payload to port 9021.
4. Copy the  cube.elf to `/data` directory on your PS5.
5. Execute `telnet [PS5-IP] 2323`.
6. cd `/data`.
6. Run the command `hbldr cube.elf`.

## Credits
Credits to J. Tornblom for the SDK, SDL2 port for PS5, and required payloads.
