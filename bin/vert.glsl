varying vec3 color;
varying vec2 texCoord;

void main(void)
{
    color = gl_Color.rgb;
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
    texCoord=gl_MultiTexCoord0.xy;
}
