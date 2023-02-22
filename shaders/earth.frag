uniform sampler2D EarthDay;
uniform sampler2D EarthNight;
uniform sampler2D EarthCloud;
uniform sampler2D NormalMap;

//varying float Diffuse;
varying vec3 Specular;

varying vec3 LightDir;

void main(void) {
   vec3 norm = texture2D(NormalMap, gl_TexCoord[0].st).xyz;
   norm = norm * 2.0 - 1.0;
   norm = normalize(norm);

   float Diffuse = max(dot(normalize(LightDir), norm), 0.0);

   vec4 clouds = texture2D(EarthCloud, gl_TexCoord[0].st);
   vec3 daytime = (texture2D(EarthDay, gl_TexCoord[0].st).rgb * Diffuse + Specular * clouds.a) * (1.0 - clouds.r) + clouds.r * Diffuse;
   vec3 nighttime = texture2D(EarthNight, gl_TexCoord[0].st).stp * (1.0 - clouds.r);
   vec3 color = daytime;
   
//   if(Diffuse < -0.1) {
//      color = nighttime;
//   }
//   else if(abs(Diffuse) < 0.1) {
//      color = mix(nighttime, daytime, (Diffuse + 0.1) * 5.0);
//   }

   gl_FragColor = vec4(color, 1.0);
}
