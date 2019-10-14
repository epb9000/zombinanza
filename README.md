This is an old Fanfest Earthbound game project from over 15 years ago. I found the code
and decided to post it. The code itself is CC0, I have literally no commercial interest in
it. Even if I did, being a fan project, many of the resources are copy protected works, so there's also that.

You'll need MinGW and the following libraries to compile it:

FLTK (whichever version for Windows was around in 2004)

And... whichever others of these from the Makefile aren't available in the MinGW installer.
-mwindows -lwinmm -ldsound -lfltk -lstdc++ -lUUID -lole32 -lwsock32  --disable-runtime-pseudo-reloc  -lgmon -pg

(Note: As of this commit, I don't actually know if it will compile.)

And the you'll need to tweak the Makefile.
