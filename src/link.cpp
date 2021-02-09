#include "link.h"

Link::Link(float ep_link, float r_link, float edge, float col_r, float col_g, float col_b){

    float l_link = r_link - edge/2;

    /* FACE 1 */

    // Point A
    createVertice(0, -r_link/2, l_link/2, ep_link/2);
    createSide(0, 0, -r_link/2, l_link/2, ep_link/2); //side 1
    createSide(7, 3, -r_link/2, l_link/2, ep_link/2); //side 1

    createColor(0, col_r, 0.2*col_g, 0.2*col_b);


    // B
    createVertice(1, -l_link/2, r_link/2, ep_link/2);
    createSide(0, 3, -l_link/2, r_link/2, ep_link/2); //side 1
    createSide(1, 0, -l_link/2, r_link/2, ep_link/2); //side 2

    createColor(1, col_r, col_g, col_b);


    // C
    createVertice(2, l_link/2, r_link/2, ep_link/2);
    createSide(1, 3, l_link/2, r_link/2, ep_link/2); //side2
    createSide(2, 0, l_link/2, r_link/2, ep_link/2); //side3

    createColor(2, col_r, col_g, col_b);


    // D
    createVertice(3, r_link/2, l_link/2, ep_link/2);
    createSide(2, 3, r_link/2, l_link/2, ep_link/2); //side3
    createSide(3, 0, r_link/2, l_link/2, ep_link/2); //side4

    createColor(3, col_r, col_g, col_b);


    // E
    createVertice(4, r_link/2, -l_link/2, ep_link/2);
    createSide(3, 3, r_link/2, -l_link/2, ep_link/2); //side4
    createSide(4, 0, r_link/2, -l_link/2, ep_link/2); //side5

    createColor(4, col_r, col_g, col_b);

    // F
    createVertice(5, l_link/2, -r_link/2, ep_link/2);
    createSide(4, 3, l_link/2, -r_link/2, ep_link/2); //side5
    createSide(5, 0, l_link/2, -r_link/2, ep_link/2); //side6

    createColor(5, col_r, col_g, col_b);


    // G
    createVertice(6, -l_link/2, -r_link/2, ep_link/2);
    createSide(5, 3, -l_link/2, -r_link/2, ep_link/2);
    createSide(6, 0, -l_link/2, -r_link/2, ep_link/2);

    createColor(6, col_r, col_g, col_b);


    // H
    createVertice(7, -r_link/2, -l_link/2, ep_link/2);
    createSide(6, 3, -r_link/2, -l_link/2, ep_link/2);
    createSide(7, 0, -r_link/2, -l_link/2, ep_link/2);

    createColor(7, col_r, col_g, col_b);


    /* FACE 2 */

    // Point A'
    createVertice(8, -r_link/2, l_link/2, -ep_link/2);
    createSide(0, 1, -r_link/2, l_link/2, -ep_link/2); //side 1
    createSide(7, 2, -r_link/2, l_link/2, -ep_link/2); //side 1

    createColor(8, col_r, 0.2*col_g, 0.2*col_b);

    // B'
    createVertice(9, -l_link/2, r_link/2, -ep_link/2);
    createSide(0, 2, -l_link/2, r_link/2, -ep_link/2); // side 1
    createSide(1, 1, -l_link/2, r_link/2, -ep_link/2); // side 2

    createColor(9, col_r, col_g, col_b);


    // C'
    createVertice(10, l_link/2, r_link/2, -(ep_link/2));
    createSide(1, 2, l_link/2, r_link/2, -(ep_link/2));
    createSide(2, 1, l_link/2, r_link/2, -(ep_link/2));

    createColor(10, col_r, col_g, col_b);

    // D'
    createVertice(11, r_link/2, l_link/2, -(ep_link/2));
    createSide(2, 2, r_link/2, l_link/2, -(ep_link/2));
    createSide(3, 1, r_link/2, l_link/2, -(ep_link/2));

    createColor(11, col_r, col_g, col_b);


    // E'
    createVertice(12, r_link/2, -l_link/2, -(ep_link/2));
    createSide(3, 2, r_link/2, -l_link/2, -(ep_link/2));
    createSide(4, 1, r_link/2, -l_link/2, -(ep_link/2));

    createColor(12, col_r, col_g, col_b);

    // F'
    createVertice(13, l_link/2, -r_link/2, -(ep_link/2));
    createSide(4, 2, l_link/2, -r_link/2, -(ep_link/2));
    createSide(5, 1, l_link/2, -r_link/2, -(ep_link/2));

    createColor(13, col_r, col_g, col_b);


    // G'
    createVertice(14, -l_link/2, -r_link/2, -(ep_link/2));
    createSide(5, 2, -l_link/2, -r_link/2, -(ep_link/2));
    createSide(6, 1, -l_link/2, -r_link/2, -(ep_link/2));

    createColor(14, col_r, col_g, col_b);


    // H'
    createVertice(15, -r_link/2, -l_link/2, -(ep_link/2));
    createSide(6, 2, -r_link/2, -l_link/2, -(ep_link/2));
    createSide(7, 1, -r_link/2, -l_link/2, -(ep_link/2));

    createColor(15, col_r, col_g, col_b);
}

void Link::createVertice(int index, float x, float y, float z){
    vertices[3 * index]     = x;
    vertices[3 * index + 1] = y;
    vertices[3 * index + 2] = z;
}

void Link::createSide(int number, int index, float x, float y, float z){
    switch(number){
    case 0:
        side1[3*index]     = x;
        side1[3*index + 1] = y;
        side1[3*index + 2] = z;
        break;

    case 1:
        side2[3*index]     = x;
        side2[3*index + 1] = y;
        side2[3*index + 2] = z;
        break;

    case 2:
        side3[3*index]     = x;
        side3[3*index + 1] = y;
        side3[3*index + 2] = z;
        break;

    case 3:
        side4[3*index]     = x;
        side4[3*index + 1] = y;
        side4[3*index + 2] = z;
        break;

    case 4:
        side5[3*index]     = x;
        side5[3*index + 1] = y;
        side5[3*index + 2] = z;
        break;

    case 5:
        side6[3*index]     = x;
        side6[3*index + 1] = y;
        side6[3*index + 2] = z;
        break;

    case 6:
        side7[3*index]     = x;
        side7[3*index + 1] = y;
        side7[3*index + 2] = z;
        break;

    case 7:
        side8[3*index]     = x;
        side8[3*index + 1] = y;
        side8[3*index + 2] = z;
        break;
    }
}

void Link::createColor(int index, float col_r, float col_g, float col_b){
    colors[3 * index]       =  col_r;
    colors[3 * index + 1]   =  col_g;
    colors[3 * index + 2]   =  col_b;
}

GLfloat* Link::getVertices(){
    return vertices;
}

GLfloat* Link::getVertices(int numberFace){
    switch (numberFace) {
        case 0:
        return side1;

        case 1:
        return side2;

        case 2:
        return side3;

        case 3:
        return side4;

        case 4:
        return side5;

        case 5:
        return side6;

        case 6:
        return side7;

        case 7:
        default:
        return side8;
    }
}

GLfloat* Link::getColors(){
    return colors;
}
