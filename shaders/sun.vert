varying vec3 viewVec;
varying vec3 tnorm;

void main() {
   tnorm = normalize(gl_NormalMatrix * gl_Normal);
   vec3 ecPosition = vec3(gl_ModelViewMatrix * gl_Vertex);
   viewVec = normalize(-ecPosition);

   gl_Position = ftransform();
}
