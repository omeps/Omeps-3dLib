#include "world.h"
#include <stdio.h>
#include <stdlib.h>
#define header_size 15
void makeFile(obj o, char *name) {
    FILE *file = fopen(name,"w");
    fputc('O', file);
    fputc('b', file);
    fputc('j', file);

    fputc(o.border[0],file);
    fputc(o.border[1],file);
    fputc(o.border[2],file);

    fputc(o.fill[0],file);
    fputc(o.fill[1],file);
    fputc(o.fill[2],file);
   
    fputc(o.amtOfTriangles-((o.amtOfTriangles>>8)<<8),file);
    fputc((o.amtOfTriangles>>8)-((o.amtOfTriangles>>16)<<8),file);
    fputc((o.amtOfTriangles>>16)-((o.amtOfTriangles>>24)<<16),file);
    fputc(o.amtOfTriangles>>24,file);
    fputc(o.amtOfVerts-((o.amtOfVerts>>8)<<8),file);
    fputc((o.amtOfVerts>>8)-((o.amtOfVerts>>16)<<8),file);
    fputc((o.amtOfVerts>>16)-((o.amtOfVerts>>24)<<16),file);
    fputc(o.amtOfVerts>>24,file);
    
    for(int i = 0; i < o.amtOfTriangles; i++) {
        fputc((*(o.triangles+i))[0],file);
        fputc((*(o.triangles+i))[1],file);
        fputc((*(o.triangles+i))[2],file);
    }
    
    char *dummy = (char *)o.verts;
    
    for(int i = 0; i <o.amtOfVerts*12; i++) {
        fputc(*(dummy++),file);
    }
    fclose(file);
};
obj getFile(char *name) {
    FILE *f = fopen(name,"r");
    obj o;
    if(fgetc(f) != 'O') {
        return o;
    }
    if(fgetc(f) != 'b') {
        return o;
    }
    if(fgetc(f) != 'j') {
        return o;
    }

    o.border[0] = fgetc(f); o.border[1] = fgetc(f); o.border[2] = fgetc(f);
    o.fill[0] = fgetc(f); o.fill[1] = fgetc(f); o.fill[2] = fgetc(f);

    o.amtOfTriangles = fgetc(f);
    o.amtOfTriangles += fgetc(f)<<8;
    o.amtOfTriangles += fgetc(f)<<16;
    o.amtOfTriangles += fgetc(f)<<24;
    
    o.amtOfVerts = fgetc(f);
    o.amtOfVerts += fgetc(f)<<8;
    o.amtOfVerts += fgetc(f)<<16;
    o.amtOfVerts += fgetc(f)<<24;

    o.triangles = (unsigned char (*)[3])malloc(o.amtOfTriangles*sizeof(unsigned char[3]));
    for(int i = 0; i < o.amtOfTriangles; i++) {
        (*(o.triangles+i))[0] = fgetc(f);
        (*(o.triangles+i))[1] = fgetc(f);
        (*(o.triangles+i))[2] = fgetc(f);
    }

    o.verts = (float (*)[3])malloc(o.amtOfVerts*sizeof(float[3]));
    float *v = (float *)o.verts;
    for(int i = 0; i < o.amtOfVerts; i++) {
        char buffer[12];
        for(int j = 0; j < 12; j++) {
            buffer[j] = fgetc(f);
        }
        float *b = (float *)&(buffer);
        *(v++) = *(b++);
        *(v++) = *(b++);
        *(v++) = *(b++);
    }
    return o;
};
