//varying float Diffuse;
varying vec3 Specular;
varying vec3 LightDir;

attribute vec3 tangent;
attribute vec3 binormal;

void main(void) {
   vec3 tnorm = normalize(gl_NormalMatrix * gl_Normal);
   vec3 ecPosition = vec3(gl_ModelViewMatrix * gl_Vertex);
   vec3 lightVec = normalize(gl_LightSource[0].position.xyz - ecPosition);

   Specular = vec3(0,0,0);
   float Diffuse = max(dot(lightVec, tnorm), 0.0);

   if(Diffuse > 0.0) {
      vec3 reflectVec = reflect(-lightVec, tnorm);
      vec3 viewVec = normalize(-ecPosition);
   
      float spec = clamp(dot(reflectVec, viewVec), 0.0, 1.0);
      spec = pow(spec, 8.0);
      Specular = spec * vec3(1.0, 0.941, 0.898) * 0.3;
   }
   
   gl_TexCoord[0] = gl_MultiTexCoord0;
   gl_Position = ftransform();
   
   
   vec3 n = normalize(gl_NormalMatrix * gl_Normal);
   vec3 t = normalize(gl_NormalMatrix * tangent);
   vec3 b = normalize(gl_NormalMatrix * binormal);
   
   vec3 v;
   v.x = dot(lightVec, t);
   v.y = dot(lightVec, b);
   v.z = dot(lightVec, n);
   LightDir = normalize(v);
}
