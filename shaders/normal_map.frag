uniform sampler2D ColorMap;
uniform sampler2D NormalMap;

varying vec3 LightDir;
   
void main() {
   vec3 color = texture2D(ColorMap, gl_TexCoord[0].st).rgb;
   
   vec3 p = texture2D(NormalMap, gl_TexCoord[0].st).xyz;
   p = p * 2.0 - 1.0;
   p = normalize(p);
   
   vec3 normDelta = p;
   vec3 litColor = color * max(dot(normDelta, normalize(LightDir)), 0.0);
   
   gl_FragColor = vec4(litColor, 1.0);
}