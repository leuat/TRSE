attribute vec2 vPosition;
varying vec2 v_pos;

void main()
{
    v_pos = vPosition.xy;
    gl_Position = vec4(vPosition.xy, 0.0, 1.0);
}