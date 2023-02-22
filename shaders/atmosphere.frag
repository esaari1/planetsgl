varying vec3 viewVec;
varying vec3 tnorm;
varying vec3 lightVec;

uniform vec3 color;

void main(void) {
   vec3 n = tnorm;
   float dP = 1.0 - abs(dot(viewVec, n));
   float alpha = 1.0 - pow(dP, 5.0);
   alpha *= dP;
   
   alpha = clamp(pow(alpha, 4.0) * dot(n, lightVec) * 7.0, 0.0, 1.0);

   gl_FragColor = vec4(color, alpha);
}
