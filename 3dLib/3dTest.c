#include "world.h"
#include <stdlib.h>
#include <stdio.h>
#define pi 3.1415926535
void set(float (*v)[3],float newV[3]) {
    (*v)[0] = newV[0];
    (*v)[1] = newV[1];
    (*v)[2] = newV[2];
};
void setT(unsigned char (*v)[3],unsigned char newV[3]) {
    (*v)[0] = newV[0];
    (*v)[1] = newV[1];
    (*v)[2] = newV[2];
};
int main() {
    printf("hello!");
    scr *screen = makeScreen(500,500);
    obj cube;
    cube.amtOfVerts = 8;
    cube.verts = (float (*)[3])malloc(24*sizeof(float));
    float (*v)[3] = cube.verts;
    set(v++,(float[]){-1.0,-1.0,1});
    set(v++,(float[]){1.0,-1.0,1});
    set(v++,(float[]){-1.0,1.0,1});
    set(v++,(float[]){1.0,1.0,1});
    set(v++,(float[]){-1.0,-1.0,-1});
    set(v++,(float[]){1.0,-1.0,-1});
    set(v++,(float[]){-1.0,1.0,-1});
    set(v,(float[]){1.0,1.0,-1});
    cube.amtOfTriangles = 12;
    cube.triangles = (unsigned char (*)[3])malloc(36*sizeof(char));
    unsigned char (*t)[3] = cube.triangles;
    setT(t++,(unsigned char[]){0,1,2});
    setT(t++,(unsigned char[]){3,1,2});

    setT(t++,(unsigned char[]){5,1,0});
    setT(t++,(unsigned char[]){0,4,5});
    
    setT(t++,(unsigned char[]){5,1,3});
    setT(t++,(unsigned char[]){5,3,7});

    setT(t++,(unsigned char[]){6,2,3});
    setT(t++,(unsigned char[]){6,3,7});

    setT(t++,(unsigned char[]){6,2,0});
    setT(t++,(unsigned char[]){4,6,0});

    setT(t++,(unsigned char[]){4,5,6});
    setT(t++,(unsigned char[]){5,6,7});
    ray camera;
    camera.pos[0] = 0; camera.pos[1] = 0; camera.pos[2] = -10; 
    camera.rotation[0] = 0; camera.rotation[1] = 0; camera.rotation[2] = 0;
    instance *objects = (instance *)malloc(1*sizeof(instance));
    objects->position.pos[0] = 0; objects->position.pos[1] = 0; objects->position.pos[2] = 0;
    objects->position.rotation[0] = 0; objects->position.rotation[1] = 0; objects->position.rotation[2] = 0;
    objects->object = &cube;
    char name[] = "Desktop/3dLib/render0.bmp";
    for(int i = 0; i < 10; i++) {
        name[20] = '0' + i;
        (objects->position.rotation)[0] = (pi/20)*i;
        addToScreen(1,objects,camera,screen);
        render(*screen, name);
        clear(screen);
    }
    printf("done");
    return 0;
};