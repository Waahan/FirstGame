First Game
---------

My first project in c++. based off https://www.parallelrealities.co.uk/tutorials/. 

To use this you must edit the cmakelists file to include your SDL2 iamge libary 

Installing on linux
--------

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
