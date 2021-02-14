attribute highp vec4 posAttr;
attribute lowp  vec4 colAttr;
attribute highp vec3 norAttr;
varying lowp  vec4 col;
varying highp vec3 nor;
uniform highp mat4 projMatrix;  // projection matrix
uniform highp mat4 mvMatrix;    // model-view matrix
uniform highp mat3 norMatrix;   // normal matrix 3x3

void main() {
   col = colAttr;
   gl_Position = projMatrix * mvMatrix * posAttr;
   nor = norMatrix * norAttr;
}

