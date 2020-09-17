varying vec2 v_pos;
uniform sampler2D t1;
uniform sampler2D t2;
uniform float time;
uniform float time2;
uniform int type1;
uniform int type2;

float sx = 4.0;


vec4 Test1()
{
    float scale = 1.0;
    float t = time*2.5;
    float h= 1.0+0.5*(cos((v_pos.x*3.5-t)*3.0));
    float v= 1.0+0.2*(cos((v_pos.x*4.5-t)*5.0));

    float scalex = 4.0+time*2.0;


    vec2 PUV = vec2(v_pos.x*scalex*v, 0.0-h*(v_pos.y));
    if (PUV.y>1.0 || PUV.y<-1.0) scale=0.0;
    vec4 c = texture2D( t1, PUV*0.5 + vec2(0.5,0.5) );
    vec4 g = texture2D( t2, PUV*0.5 + vec2(0.5,0.5) );
/*    c.r = 0;
    c.g = 0;
    gl_FragColor=vec4(c,1);*/

    //float mixVal = 0.5+0.5*cos(t);

    float mixVal = 0.0;

    return (mixVal*c+(1.0-mixVal)*g)*scale;

}

vec4 Flip()
{
    float pixelScale = 8.0;
    float ph = 1.0/pixelScale;

    vec2 px = vec2(pixelScale*sx,pixelScale);
    vec2 pix = vec2(int(v_pos.x*px.x), int((v_pos.y*px.y)));
    pix.x = pix.x/px.x;
    pix.y = pix.y/px.y;


    float scale = 1.0;


    float t = -5.14159 + (time)*6.0 +(pix.y*0.0+pix.x*1.0)*0.3;
    t = clamp(t,-3.14159*2.0,3.14159*2.0);

    t= 3.14159*2.0;

    float h = cos(t+3.14159);
    vec2 PUV = vec2(pix.x*sx, 0.0-(pix.y));
    vec2 delta = vec2(v_pos.x*sx, 0.0-(v_pos.y))+vec2(0.25,0.25)/2.0;
    delta = ph*floor(delta/pixelScale);//    mod(delta,vec2(ph,ph));//-ph/2.0;
//    if (v_pos.y>=0.5) delta.y+=ph;

//   delta.y = (delta.y-pix.y)*(1.0+abs(tan(t))/1.6) + pix.y;
//    delta.y *= (1.0+abs(tan(t))/(3.14159265/2.0));

    PUV = PUV + delta;


    vec4 c = texture2D( t1, PUV*0.5 + vec2(0.5,0.5) );
    vec4 g = texture2D( t2, PUV*0.5 + vec2(0.5,0.5) );

//    if (delta.y>ph || delta.y<-ph) scale=0.0;
/*    c.r = 0;
    c.g = 0;
    gl_FragColor=vec4(c,1);*/

    //float mixVal = 0.5+0.5*cos(t);

    float mixVal = float(h>0.0);

    return (mixVal*c+(1.0-mixVal)*g)*scale;

}



vec2 rotate2D(vec2 rp, float r) {
    rp = rp - vec2(0.5,0.5);
    vec2 or = rp;
    rp.x = or.x*cos(r)  -or.y*sin(r);
    rp.y = or.x*sin(r)  +or.y*cos(r);



    rp = rp + vec2(0.5,0.5);
    return rp;
}


vec2 Pixel()
{
    float t = time*2.0*3.14159;//clamp(time,0.0,3.14159*2.0);


    float amp = cos(t)*0.7;
    float s1 = 1.1;
    float s2 = 1.06;

    float r = time*2.0*3.14159;

    vec2 rp = v_pos;
    rp.x*=sx;

    rp.x = rp.x*(1.0 + rp.y*0.3*cos(r/2.0));
    rp = rp - vec2(0.5,0.5);
    vec2 or = rp;
    rp.x = or.x*cos(r)  -or.y*sin(r);
    rp.y = or.x*sin(r)  +or.y*cos(r);



    rp = rp + vec2(0.5,0.5);

    vec2 twirl = vec2(cos(-rp.y*0.3+rp.x*s1-time*0.3)*amp, sin(rp.y*s2+time*0.252)*amp);
    vec2 pix = vec2(rp.x+cos(time*0.2)*7.23, rp.y+sin(time*0.152)*4.23) - twirl;


    float scale = 1.0;

    vec2 PUV = vec2(pix.x, 0.0-(pix.y));

    return PUV;
}


vec2 Tunnel()
{
    float t = time*3.14159265*2.0;//clamp(time,0.0,3.14159*2.0);




    vec2 pix = vec2(1.5/(0.2+sqrt(length(vec2(v_pos.x*sx, v_pos.y)))),1.0/3.14159*atan(v_pos.y,(v_pos.x*sx)));
    vec2 PUV = vec2(pix.x+t/(3.14159265*2.0)*6.0, pix.y+cos(t));

//    PUV = rotate2D(PUV, -time);


    return PUV;


}


vec2 Cylinder() {
    float t = float(int((10.5+(v_pos.x)/2.0*(sx+0.0))));
    float y = sin(v_pos.y*0.9+t*5.0+time*5.0 + 4.5*cos(v_pos.x));

//    y = clamp(2.0/(y+1.4),-10.5,10.5);
    vec2 p = vec2(v_pos.x*sx,y+(time)*10.0+t*7.33);


    return p;
}


vec2 Grid() {

//    y = clamp(2.0/(y+1.4),-10.5,10.5);

    vec2 t;

    t.x = v_pos.x*sx+cos(time*0.5)*2.0;
    t.y = 1.0-v_pos.y+cos(time*0.7)*2.0;

    t = rotate2D(t,0.0-time*2.0+0.4*length(t)*cos(time)+0.4*sin(time)*(t.y - t.x));

    float x1 = 1.0+t.x*0.3*(0.7+0.5*sin(time*13.0));
    float y1 = 1.0+t.y*0.3*(0.7+0.5*cos(time*10.0));

    vec2 p = vec2(t.x*x1,t.y*y1);


    return p;
}


vec2 News() {

//    y = clamp(2.0/(y+1.4),-10.5,10.5);

    vec2 t;

    t.x = v_pos.x*sx+cos(time*0.5)*2.0;
    t.y = 1.0-v_pos.y+cos(time*0.7)*2.0;

    t = rotate2D(t,0.0-time*2.0+0.4*length(t)*cos(time)+0.4*sin(time)*(t.y - t.x));

    float x1 = 1.0+t.x*0.3*(0.7+0.5*sin(time*13.0));
    float y1 = 1.0+t.y*0.3*(0.7+0.5*cos(time*10.0));

    vec2 p = vec2(t.x*x1,t.y*y1);


    return p;
}


vec2 getType(float t)
{
    t = mod(t,3.0);

//    return News();
//    return Grid();

    if (t==0.0) return Grid();
    if (t==1.0) return Tunnel();
//    if (t==2.0) return Cylinder();
    if (t==2.0) return Pixel();
}



void main()
{

    float scale = smoothstep(0.0, 1.0, time2);

    vec4 g = texture2D( t1, getType(float(type1))*0.5 + vec2(0.5,0.5) );
    vec4 c = texture2D( t2, getType(float(type2))*0.5 + vec2(0.5,0.5) );
    scale = pow(scale,4.0);
    gl_FragColor=c*scale + g*(1.0-scale);
//    gl_FragColor=Tunnel(pixelScale);

 }
