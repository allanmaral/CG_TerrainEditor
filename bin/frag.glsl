uniform sampler2D img;
varying vec3 color;
varying vec2 texCoord;

void main(void)
{
    gl_FragColor =  texture2D(img, texCoord);
}
