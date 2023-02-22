varying vec3 LightDir;

attribute vec3 tangent;
attribute vec3 binormal;

void main() {
   gl_Position = ftransform();
   gl_TexCoord[0] = gl_MultiTexCoord0;
   
   // Vertex coordinates from the eye position
   vec3 ecPosition = vec3(gl_ModelViewMatrix * gl_Vertex);
   vec3 LightPosition = normalize(gl_LightSource[0].position.xyz - ecPosition);

   vec3 n = normalize(gl_NormalMatrix * gl_Normal);
   vec3 t = normalize(gl_NormalMatrix * tangent);
   vec3 b = normalize(gl_NormalMatrix * binormal); // cross(n, t);
   
   vec3 v;
   v.x = dot(LightPosition, t);
   v.y = dot(LightPosition, b);
   v.z = dot(LightPosition, n);
   LightDir = normalize(v);
}
