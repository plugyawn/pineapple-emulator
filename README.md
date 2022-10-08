[![forthebadge made-with-python](https://forthebadge.com/images/badges/built-with-love.svg)](https://www.python.org/)
[![forthebadge](https://forthebadge.com/images/badges/made-with-c-plus-plus.svg)](https://forthebadge.com)

<div align = center>
<a href = "github.com/plugyawn"><img width="400px" src= "https://user-images.githubusercontent.com/76529011/194703131-31f83e29-e1cc-4877-ab12-e358750ab697.png"></a>
</div>

## ```pineapple-EMU``` is a test ```MOS 6502``` processor emulator for NES ROMs.

We wanted to play Mario on the NES but couldn't, so we wrote an emulator for the NES on our own -- or tried to, anyway. This is an abridged version of ```OneLoneCoder```'s tutorial on a complete NES emulation, implemented and tested for the ```MOS 6502``` chip aboard the original NES.

The snapshot below captures our emulator using the ```OLCPixelEngine``` to blit a live-feed of the emulation process.

![Our NES Emulator](https://user-images.githubusercontent.com/76529011/194702025-4a96385f-1cfb-4b26-83f1-acc92824bba5.png)

## Aside: A little note from the devs.

Note that While we have a functioning Picture Processing Unit too, we were not able (yet) to connect the PPU
to the cartridge, which is how sprites are blitted onto the screen.
 
In essence, each peripheral of the NES is to be thought of as an object connected to the Bus, which
as most online resources rightly call, is the heart of the architecture of the NES.

We have emulated the processor (the ’6502), the PPU (the ’2C02), as well as the general functioning
of the cartridges and the different kinds of RAM accessed by the bus during the working of the NES.
Right now, our emulator can output to some degree of accuracy the functioning of the processor when
given a .NES file, which is essentially a dump from a physical NES cartridge. We have tested it out with
the NES testing suite, and our results corroborate well with the sources that we referred to. We believe our
emulation is acccurate.

Paired with a disassembler function, our project is a good way to look at actual programs from the
1980s and see how they were written and structured.

The project would not have been possible without OneLoneCoder’s PixelGameEngine, which is an
open-source tool that we used to display our emulated data on the screen.



