varying vec3 viewVec;
varying vec3 tnorm;
varying vec3 lightVec;

void main(void) {
   tnorm = normalize(gl_NormalMatrix * gl_Normal);
   vec3 ecPosition = vec3(gl_ModelViewMatrix * gl_Vertex);
   viewVec = normalize(-ecPosition);
   lightVec = normalize(gl_LightSource[0].position.xyz - ecPosition);

   gl_Position = ftransform();
}
