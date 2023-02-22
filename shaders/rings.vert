uniform vec4 planetCenter;

varying vec2 TexCoord;
varying vec3 vnorm;
varying vec3 planetVec;

void main() {
   TexCoord = gl_MultiTexCoord0.st;
   vnorm = normalize(gl_NormalMatrix * gl_Normal);
   gl_Position = ftransform();
   
   vec4 ecPosition = gl_ModelViewMatrix * planetCenter;
   planetVec = vec3(gl_LightSource[0].position - ecPosition);
}
