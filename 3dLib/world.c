#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include "world.h"
#define LUMINOSITY 0.3
typedef struct sortingkey {
    int index;
    float val;
} key;
float max(float a, float b, float c) {
    float s;
    if(b > a) s = b; else s = a;
    if(c > s) return c; else return s;
};
float max2(float a, float b) {
    if(a > b) return a; else return b;
};
float min(float a, float b, float c) {
    float s;
    if(b < a) s = b; else s = a;
    if(c < s) return c; else return s;
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
    float *maxList = (float *)malloc(sizeof(float)*object.amtOfTriangles);
    float (*normals)[3] =  (float (*)[3])calloc(object.amtOfVerts, sizeof(float[3]));
    for(int i = 0; i < object.amtOfTriangles; i++) {
        (keys+i)->index = i;
        (keys+i)->val = min((*(object.verts+((*(object.triangles+i))[0])))[2],(*(object.verts+((*(object.triangles+i))[1])))[2],(*(object.verts+((*(object.triangles+i))[2])))[2]);
        *(maxList+i) = max((*(object.verts+((*(object.triangles+i))[0])))[2],(*(object.verts+((*(object.triangles+i))[1])))[2],(*(object.verts+((*(object.triangles+i))[2])))[2]);
        float a[3];
        float b[3];
        for(int j = 0; j < 3; j++) {
            a[j] = (*(object.verts+((*(object.triangles+i))[2])))[j] - (*(object.verts+((*(object.triangles+i))[1])))[j];
            b[j] = (*(object.verts+((*(object.triangles+i))[2])))[j] - (*(object.verts+((*(object.triangles+i))[0])))[j];
        }
        float normal[3];
        normal[0] = a[1] * b[2] - a[2] * b[1];
        normal[1] = a[2] * b[0] - a[0] * b[2];
        normal[2] = a[0] * b[1] - a[1] * b[0];
        for(int j = 0; j < 3; j++) {
            for(int k = 0; k < 3; k++) {
                (*(normals + (*(object.triangles+i))[j]))[k] += normal[k];
            }
        }

    }
    for(int i = 0; i < object.amtOfVerts; i++) {
        printf("%f\n",(*(normals+i))[1]);
    }
    sort(object.amtOfTriangles, keys);
    float currentMaxValue = *(maxList + (keys + object.amtOfTriangles - 1)->index);
    bool zCull[object.amtOfTriangles - 1]; 
    for(int i = object.amtOfTriangles - 2; i >= 0; i--) {
        zCull[i] = currentMaxValue > (keys + i)->val;
        if(currentMaxValue < *(maxList + (keys + i)->index)) {
            currentMaxValue = *(maxList + (keys + i)->index);
        }
    }
    zMask cullMask = ZMask(screen->length, screen->width);
    for(int i = 0; i < object.amtOfTriangles - 1; i++) {
        #define valOfTri(i_,vertice, component) (*(object.verts+((*(object.triangles+(keys+i_)->index))[vertice])))[component]
        #define getVec(index,n,v) (valOfTri(index,0,v) - valOfTri(index,n,v))
        float normal[3];
        normal[0] = getVec(i,1,1) * getVec(i,2,2) - getVec(i,1,2) * getVec(i,2,1);
        normal[1] = getVec(i,1,2) * getVec(i,2,0) - getVec(i,1,0) * getVec(i,2,2);
        normal[2] = getVec(i,1,0) * getVec(i,2,1) - getVec(i,1,1) * getVec(i,2,0);
        if(zCull[i] || (i != 0 && zCull[i-1])) {
            int x[3];
            int y[3];
            float z[3];
            for(int j = 0; j < 3; j++) {
                x[j] = (int)((screen->length>>2) * (*(object.verts+((*(object.triangles+((keys+i)->index)))[j])))[0]/(*(object.verts+((*(object.triangles+((keys+i)->index)))[j])))[2])+(screen->length>>1);
                y[j] = (int)((screen->width>>2) * (*(object.verts+((*(object.triangles+((keys+i)->index)))[j])))[1]/(*(object.verts+((*(object.triangles+((keys+i)->index)))[j])))[2])+(screen->width>>1);
                z[j] = (*(object.verts+((*(object.triangles+((keys+i)->index)))[j])))[2];
            }
            unsigned char fill[3];
            unsigned char border[3];
            #define getSquare(_index, vert, k) pow((*(normals + *(object.triangles+((keys+_index)->index))[vert]))[k],2)
            #define magnitude(index,vert) sqrt(getSquare(index,vert,0)+getSquare(index,vert,1)+getSquare(index,vert,2))
            float dotProd = max2(0,(*(normals + (*(object.triangles+((keys+i)->index)))[0]))[1] / magnitude(i,0) + (*(normals + (*(object.triangles+((keys+i)->index)))[1]))[1] / magnitude(i,1) + (*(normals + (*(object.triangles+((keys+i)->index)))[2]))[1] / magnitude(i,2));
            dotProd /= 3;
            for(int j = 0; j < 3; j++) {
                fill[j] = (unsigned char) (object.fill[j]) * (dotProd*(1-LUMINOSITY) + LUMINOSITY);
                border[j] = (unsigned char) (object.border[j]) * (dotProd*(1-LUMINOSITY) + LUMINOSITY);
            }
            zDrawTriangle(cullMask, screen, x, y, z, fill,border);
        } else {
            int x[3];
            int y[3];
            for(int j = 0; j < 3; j++) {
                x[j] = (int)((screen->length>>2) * (*(object.verts+((*(object.triangles+((keys+i)->index)))[j])))[0]/(*(object.verts+((*(object.triangles+((keys+i)->index)))[j])))[2])+(screen->length>>1);
                y[j] = (int)((screen->width>>2) * (*(object.verts+((*(object.triangles+((keys+i)->index)))[j])))[1]/(*(object.verts+((*(object.triangles+((keys+i)->index)))[j])))[2])+(screen->width>>1);
            }
            unsigned char fill[3];
            unsigned char border[3];
            #define getSquare(_index, vert, k) pow((*(normals + *(object.triangles+((keys+_index)->index))[vert]))[k],2)
            #define magnitude(index,vert) sqrt(getSquare(index,vert,0)+getSquare(index,vert,1)+getSquare(index,vert,2))
            float dotProd = max2(0,(*(normals + (*(object.triangles+((keys+i)->index)))[0]))[1] / magnitude(i,0) + (*(normals + (*(object.triangles+((keys+i)->index)))[1]))[1] / magnitude(i,1) + (*(normals + (*(object.triangles+((keys+i)->index)))[2]))[1] / magnitude(i,2));
            dotProd /= 3;
            for(int j = 0; j < 3; j++) {
                fill[j] = (unsigned char) (object.fill[j]) * (dotProd*(1-LUMINOSITY) + LUMINOSITY);
                border[j] = (unsigned char) (object.border[j]) * (dotProd*(1-LUMINOSITY) + LUMINOSITY);
            }
            triangle(*screen, x, y,fill,border);
        }
    }
    int i = object.amtOfTriangles - 1;
    #define valOfTri(i_,vertice, component) (*(object.verts+((*(object.triangles+(keys+i_)->index))[vertice])))[component]
    #define getVec(index,n,v) (valOfTri(index,0,v) - valOfTri(index,n,v))
    float normal[3];
    normal[0] = getVec(i,1,1) * getVec(i,2,2) - getVec(i,1,2) * getVec(i,2,1);
    normal[1] = getVec(i,1,2) * getVec(i,2,0) - getVec(i,1,0) * getVec(i,2,2);
    normal[2] = getVec(i,1,0) * getVec(i,2,1) - getVec(i,1,1) * getVec(i,2,0);
    if(zCull[i-1]) {
            int x[3];
            int y[3];
            float z[3];
            for(int j = 0; j < 3; j++) {
                x[j] = (int)((screen->length>>2) * (*(object.verts+((*(object.triangles+((keys+i)->index)))[j])))[0]/(*(object.verts+((*(object.triangles+((keys+i)->index)))[j])))[2])+(screen->length>>1);
                y[j] = (int)((screen->width>>2) * (*(object.verts+((*(object.triangles+((keys+i)->index)))[j])))[1]/(*(object.verts+((*(object.triangles+((keys+i)->index)))[j])))[2])+(screen->width>>1);
                z[j] = (*(object.verts+((*(object.triangles+((keys+i)->index)))[j])))[2];
            }
            unsigned char fill[3];
            unsigned char border[3];
            #define getSquare(_index, vert, k) pow((*(normals + *(object.triangles+((keys+_index)->index))[vert]))[k],2)
            #define magnitude(index,vert) sqrt(getSquare(index,vert,0)+getSquare(index,vert,1)+getSquare(index,vert,2))
            float dotProd = max2((*(normals + (*(object.triangles+((keys+i)->index)))[0]))[1] / magnitude(i,0) + (*(normals + (*(object.triangles+((keys+i)->index)))[1]))[1] / magnitude(i,1) + (*(normals + (*(object.triangles+((keys+i)->index)))[2]))[1] / magnitude(i,2),0);
            dotProd /= 3;
            for(int j = 0; j < 3; j++) {
                fill[j] = (unsigned char) (object.fill[j]) * (dotProd*(1-LUMINOSITY) + LUMINOSITY);
                border[j] = (unsigned char) (object.border[j]) * (dotProd*(1-LUMINOSITY) + LUMINOSITY);
            }
            zDrawTriangle(cullMask, screen, x, y, z, fill,border);
        } else {
            int x[3];
            int y[3];
            for(int j = 0; j < 3; j++) {
                x[j] = (int)((screen->length>>2) * (*(object.verts+((*(object.triangles+((keys+i)->index)))[j])))[0]/(*(object.verts+((*(object.triangles+((keys+i)->index)))[j])))[2])+(screen->length>>1);
                y[j] = (int)((screen->width>>2) * (*(object.verts+((*(object.triangles+((keys+i)->index)))[j])))[1]/(*(object.verts+((*(object.triangles+((keys+i)->index)))[j])))[2])+(screen->width>>1);
            }
            unsigned char fill[3];
            unsigned char border[3];
            #define getSquare(_index, vert, k) pow((*(normals + *(object.triangles+((keys+_index)->index))[vert]))[k],2)
            #define magnitude(index,vert) sqrt(getSquare(index,vert,0)+getSquare(index,vert,1)+getSquare(index,vert,2))
            float dotProd = max2(0,(*(normals + (*(object.triangles+((keys+i)->index)))[0]))[1] / magnitude(i,0) + (*(normals + (*(object.triangles+((keys+i)->index)))[1]))[1] / magnitude(i,1) + (*(normals + (*(object.triangles+((keys+i)->index)))[2]))[1] / magnitude(i,2));
            dotProd /= 3;
            for(int j = 0; j < 3; j++) {
                fill[j] = (unsigned char) (object.fill[j]) * (dotProd*(1-LUMINOSITY) + LUMINOSITY);
                border[j] = (unsigned char) (object.border[j]) * (dotProd*(1-LUMINOSITY) + LUMINOSITY);
            }
            triangle(*screen, x, y,fill,border);
        }
    free(keys); free(maxList); free(cullMask.distances);
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
    float *minValList = (float *)malloc(amtOfObjects*sizeof(float));
    {
        for(int i = 0; i < amtOfObjects; i++) {
            (keyList + i)->index = i;
            (keyList + i)->val = *((o+i)->verts)[0];
            *(minValList+i) = *((o+i)->verts)[0];
            for(int j = 1; j < (o+i)->amtOfVerts; j++) {
                if(*((o+i)->verts+j)[0] > (keyList + i)->val) 

                (keyList + i)->val = *((o+i)->verts+j)[0];

                else if(*((o+i)->verts+j)[0] < (keyList + i)->val)

                *(minValList+i) = *((o+i)->verts+j)[0];
            }
        }
        sort(amtOfObjects, keyList);
    }
    /* print objects */
    key *k = keyList;
    int start = 0;
    while(start < amtOfObjects && (k++)->val < 0) start++;
    k--;
    for(int i = start; i < amtOfObjects; i++) {

        addObject(*(objList+(k++)->index),screen);
    }
    /* free allocated data */
    free(keyList); free(minValList);
    o = objList;
    for(int i = 0; i < amtOfObjects; i++) {
        freeUp(*(o++));
    }
    free(objList);
    
};
