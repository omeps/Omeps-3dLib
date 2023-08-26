exec : world.o 3dTest.o screenTools.o objectFiles.o
	cc -o exec world.o 3dTest.o screenTools.o objectFiles.o

world.o : world.h
3dTest.o : world.h
screenTools.o : world.h
objectFiles.o : world.h
.PHONY : clean
clean :
	rm exec world.o 3dTest.o