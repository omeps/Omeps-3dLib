this is a simple 3d Graphics library made by @omeps as a side project. 

Current features: 

Creating .bmp files
creating textureless objects and adding them to a 'screen' (there isn't any z-culling yet and some bugs need fixing, so there are some errors)
rotating and moving objects (no scaling yet)
creating and reading file versions of objects

It's currently under development by @omeps alone.

HOW TO GET THE LIBRARY (mac):
-First, read the code! You never know what might happen if you run someone you dont know's code without reading it.

-Download the repository
-Open terminal at the '3dLib' folder
-Run 'make'
-Move the inner '3dLib' folder to your project
-add the header to your c files: Add '#include "3dLib/world.h"' to your project
-Remember to include '3dLib/3dLib.a' when you compile

-For the example code, open terminal at the repositories' folder and run 'gcc example.c 3dLib/3dLib.a -o executable after running make to get the .exe file 

how it runs:

This library uses objects, rays, and instances. 
An object stores vertices and triangles, a ray stores a position and rotation, and an instance stores a ray and a reference to an object.
you can add instances to the screen using addToScreen(), which first converts the instances to objects based on a camera ray, then sorts them, and adds them to the screen.

You should use instances for all of your actual coding and only use objects when you create them and create instances of them because objects are difficult to work with. They're much harder to move and rotate.
