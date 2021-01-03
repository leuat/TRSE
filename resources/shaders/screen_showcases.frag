varying vec2 v_pos;
uniform sampler2D t1;
uniform sampler2D t2;
uniform float time;
uniform float time2;
uniform int type1;
uniform int type2;

float sx = 4.0;


void FadeRegular() {
    float scale = smoothstep(0.0, 1.0, time2);
    vec2 uv = vec2(v_pos.x, -v_pos.y);

    vec4 g = texture2D( t1, uv*0.5 + vec2(0.5,0.5) );
    vec4 c = texture2D( t2, uv*0.5 + vec2(0.5,0.5) );
    scale = pow(scale,4.0);

    gl_FragColor=c*scale + g*(1.0-scale);

}

void RollDown() {
    float scale = smoothstep(0.0, 1.0, time2);
    scale = pow(scale,8.0);

    float sa = 1.2;
    float sb = 1.2;

    vec2 uv = vec2(v_pos.x, -v_pos.y);

    vec2 uva = uv*0.5*sa + vec2(0.5,0.5-(scale)*sa);
    vec2 uvb = uv*0.5*sb + vec2(0.5,0.5-(scale)*sb+sb);



    vec4 g = texture2D( t1, uva );
    vec4 c = texture2D( t2, uvb );
    if (uva.y<0.0 || uva.y>1.0) g=vec4(0,0,0,0);
    if (uvb.y<0.0 || uvb.y>1.0) c=vec4(0,0,0,0);
    if (uva.x<0.0 || uva.x>1.0) g=vec4(0,0,0,0);
    if (uvb.x<0.0 || uvb.x>1.0) c=vec4(0,0,0,0);
    gl_FragColor=c+g;

}


void main()
{

    RollDown();
//    gl_FragColor.a = 1.0;
//    gl_FragColor=Tunnel(pixelScale);

 }
