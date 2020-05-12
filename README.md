# stars

 This repo contains code related to simulate moving through a starfield in C.
I originally wrote 'stars,' (now called sstars) using SDL. Next, I moved
the logic for generating and rendering a starfield onto a 2D plane into a
library and built xstars. xstars is an X client app that draws a starfield
into the root window. Most recently, I've been experimenting with an ncurses
frontend with an IRC Client like interface.

 This is a toy and probably not very useful to anyone, but what the hell...

# building

    sudo apt-get install build-essential libsdl1.2-dev libncurses-dev libcunit1-dev
    make all
    make check
    sudo make install
