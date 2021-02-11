#include "link.h"


Link::Link(float ep_link, float r_link, float edge, float col_r, float col_g, float col_b){

    float l_link = r_link - edge/2;

        // Point A
        createVertice(0, -r_link/2, l_link/2, ep_link/2);
        createColor(0, col_r, col_g, col_b);

        // B
        createVertice(1, -l_link/2, r_link/2, ep_link/2);
        createColor(1, col_r, col_g, col_b);

        // C
        createVertice(2, l_link/2, r_link/2, ep_link/2);
        createColor(2, col_r, col_g, col_b);

        // D
        createVertice(3, r_link/2, l_link/2, ep_link/2);
        createColor(3, col_r, col_g, col_b);

        // E
        createVertice(4, r_link/2, -l_link/2, ep_link/2);
        createColor(4, col_r, col_g, col_b);

        // F
        createVertice(5, l_link/2, -r_link/2, ep_link/2);
        createColor(5, col_r, col_g, col_b);

        // G
        createVertice(6, -l_link/2, -r_link/2, ep_link/2);
        createColor(6, col_r, col_g, col_b);


        // H
        createVertice(7, -r_link/2, -l_link/2, ep_link/2);
        createColor(7, col_r, col_g, col_b);


/*
        // Point A'
        createVertice(8, -r_link/2, l_link/2, -(ep_link/2));
        createColor(8, col_r, col_g, col_b);

        // B'
        createVertice(9, -l_link/2, r_link/2, -(ep_link/2));
        createColor(9, col_r, col_g, col_b);

        // C'
        createVertice(10, l_link/2, r_link/2, -(ep_link/2));
        createColor(10, col_r, col_g, col_b);

        // D'
        createVertice(11, r_link/2, l_link/2, -(ep_link/2));
        createColor(11, col_r, col_g, col_b);

        // E'
        createVertice(12, r_link/2, -l_link/2, -(ep_link/2));
        createColor(12, col_r, col_g, col_b);

        // F'
        createVertice(13, l_link/2, -r_link/2, -(ep_link/2));
        createColor(13, col_r, col_g, col_b);

        // G'
        createVertice(14, -l_link/2, -r_link/2, -(ep_link/2));
        createColor(14, col_r, col_g, col_b);


        // H'
        createVertice(15, -r_link/2, -l_link/2, -(ep_link/2));
        createColor(15, col_r, col_g, col_b);
*/



}



void Link::createVertice(int index, float x, float y, float z){
    vertices[3 * index]     = x;
    vertices[3 * index + 1] = y;
    vertices[3 * index + 2] = z;
}

void Link::createColor(int index, float col_r, float col_g, float col_b){
    colors[3 * index]       =  col_r;
    colors[3 * index + 1]   =  col_g;
    colors[3 * index + 2]   =  col_b;
}

GLfloat* Link::getVertices(){
    return vertices;
}

GLfloat* Link::getColors(){
    return colors;
}
