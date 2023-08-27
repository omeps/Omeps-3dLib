#include "world.h"
#include <stdio.h>
#include <stdlib.h>
void render(scr s, char name[]) {
    printf("hello!\n");
    int chars = 3*s.length*s.width + 54;
    unsigned char *map = (unsigned char *)malloc((chars)*sizeof(char));
    unsigned char *ret = map;
    //header and stuff
    {
    //signature (2 bytes)
    
    *(map++) = 'B';
    *(map++) = 'M';
    //file size (4 bytes)
    *(map++) = chars%(1<<8);
    *(map++) = (chars%(1<<16))>>8;
    *(map++) = (chars%(1<<24))>>16;
    *(map++) = 0;
    // reserved field (4 bytes)
    *(map++) = 0;
    *(map++) = 0;
    *(map++) = 0;
    *(map++) = 0;
    //offset of pixel data (4 bytes)
    *(map++) = 54;
    *(map++) = 0;
    *(map++) = 0;
    *(map++) = 0;

    //header

    //header size
    *(map)++ = 40;
    *(map++) = 0;
    *(map++) = 0;
    *(map++) = 0;
    //width
    *(map++) = s.length%(1<<8);
    *(map++) = (s.length%(1<<16))>>8;
    *(map++) = (s.length%(1<<24))>>16;
    *(map++) = 0;
    //height
    *(map++) = s.width%(1<<8);
    *(map++) = (s.width%(1<<16))>>8;
    *(map++) = (s.width%(1<<24))>>16;
    *(map++) = 0;
    //reserved field
    *(map++) = 1;
    *(map++) = 0;
    //bits per pixel
    *(map++) = 24;
    *(map++) = 0;
    //compression method
    *(map++) = 0;
    *(map++) = 0;
    *(map++) = 0;
    *(map++) = 0;
    //size of pixel data
    *(map++) = (chars-54)%(1<<8);
    *(map++) = ((chars-54)%(1<<16))>>8;
    *(map++) = ((chars-54)%(1<<24))>>16;
    *(map++) = 0;
    //horizontal resolution
    *(map++) = 0;
    *(map++) = 0;
    *(map++) = 0b00110000;
    *(map++) = 0b10110001;
    //vertical resolution
    *(map++) = 0;
    *(map++) = 0;
    *(map++) = 0b00110000;
    *(map++) = 0b10110001;
    //color palette info
    *(map++) = 0;
    *(map++) = 0;
    *(map++) = 0;
    *(map++) = 0;
    //number of important colors
    *(map++) = 0;
    *(map++) = 0;
    *(map++) = 0;
    *(map++) = 0;
    }
    //pixel data
    int n = s.length*s.width*3;
    for(int i = 0; i < n; i++) {
        *(map++) = *(s.data+i);
    }
    FILE *file;
    file = fopen(name,"w+");
    for(int i = 0; i < chars; i++) {
        fputc(*(ret++),file);
    }
    fclose(file);
};
typedef struct mask {
    int length;
    int width;
    unsigned char *data;
} mask;
typedef struct zMask {
    int length;
    int width;
    float *distances;
} zMask;
scr *makeScreen(int length, int width) {
    scr *ret = (scr *)malloc(sizeof(scr));
    ret->length = length;
    ret->width = width;
    ret->data = (unsigned char *)calloc(3*length*width,3*length*width*sizeof(char));
    return ret;
};
void clear(scr *screen) {
   for(int i = 0; i < screen->length*screen->width*3; i++) {
        *(screen->data+i) = 0;
   }
}
mask Mask(int length, int width) {
    mask m;
    m.length = length;
    m.width = width;
    m.data = (unsigned char *)malloc(sizeof(char)*length*width);
    return m;
};
zMask ZMask(int length, int width) {
    zMask m;
    m.length = length;
    m.width = width;
    m.distances = (float *)malloc(sizeof(float)*length*width);
    return m;
};
void dot(mask m, int index) {
    *(m.data+(index>>3)) |= 1<<(index-((index>>3)<<3));
}
void zDot(zMask m, int x, int y, float dist) {
    *(m.distances+(x+y*(m.length))) = dist;
};
void drawLineToMask(mask m, int x1, int x2, int y1, int y2) {
    if(x1>x2) {
        drawLineToMask(m, x2, x1, y2, y1);
        return;
    }
    int dx = (x2-x1)<<1;
    int dy = (y2-y1)<<1;
    if(dy < 0) {
        dy = dy * -1;
        if(dx>=dy) {
        int error = dy-dx;
        int y = y1;
        for(int x = x1; x <= x2; x++) {
            if(0<=x & m.length>x & 0<=y & m.width>y) {
            dot(m,x+m.length*y);
            }
            error += dy;
            if(error >=0) {
                y--;
                error -= dx;
            }
        }
        } else {
        int error = dx-dy;
        int x = x1;
        for(int y = y1; y >= y2; y--) {
            if(0<=x & m.length>x & 0<=y & m.width>y) {
            dot(m,x+m.length*y);
            }
            error += dx;
            if(error >= 0) {
                x++;
                error -= dy;
            }
        }
        }
    }
    else if(dx>=dy) {
        int error = dy-dx;
        int y = y1;
        for(int x = x1; x <= x2; x++) {
            if(0<=x & m.length>x & 0<=y & m.width>y) {
            dot(m,x+m.length*y);
            }
            error += dy;
            if(error >=0) {
                y++;
                error -= dx;
            }
        }
    } else {
        int error = dx-dy;
        int x = x1;
        for(int y = y1; y <= y2; y++) {
            if(0<=x & m.length>x & 0<=y & m.width>y) {
            dot(m,x+m.length*y);
            }
            error += dx;
            if(error >= 0) {
                x++;
                error -= dy;
            }
        }
    }
};
void triangle(scr screen, int x[3], int y[3], unsigned char fillcolor[3], unsigned char border[3]) {
    int min_x = x[0];
    int max_x = x[0];
    if(x[1]<min_x) min_x = x[1]; else max_x = x[1];
    if(x[2]<min_x) min_x = x[2]; else if(max_x < x[2]) max_x = x[2];
    int min_y = y[0];
    int max_y = y[0];
    if(y[1]<min_y) min_y = y[1]; else max_y = y[1];
    if(y[2]<min_y) min_y = y[2]; else if(max_y < y[2]) max_y = y[2];
    if(min_y < 0) min_y = 0;
    if(max_y >= screen.width) max_y = screen.width - 1;
    mask m = Mask(max_x - min_x + 1, max_y - min_y + 1);
    drawLineToMask(m, x[0] - min_x, x[1] - min_x, y[0] - min_y, y[1] - min_y);
    drawLineToMask(m, x[1] - min_x, x[2] - min_x, y[1] - min_y, y[2] - min_y);
    drawLineToMask(m, x[2] - min_x, x[0] - min_x, y[2] - min_y, y[0] - min_y);
    min_y *= screen.length;
    max_y *= screen.length;
    int i = min_y;
    int mi = 0;
    while(i <= max_y) {
        int left = 0;
        int right = max_x-min_x;
        while((*(m.data + ((left + mi)>>3)) & (1<<(left+mi-(((left+mi)>>3)<<3)))) == 0) {
            left++;
        }
        if(left < -min_x) left = -min_x;
        while((*(m.data + ((right + mi)>>3)) & (1<<(right+mi-(((right+mi)>>3)<<3)))) == 0) {
            right--;
        }
        if(right > screen.length-max_x) right = screen.length - max_x;
        int j = left;
        while(j<=right) {
            if((*(m.data + ((j + mi)>>3)) & (1<<(j+mi-(((j+mi)>>3)<<3)))) == 0) {
                for(int k = 0; k < 3; k++) {
                    *(screen.data+3*(i+j+min_x)+k) = fillcolor[k];
                }
                
            } else {
                for(int k = 0; k < 3; k++) {
                    *(screen.data+3*(i+j+min_x)+k) = border[k];
                }
            }
            j++;
        }
        i += screen.length;
        mi += m.length;
    }
};
/*

void drawLineToZMask(zMask m, int x1, int x2, int y1, int y2, float d1, float d2) {
    if(x1>x2) {
        drawLine(m, x2, x1, y2, y1,d2,d1);
        return;
    };
    int dx = (x2-x1)<<1;
    int dy = (y2-y1)<<1;
    if(dy < 0) {
        dy = dy * -1;
        if(dx>=dy) {
        int error = dy-dx;
        int y = y1;
        for(int x = x1; x <= x2; x++) {
            if(0<=x & m.length>x & 0<=y & m.width>y) {
            zDot();
            }
            error += dy;
            if(error >=0) {
                y--;
                error -= dx;
            }
        }
        } else {
        int error = dx-dy;
        int x = x1;
        for(int y = y1; y >= y2; y--) {
            if(0<=x & m.length>x & 0<=y & m.width>y) {
            dot(m,x+m.length*y);printf("%d,%d\n",x,y);
            }
            error += dx;
            if(error >= 0) {
                x++;
                error -= dy;
            }
        }
        }
    }
    else if(dx>=dy) {
        int error = dy-dx;
        int y = y1;
        for(int x = x1; x <= x2; x++) {
            if(0<=x & m.length>x & 0<=y & m.width>y) {
            dot(m,x+m.length*y);printf("%d,%d\n",x,y);
            }
            error += dy;
            if(error >=0) {
                y++;
                error -= dx;
            }
        }
    } else {
        int error = dx-dy;
        int x = x1;
        for(int y = y1; y <= y2; y++) {
            if(0<=x & m.length>x & 0<=y & m.width>y) {
            dot(m,x+m.length*y);printf("%d,%d\n",x,y);
            }
            error += dx;
            if(error >= 0) {
                x++;
                error -= dy;
            }
        }
    }
};
*/
