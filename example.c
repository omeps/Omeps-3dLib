#include "3dLib/world.h"
#include <stdlib.h>
int main() {
    obj o;
    o.amtOfVerts = 8;
    o.verts = (float (*)[3])malloc(o.amtOfVerts*sizeof(float[3]));
    float (*v)[3] = o.verts;
    set(v++,(float[]){1,1,1});
    set(v++,(float[]){-1,1,1});
    set(v++,(float[]){1,-1,1});
    set(v++,(float[]){-1,-1,1});
    set(v++,(float[]){1,1,-1});
    set(v++,(float[]){-1,1,-1});
    set(v++,(float[]){1,-1,-1});
    set(v++,(float[]){-1,-1,-1});
    o.amtOfTriangles = 12;
    o.triangles = (unsigned char (*)[3])malloc(o.amtOfVerts*sizeof(unsigned char[3]));
    unsigned char (*t)[3] = o.triangles;
    setT(t++,(unsigned char[3]){0,1,2});
    setT(t++,(unsigned char[3]){1,2,3});

    setT(t++,(unsigned char[3]){0,1,4});
    setT(t++,(unsigned char[3]){1,4,5});

    setT(t++,(unsigned char[3]){0,2,4});
    setT(t++,(unsigned char[3]){2,4,6});

    setT(t++,(unsigned char[3]){4,5,6});
    setT(t++,(unsigned char[3]){5,6,7});

    setT(t++,(unsigned char[3]){2,3,6});
    setT(t++,(unsigned char[3]){3,6,7});

    setT(t++,(unsigned char[3]){1,3,5});
    setT(t++,(unsigned char[3]){3,5,7});
    o.border[0] = 255; o.border[1] = 255; o.border[2] = 255; 
    o.fill[0] = 122; o.fill[1] = 122; o.fill[2] = 122; 
    instance instances[2];
    instances[0].object = &o;
    instances[0].position.pos[0] = 0.0f;
    instances[0].position.pos[1] = 0.0f;
    instances[0].position.pos[2] = 20.0f;
    instances[0].position.rotation[0] = 0;
    instances[0].position.rotation[1] = 0;
    instances[0].position.rotation[2] = 0;
    instances[1].object = &o;
    instances[1].position.pos[0] = 3.0f;
    instances[1].position.pos[1] = 0.0f;
    instances[1].position.pos[2] = 20.0f;
    instances[1].position.rotation[0] = 0;
    instances[1].position.rotation[1] = 0;
    instances[1].position.rotation[2] = 0;
    ray camera;
    camera.pos[0] = 0; camera.pos[1] = 0; camera.pos[2] = 0;
    camera.rotation[0] = 0; camera.rotation[1] = 0; camera.rotation[2] = 0;
    scr *screen = makeScreen(500,500);
    addToScreen(2,instances,camera,screen);
    render(*screen,"render.bmp");
    makeFile(o,"cube");
};
