varying vec3 viewVec;
varying vec3 tnorm;

uniform vec3 color;

void main() {
   float alpha = abs(dot(viewVec, tnorm));
   alpha = max(0.0, alpha / 2.0);
   gl_FragColor = vec4(color, alpha);
}
