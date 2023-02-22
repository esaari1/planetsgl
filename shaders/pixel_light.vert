varying vec3 normal,lightDir;

void main() 
{
   /* first transform the normal into eye space and normalize the result */
   normal = normalize(gl_NormalMatrix * gl_Normal);
   
   /* now normalize the light's direction. */
   /* Light is stored in eye space so no need to transform. */
   /* Since this is a diretional light, the position is the direction. */
   lightDir = normalize(vec3(gl_LightSource[0].position));
   
   gl_TexCoord[0] = gl_MultiTexCoord0;

   gl_Position = ftransform();
}
