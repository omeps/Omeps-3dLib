#include "world.h"
#include <stdio.h>
#define header_size 15
void makeFile(obj o, unsigned char name[]) {
    int fileLength = o.amtOfTriangles * 3 + o.amtOfVerts * 12 + header_size;
    FILE *file = fopen(name,"w");
    fputc("O", file);
    fputc("b", file);
    fputc("j", file);
    fputc(fileLength-((fileLength>>8)<<8),file);
    fputc((fileLength>>8)-((fileLength>>16)<<8),file);
    fputc((fileLength>>16)-((fileLength>>24)<<16),file);
    fputc(fileLength>>24,file);
    fputc(o.amtOfTriangles-((o.amtOfTriangles>>8)<<8),file);
    fputc((o.amtOfTriangles>>8)-((o.amtOfTriangles>>16)<<8),file);
    fputc((o.amtOfTriangles>>16)-((o.amtOfTriangles>>24)<<16),file);
    fputc(o.amtOfTriangles>>24,file);
    fputc(o.amtOfVerts-((o.amtOfVerts>>8)<<8),file);
    fputc((o.amtOfVerts>>8)-((o.amtOfVerts>>16)<<8),file);
    fputc((o.amtOfVerts>>16)-((o.amtOfVerts>>24)<<16),file);
    fputc(o.amtOfVerts>>24,file);
    for(int i = 0; i < o.amtOfTriangles; i++) {
        fputc((*(o.triangles))[0],file);
        fputc((*(o.triangles))[1],file);
        fputc((*(o.triangles))[2],file);
    }
    char *dummy = (char *)o.verts;
    for(int i = 0; i <o.amtOfVerts*12; i++) {
        fputc(*(dummy++),file);
    }
    fclose(file);
};
obj getFile(unsigned char name[]) {
    FILE *f = fopen(name,"r");
    if(fgetc(f) != 'O') {
        return;
    }
    if(fgetc(f) != 'b') {
        return;
    }
    if(fgetc(f) != 'j') {
        return;
    }
    
};