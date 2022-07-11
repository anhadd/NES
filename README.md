## NES Emulator
A NES Emulator in C++ using SDL2.


#### How to Run
From the root directory, run:  
`make all`  
Then run the created main binary with:  
`./main <ROM file>`

#### Controls
Button      | Mapped To
----------- | ------------
**A**       | Z
**B**       | X
**UP**      | Arrow Up
**DOWN**    | Arrow Down
**LEFT**    | Arrow Left
**RIGHT**   | Arrow Right
**Start**   | Enter
**Select**  | Left Shift
**Reset**   | R

The speed of the running ROM can also be changed with:  
`9`: Decrease speed.  
`0`: Increase speed.

To change the current color palette use:\
`P`: Cycle through palettes.

To quit the emulator use:\
`ESC`: Closes the emulator.

Others:\
`Space`: Pause the emulator.\
`F`: Execute a single frame.

Disabled By Default:\
`L`: Log debug information to file.
