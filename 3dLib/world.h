#ifndef WORLD_H_
#define WORLD_H_
struct ray;
typedef struct ray {
    float pos[3];
    float rotation[3];
} ray;
struct obj;
typedef struct obj {
    int amtOfVerts;
    float (*verts)[3];
    int amtOfTriangles;
    unsigned char (*triangles)[3];
    unsigned char border[3];
    unsigned char fill[3];
} obj;
struct instance;
typedef struct instance {
    ray position;
    obj *object;
} instance;

struct scr;
typedef struct scr {
    unsigned char *data;
    int length;
    int width;
} scr;
void addToScreen(int amtOfObjects, instance *instances, ray camera, scr *screen);
void addObject(obj object, scr *screen);
scr *makeScreen(int length, int width);
void triangle(scr screen, int x[3], int y[3], unsigned char fillcolor[3], unsigned char border[3]);
void render(scr s, char name[]);
void clear(scr *screen);
#endif
