#include "3dLib/world.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#define PI 3.1415926535
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
    o.triangles = (unsigned char (*)[3])malloc(o.amtOfTriangles*sizeof(unsigned char[3]));
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
    obj circle;
    circle.amtOfVerts = 82;
    circle.verts = (float (*)[3])malloc(circle.amtOfVerts*sizeof(float[3]));
    v = circle.verts;
    set(v++,(float[]){0,1,0});
    for(int i = 1; i < 9; i++) {
        for(int j = 0; j < 10; j++) {
            float point[3];
            point[0] = sin(PI * i / 9) * cos(2 * PI * j / 10);
            point[1] = cos(PI * i / 9); 
            point[2] = sin(PI * i / 9) * sin(2 * PI * j / 10); 

            set(v++, point);
        }
    }
    set(v++, (float[]){0,-1,0});
    printf("hello\n");
    circle.amtOfTriangles = 160;
    circle.triangles = (unsigned char (*)[3])malloc(circle.amtOfTriangles*sizeof(unsigned char[3]));
    t = circle.triangles;
    for(int i = 0; i < 10; i++) {
        unsigned char  in[3];
        in[0] = 0;
        in[1] = i + 1;
        in[2] = (i+1)%10 + 1;
        setT(t++, in);
    } 
    for(int i = 0; i < 7; i++) {
        for(int j = 0; j < 10; j++) {
            unsigned char  in[3];
            in[0] = i*10 + j + 1;
            in[1] = (i+1)*10 + j + 1;
            in[2] = (i+1)*10 + (j+1)%10 + 1;
            setT(t++, in);
            in[1] = (i)*10 + (j+1)%10 + 1;
            setT(t++, in);
        }
    } 
    for(int i = 0; i < 10; i++) {
        unsigned char  in[3];
        in[0] = 81;
        in[1] = 70 + i + 1;
        in[2] = 70 + (i+1)%10 + 1;
        setT(t++, in);
    } 
    circle.border[0] = 255; circle.border[1] = 255; circle.border[2] = 255; 
    circle.fill[0] = 122; circle.fill[1] = 122; circle.fill[2] = 122; 
    instance instances[3];
    instances[0].object = &o;
    instances[0].position.pos[0] = 0.0f;
    instances[0].position.pos[1] = 0.0f;
    instances[0].position.pos[2] = 5.0f;
    instances[0].position.rotation[0] = 0;
    instances[0].position.rotation[1] = 0;
    instances[0].position.rotation[2] = 0;
    instances[1].object = &circle;
    instances[1].position.pos[0] = 3.0f;
    instances[1].position.pos[1] = 0.0f;
    instances[1].position.pos[2] = 5.0f;
    instances[1].position.rotation[0] = 0;
    instances[1].position.rotation[1] = 0;
    instances[1].position.rotation[2] = 0;
    instances[2].object = &circle;
    instances[2].position.pos[0] = -2.0f;
    instances[2].position.pos[1] = -2.0f;
    instances[2].position.pos[2] = 4.0f;
    instances[2].position.rotation[0] = 0.1;
    instances[2].position.rotation[1] = 0;
    instances[2].position.rotation[2] = 0;
    ray camera;
    camera.pos[0] = 0; camera.pos[1] = 0; camera.pos[2] = 0;
    camera.rotation[0] = 0; camera.rotation[1] = 0; camera.rotation[2] = 0;
    char name[] = "render.bmp";
    scr *screen = makeScreen(500,500);
    addToScreen(3,instances,camera,screen);
    render(*screen,name);
    free(screen->data); free(screen);
    makeFile(o,"cube.odat");
    makeFile(circle, "circle.odat");
};
