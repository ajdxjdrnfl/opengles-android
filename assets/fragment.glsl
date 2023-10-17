#version 300 es

precision mediump float;

uniform sampler2D textureDiff;
uniform sampler2D textureDissolve;

uniform vec3 matSpec, matAmbi, matEmit;
uniform float matSh;
uniform vec3 srcDiff, srcSpec, srcAmbi;
uniform float threshold;
uniform vec3 lightPos;

in vec3 v_normal, v_view;
in vec2 v_texCoord;

layout(location = 0) out vec4 fragColor;

void main() {
     vec3 color = texture(textureDiff, v_texCoord).rgb;

    vec3 normal = normalize(v_normal);
    vec3 view=normalize(v_view);
    vec3 light=normalize(lightPos);

    vec3 diff = max(dot(normal, light), 0.0)*srcDiff*color;

    vec3 reflect =2.0*normal*dot(normal,light)-light;
    vec3 spec=pow(max(dot(reflect, view), 0.0), matSh) * srcSpec * matSpec;

    vec3 ambient = srcAmbi*matAmbi;

   color = diff+spec+ambient+matEmit;



    float alpha = 1.0f;
    float dissolve = 0.3*texture(textureDissolve,v_texCoord).r+0.6*texture(textureDissolve,v_texCoord).g+0.1*texture(textureDissolve,v_texCoord).b;
    if (dissolve<threshold)
       alpha = 0.0f;

    fragColor = vec4(color, alpha);
}