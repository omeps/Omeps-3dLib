#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include "world.h"
typedef struct sortingkey {
    int index;
    float val;
} key;
float min(float a, float b, float c) {
    float s;
    if(b > a) s = b; else s = a;
    if(c > s) return c; else return s;
};
void swap(key *a, key *b) {
    key stored = *a;
    *a = *b;
    *b = stored;    
};

void sort(int length, key *objects) {
    if(length == 0 || length == 1) {
        return;
    }
    if(length == 2) {
        if(objects->val < (objects +1)->val) {
            swap(objects,objects+1);
        }
        return;
    }
    key *pivot = (objects+length-1);
    key *big = objects;
    key *little;
    while(big<pivot) {
        if(big->val < pivot->val) {
            little = big;
            little++;
            while(little<pivot && little->val > pivot->val) {
                little++;
            }
            swap(big,little);
            if(little == pivot) {
                pivot = big;
            }
        }
        big++;
    }
    sort(pivot-objects,objects);
    sort(length-(pivot-objects)-1, (pivot+1));


};
void rotate(float *a, float *b, float theta) {
    float newa = (*a * cos(theta)) + (*b * sin(theta));
    *b = (*a * sin(theta)) - (*b * cos(theta));
    *a = newa;
};
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
obj pack(instance object, ray camera) {
    obj packedObject;

    packedObject.amtOfTriangles = object.object->amtOfTriangles;
    packedObject.amtOfVerts = object.object->amtOfVerts;

    packedObject.border[0] = object.object->border[0];
    packedObject.border[1] = object.object->border[1];
    packedObject.border[2] = object.object->border[2];

    packedObject.fill[0] = object.object->fill[0];
    packedObject.fill[1] = object.object->fill[1];
    packedObject.fill[2] = object.object->fill[2];

    packedObject.verts = (float (*)[3])malloc(object.object->amtOfVerts*sizeof(float[3]));
    
    float (*packedV)[3] = packedObject.verts;
    float (*unpackedV)[3] = object.object->verts;
    for(int i = 0; i < packedObject.amtOfVerts; i++) {
        (*(packedV))[0] = (*(unpackedV))[0]; (*(packedV))[1] = (*unpackedV)[1]; (*packedV)[2] = (*unpackedV)[2];
        
        rotate(&((*(packedV))[0]),&((*(packedV))[1]),object.position.rotation[2]);
        rotate(&((*(packedV))[2]),&((*(packedV))[1]),object.position.rotation[1]);
        rotate(&((*(packedV))[0]),&((*(packedV))[2]),object.position.rotation[0]);

        (*(packedV))[0] += object.position.pos[0] - camera.pos[0]; (*packedV)[1] += object.position.pos[1] - camera.pos[1]; (*packedV)[2] -= object.position.pos[2] - camera.pos[2];

        rotate(&((*(packedV))[0]), &((*(packedV))[2]), camera.rotation[0]);
        rotate(&((*(packedV))[2]), &((*(packedV))[1]), camera.rotation[1]);
        rotate(&((*(packedV))[0]), &((*(packedV))[1]), camera.rotation[2]);

        packedV++;
        unpackedV++;
    }
    packedObject.triangles = object.object->triangles;
    return packedObject;
};
void freeUp(obj o) {
    free(o.verts);
};
void addObject(obj object, scr *screen) {
    key *keys = (key *)malloc(sizeof(key)*object.amtOfTriangles);
    for(int i = 0; i < object.amtOfTriangles; i++) {
        (keys+i)->index = i;
        (keys+i)->val = min((*(object.verts+((*(object.triangles+i))[0])))[2],(*(object.verts+((*(object.triangles+i))[1])))[2],(*(object.verts+((*(object.triangles+i))[2])))[2]);
    }
    sort(object.amtOfTriangles, keys);
    int start = 0;
    while(start < object.amtOfTriangles && (keys+start)->val < 0) start++; 
    for(int i = 0; i < object.amtOfTriangles; i++) {
        int x[3];
        int y[3];
        for(int j = 0; j < 3; j++) {
            x[j] = (int)(1000 * (*(object.verts+((*(object.triangles+((keys+i)->index)))[j])))[0]/(*(object.verts+((*(object.triangles+((keys+i)->index)))[j])))[2])+(screen->length>>1);
            y[j] = (int)(1000 * (*(object.verts+((*(object.triangles+((keys+i)->index)))[j])))[1]/(*(object.verts+((*(object.triangles+((keys+i)->index)))[j])))[2])+(screen->width>>1);
        }
        unsigned char fill[3] = {122,122,122};
        unsigned char border[3] = {255,255,255};
        triangle(*screen, x, y,fill,border);
    }
    free(keys);
};
void addToScreen(int amtOfObjects, instance *instances, ray camera, scr *screen) {
    /* convert to objects */

    obj *objList = (obj *)malloc(amtOfObjects*sizeof(obj));
    obj *o = objList;
    instance *in = instances;
    for(int i = 0; i < amtOfObjects; i++) {
        *(o++) = pack(*(in++),camera);
    }

    /* sort objects */ 

    key *keyList = (key *)malloc(amtOfObjects*sizeof(key));
    o = objList;
    {
        for(int i = 0; i < amtOfObjects; i++) {
            (keyList + i)->index = i;
            (keyList + i)->val = *(o->verts)[0];
            for(int j = 1; j < o->amtOfVerts; j++) {
                if(*(o->verts+j)[0] > (keyList + i)->val) (keyList + i)->val = *(o->verts+j)[0];
            }
        }
        sort(amtOfObjects, keyList);
    }
    /* print objects */
    key *k = keyList;
    int start = 0;
    while(start < amtOfObjects && (k++)->val < 0) start++;
    for(int i = start; i < amtOfObjects; i++) {

        addObject(*(objList+(k++)->index),screen);
    }
    /* free allocated data */
    free(keyList);
    o = objList;
    for(int i = 0; i < amtOfObjects; i++) {
        freeUp(*(o++));
    }
    free(objList);
    
};
