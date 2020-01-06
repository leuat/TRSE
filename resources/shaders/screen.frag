varying vec2 v_pos;
uniform sampler2D screen;
uniform sampler2D grid;



void main()
{

    vec2 PUV = vec2(v_pos.x, 0.0-(v_pos.y));
    vec4 c = texture2D( screen, PUV*0.5 + vec2(0.5,0.5) );
    vec4 g = texture2D( grid, PUV*0.5 + vec2(0.5,0.5) );
/*    c.r = 0;
    c.g = 0;
    gl_FragColor=vec4(c,1);*/
    gl_FragColor=c+g;
 }
