#version 330
in vec2 outUV;
uniform int time;
uniform vec2 resolution;
out vec4 fragColor;

const float kMaxHeight = 840.0;
const mat2 m2 = mat2(0.80,  0.60, -0.60,  0.80);
const vec3 sunDir = normalize(vec3(-0.62,0.46,-0.52));

// Smoothstep and it's derivative
vec2 smoothStepD(float a, float b, float x) {
    if (x < a) return vec2(0.0f, 0.0f);
    if (x > b) return vec2(1.0f, 0.0f);
    float ir = 1.0f / (b - a);
    float t = (x - a) * ir;
    return vec2(t * t * (3.0 - 2.0 * t), 6.0 * t * (1.0 - t) * ir);
}

float hash1( vec2 p )
{
    p  = 50.0*fract( p*0.3183099 );
    return fract( p.x*p.y*(p.x+p.y) );
}

float noise( in vec2 x )
{
    vec2 p = floor(x);
    vec2 w = fract(x);
    #if 1
    vec2 u = w*w*w*(w*(w*6.0-15.0)+10.0);
    #else
    vec2 u = w*w*(3.0-2.0*w);
    #endif

    float a = hash1(p+vec2(0,0));
    float b = hash1(p+vec2(1,0));
    float c = hash1(p+vec2(0,1));
    float d = hash1(p+vec2(1,1));

    return -1.0+2.0*(a + (b-a)*u.x + (c-a)*u.y + (a - b - c + d)*u.x*u.y);
}

float fbm_9( in vec2 x )
{
    float f = 1.9;
    float s = 0.55;
    float a = 0.0;
    float b = 0.5;
    for( int i=0; i<9; i++ )
    {
        float n = noise(x);
        a += b*n;
        b *= s;
        x = f*m2*x;
    }

    return a;
}

vec2 terrainMap( in vec2 p )
{
    float e = fbm_9( p/2000.0 + vec2(1.0,-2.0) );
    float a = 1.0-smoothstep( 0.12, 0.13, abs(e+0.12) ); // flag high-slope areas (-0.25, 0.0)
    e = 600.0*e + 600.0;

    // cliff
    //e += 90.0*smoothstep( 552.0, 594.0, e );
    //e += 90.0*smoothstep( 550.0, 600.0, e );

    return vec2(e,a);
}

vec3 getNormal( const vec3 p )
{
    float eps = 0.01;
    float hL = terrainMap(vec2(p.x - eps, p.z)).x;
    float hR = terrainMap(vec2(p.x + eps, p.z)).x;
    float hD = terrainMap(vec2(p.x, p.z - eps)).x;
    float hU = terrainMap(vec2(p.x, p.z + eps)).x;
    return normalize(vec3(hL - hR, 2.0f*eps, hD - hU));
}

float raymarchTerrain( in vec3 ro, in vec3 rd, float tmin, float tmax )
{

    // bounding plane
    float tp = (kMaxHeight+-ro.y)/rd.y;
    if( tp>0.0 ) tmax = min( tmax, tp );

    // raymarch
    float dis, th;
    float t = tmin;
    float ot = t;
    float odis = 0.0;
    for( int i=0; i<400; i++ )
    {
        th = 0.001*t;

        vec3  pos = ro + t*rd;
        vec2  env = terrainMap( pos.xz );
        float hei = env.x;

        // terrain
        dis = pos.y - hei;
        if( dis<th ) break;

        ot = t;
        odis = dis;
        t += dis*0.8*(1.0-0.75*env.y); // slow down in step areas
        if( t>tmax ) break;
    }

    if( t>tmax ) t = -1.0;
    else t = ot + (th-odis)*(t-ot)/(dis-odis); // linear interpolation for better accuracy

    return t;
}

vec3 skyColor(in vec3 ro, in vec3 rd ) {
    vec3 color;
    color = vec3(0.5,0.7,1.0) - rd.y*0.45; // sky

    // Clouds
    float t = (2000.0 - ro.y) / rd.y;
    if (t > 0.0) {
        vec2 uv = (ro+t*rd).xz;
        float cloudNoise = fbm_9(uv*0.0008 + vec2(0.0, time*0.001));
        float dl = (smoothStepD(-0.2, 0.6, cloudNoise)).x;
        color = mix(color, vec3(1.0), 0.6*dl);
    }

    // Sun glow
    float sun = clamp(dot(sunDir, rd), 0.0, 1.0);
    color += 0.3*vec3(1.0, 0.6, 0.3) * pow(sun, 32.0);

    return color;
}

void main() {

    //vec2 uv = (2.0*outUV - resolution.xy) / resolution.y;
    vec2 uv = outUV;
    uv.x *= resolution.x / resolution.y;

    //vec3 ro = vec3(0.1*time, 501.5, 6.0);
    vec3 ro = vec3(80.0, 401.5, 6.0);
    vec3 rd = normalize(vec3(uv, -1.0));

    float dist = raymarchTerrain(ro, rd, 0.0, 1000.0);

    vec3 color;

    if(dist > 0.0) {
        vec3 hitPos = ro + rd * dist;

        // Map height to color
        float h = hitPos.y / kMaxHeight;      // 0 = lowest, 1 = highest
        color = mix(vec3(0.1,0.5,0.1), vec3(1.0,1.0,0.5), h); // green → sand/yellow
        float diffuse = clamp(dot(getNormal(ro + rd * dist), sunDir), 0.0, 1.0);
        color = color * (0.3 + 0.7 * diffuse);
    }
    else {
        color = skyColor(ro, rd);
    }

    fragColor = vec4(color, 1.0);
}