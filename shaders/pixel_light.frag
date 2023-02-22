varying vec3 normal, lightDir;
uniform sampler2D ColorMap;

void main()
{
		vec3 n = normalize(normal);
      float NdotL = max(dot(n, lightDir),0.0);
      gl_FragColor = texture2D(ColorMap, gl_TexCoord[0].st) * NdotL;
}
