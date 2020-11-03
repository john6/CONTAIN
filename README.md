# CONTAIN
2D Physics Engine, and version one of a simple arcade game


#Mac and Linux Port

#Mac instructions

instructions to run on Mac currently

1. root directory: run “cmake -target install -build . “
2. run “make”
3. run make install
4. run cd “CONTAIN_RELEASE”
5. run “./CONTAIN_exe”


next step is to figure out how to make it a normal clickable program.
Right now it wont allow SFML to access resources properly because the filepath is not relative. 
ResourcePath will help but I got a linker error last time. as of right now it works!
