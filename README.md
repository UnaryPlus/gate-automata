# Gate Automata

tga.c is a program for running ternary gate automata and storing the output in an SVG file. To compile, run `cc -o tga tga.c`. Then run `./tga` (or `./tga --help`) for information on how to use the program.

Here are some neat examples:
* `./tga 5005000 5 128 sierpinski.svg`
* `./tga 3506000 5 100 pyramid.svg`
* `./tga 3506000 50000000000000000000000000000000005 100 two-pyramids.svg`
* `./tga 5306000 5 50 ship.svg`
* `./tga 5306000 6 50 rake.svg`

The SVG files can be viewed by opening them in any web browser.
