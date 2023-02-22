uniform sampler2D Rings;
uniform float inner;
uniform float thetaInner;
uniform float thetaOuter;
uniform float radius;
uniform bool shadow;

varying vec2 TexCoord;
varying vec3 vnorm;
varying vec3 planetVec;

vec4 getTextureColor(float ts) {
   return texture2D(Rings, vec2(ts, 0.5));
}

void main() {
   float s = TexCoord.s - 0.5;
   float t = TexCoord.t - 0.5;
   float distance = sqrt(s * s + t * t);
      
   if(distance > 0.5 || distance < inner) {
      gl_FragColor = vec4(0.0, 0.0, 0.0, 0.0);
   }
   else { // inner <= distance <= 0.5
      vec3 n = normalize(vnorm);
      vec3 pV = normalize(planetVec);
      float dotN = dot(n, pV);
      
      // Map distance from [inner, 0.5] to [0, 1] -> ts
      float ts = (distance - inner) / (0.5 - inner);
      vec4 tex = getTextureColor(ts);

      float p = 1.0;
      if(shadow == true) {
         if(dotN < thetaOuter) {
            p = 0.0;
         }
         else if(dotN < thetaInner) {
            float angle = sin(acos(dotN));
            float l = radius / angle;

            if(distance < l) {
               p = 0.0;
            }
         }
      }

      gl_FragColor = vec4(tex.r * p, tex.g * p, tex.b * p, tex.a);
   }
}
