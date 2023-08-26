3dlib.a : world.o screenTools.o objectFiles.o
	ar cr 3dLib.a world.o screenTools.o objectFiles.o

world.o : world.h
screenTools.o : world.h
objectFiles.o : world.h
.PHONY : clean
clean :
	rm exec world.o 3dTest.o
