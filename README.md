First Game
----------

My first project in c++. based off https://www.parallelrealities.co.uk/tutorials/. I try to use camel case, RAII and 4 space indentation in the project as much as possible. 

To use build you should just have to download the dependencies below. And cmake will do the rest.

Dependencies
------------

libsdl2-2.0-0
libsdl2-dev
libsdl2-image-2.0-0
libsdl2-image-dev
libsdl2-ttf-2.0-0
libsdl2-ttf-dev
libsdl2-mixer-2.0-0
libsdl2-mixer-dev

Installing on linux
-------------------

	git clone https://github.com/Waahan/FirstGame
	cd FirstGame
	mkdir build
	cd build
	mkdir images
	cd images
	cp ../../images/* .
        cd ..
	cmake ..
	make
	./main

Thank You
---------

I would like to give a special thank you to my puppy Obi for helping me with this project. I love you puppy. And a thank you to the rest of the family. You really helped with this project.
