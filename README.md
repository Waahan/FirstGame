First Game
----------

My first project in c++. based off https://www.parallelrealities.co.uk/tutorials/. 

To use build the project you must edit the CMakeLists.txt file to include your SDL2 image libary and SDL2 TTF libary. If you are on debain you can run sudo apt-file list "PACKAGE NAME" and include the file with the .so.0 at the end.

Dependencies
------------

libsdl2-image-2.0-0
libsdl2-ttf-2.0-0

Installing on linux
-------------------

	git clone https://github.com/Waahan/FirstGame
	cd FirstGame
	mkdir build
	cd build
	mkdir images
	cd ..
	cd images
	cp * /home/"$(whoami)"/FirstGame/build/images
	cmake ..
	make
	./main
