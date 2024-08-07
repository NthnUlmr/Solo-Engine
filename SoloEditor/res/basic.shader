#shader vertex
#version 330 core

layout(location = 0) in vec3 aPos; // Vertex position

out vec3 vertPos; // Pass position to fragment shader

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;




void main() {
    vec4 worldPos = model * vec4(aPos, 1.0);
    vertPos = worldPos.xyz; // Pass world position to fragment shader
    gl_Position = proj * view * worldPos;
    gl_Position = vec4(aPos, 1.0);
}



#shader fragment
#version 330 core

in vec3 vertPos; // Interpolated vertex position

uniform mat4 view;
uniform mat4 proj;
uniform vec3 cameraPos;
uniform vec2 u_iResolution;

uniform sampler3D sdfTexture;

uniform vec3 mPos;
uniform vec3 rScale;

out vec4 color; // Output color of the fragment

#if HW_PERFORMANCE==0
#define AA 1
#else
#define AA 1   // make this 2 or 3 for antialiasing
#endif

#define ZERO (min(1,0))

//------------------------------------------------------------------
float dot2(in vec2 v) { return dot(v, v); }
float dot2(in vec3 v) { return dot(v, v); }
float ndot(in vec2 a, in vec2 b) { return a.x * b.x - a.y * b.y; }


// p = raypos
// b = boxpos
// size = size
float sdfVoxelCube(vec3 p, vec3 b, float size) {
    vec3 d = abs(p - b) - vec3(size);
    return min(max(d.x, max(d.y, d.z)), 0.0) + length(max(d, 0.0));
}

float sdBox(vec3 p, vec3 b)
{
    vec3 d = abs(p) - b;
    return min(max(d.x, max(d.y, d.z)), 0.0) + length(max(d, 0.0));
}

mat3 setCamera(in vec3 ro, in vec3 ta, float cr)
{
    vec3 cw = normalize(ta - ro);
    vec3 cp = vec3(sin(cr), cos(cr), 0.0);
    vec3 cu = normalize(cross(cw, cp));
    vec3 cv = (cross(cu, cw));
    return mat3(cu, cv, cw);
}

// https://iquilezles.org/articles/checkerfiltering
float checkersGradBox(in vec2 p, in vec2 dpdx, in vec2 dpdy)
{
    // filter kernel
    vec2 w = abs(dpdx) + abs(dpdy) + 0.001;
    // analytical integral (box filter)
    vec2 i = 2.0 * (abs(fract((p - 0.5 * w) * 0.5) - 0.5) - abs(fract((p + 0.5 * w) * 0.5) - 0.5)) / w;
    if (abs(p.x) <  0.1)
    {
        return -1.0;
    }
    else if (abs(p.y) < 0.1)
    {
        return -1.0;
    }
    else if(length(p) > 1.0)
    {
        // xor pattern
        return 0.5 - 0.5 * i.x * i.y;
    }
    else if (length(p) > 0.9) {
        return -1.0;
    }
    else if (length(p) > 0.1)
    {
        return 0.5 - 0.5 * i.x * i.y;
    }
    else
    {
        return -1.0;
    }
}


vec2 opU(vec2 d1, vec2 d2)
{
    return (d1.x < d2.x) ? d1 : d2;
}

const bool voxels[8] = bool[8](false,true,false,false,false,false,true,false);
const float colors[8] = float[8](3.0,2.0,3.0,2.0,3.0,2.0,3.0, 2.0);

// Returns the distance to the nearest object and that object's material id
vec2 map(in vec3 pos, in vec3 chunkOrigin, in vec3 chunkExtent)
{
    vec2 res = vec2(0.21*pos.y, 0.0);

    vec3 splos = (pos - chunkOrigin);
    if (sdfVoxelCube(pos, chunkOrigin, chunkExtent.x) <= 0.0)
    {
        vec3 P = vec3(int(splos.x * 100.0) / 100.0f, int(splos.y * 100.0) / 100.0f, int(splos.z * 100.0) / 100.0f);
        vec4 tmpRes = texture(sdfTexture, P);
        res = opU(res, vec2((tmpRes.r)/10.0, tmpRes.g*10.0));
    }

    return res;
}      
// https://iquilezles.org/articles/nvscene2008/rwwtt.pdf
float calcAO(in vec3 pos, in vec3 nor)
{
    float occ = 0.0;
    float sca = 0.8;
    for (int i = ZERO; i < 5; i++)
    {
        float h = 0.01 + 0.12 * float(i) / 4.0;
        float d = map(pos + h * nor, mPos, rScale).x;
        occ += (h - d) * sca;
        sca *= 0.95;
        if (occ > 0.35) break;
    }
    return clamp(1.0 - 3.0 * occ, 0.0, 1.0) * (0.5 + 0.5 * nor.y);
}


// https://iquilezles.org/articles/rmshadows
float calcSoftshadow(in vec3 ro, in vec3 rd, in float mint, in float tmax)
{
    // bounding volume
    float tp = (0.8 - ro.y) / rd.y; 
    
    if (tp > 0.0)
    {
        tmax = min(tmax, tp);
    }

    float res = 1.0;
    float t = mint;
    for (int i = ZERO; i < 12; i++)
    {
        float h = map(ro + rd * t, mPos, rScale).x * 20.0;
        float s = clamp(8.0 * h / t, 0.0, 1.0);
        res = min(res, s);
        t += clamp(h, 0.01, 0.2);
        if (res<0.004 || t>tmax) break;
    }
    res = clamp(res, 0.0, 1.0);
    return res * res * (3.0 - 2.0 * res);
}

// https://iquilezles.org/articles/boxfunctions
vec2 iBox(in vec3 ro, in vec3 rd, in vec3 rad)
{
    vec3 m = 1.0 / rd;
    vec3 n = m * ro;
    vec3 k = abs(m) * rad;
    vec3 t1 = -n - k;
    vec3 t2 = -n + k;
    return vec2(max(max(t1.x, t1.y), t1.z),
        min(min(t2.x, t2.y), t2.z));
}

vec2 raycast(in vec3 ro, in vec3 rd)
{
    vec2 res = vec2(-1.0, -1.0);

    float tmin = 0.0001;
    float tmax = 2000.0;

    // raytrace floor plane
    float tp1 = (0.0 - ro.y) / rd.y;
    if (tp1 > 0.0)
    {
        tmax = min(tmax, tp1);
        res = vec2(tp1, 1.0);
    }
    //else return res;

    // raymarch primitives   
    //vec2 tb = iBox(ro - vec3(0.0, 10.0, -0.5), rd, vec3(2.5, 10.0, 3.0));
    //if (tb.x < tb.y && tb.y>0.0 && tb.x < tmax)
    {
        //return vec2(tb.x,2.0);
        //tmin = max(tb.x, tmin);
        //tmax = min(tb.y, tmax);

        float t = tmin;
        for (int i = 0; i < 7000 && t < tmax; i++)
        {
            vec2 h = map(ro + rd * t, mPos, rScale);
            if (abs(h.x) < (1e-6 * t))
            {
                res = vec2(t, h.y);
                break;
            }
            t += h.x;
        }
    }

    return res;
}


// https://iquilezles.org/articles/normalsSDF
vec3 calcNormal(in vec3 pos)
{
    //vec2 e = vec2(1.0, -1.0) * 0.1 *0.25;
    //return normalize(e.xyy * map(pos + e.xyy, vec3(0.0, 1.0, 0.0), vec3(1.0, 1.0, 1.0)).x +
    //                 e.yyx * map(pos + e.yyx, vec3(0.0, 1.0, 0.0), vec3(1.0, 1.0, 1.0)).x +
    //                 e.yxy * map(pos + e.yxy, vec3(0.0, 1.0, 0.0), vec3(1.0, 1.0, 1.0)).x +
    //                 e.xxx * map(pos + e.xxx, vec3(0.0, 1.0, 0.0), vec3(1.0, 1.0, 1.0)).x);
    float delta = 0.01;

    vec3 eps = vec3(1.0, 0.0, -1.0) * delta;
    float dx = map(pos + eps.xyy, mPos, rScale).x - map(pos + eps.zyy, mPos, rScale).x;
    dx = dx / (2 * delta);

    float dy = map(pos + eps.yxy, mPos, rScale).x - map(pos + eps.yzy, mPos, rScale).x;
    dy = dy / (2 * delta);

    float dz = map(pos + eps.yyx, mPos, rScale).x - map(pos + eps.yyz, mPos, rScale).x;
    dz = dz / (2 * delta);

    vec3 grad = vec3(dx, dy, dz);
    return normalize(grad);
}





vec3 render(in vec3 ro, in vec3 rd, in vec3 rdx, in vec3 rdy)
{
    // background
    vec3 col = vec3(0.7, 0.7, 0.9) - max(rd.y, 0.0) * 0.3;

    // raycast scene
    vec2 res = raycast(ro, rd);
    float t = res.x;
    float m = res.y;
    if (m > -0.5)
    {
        vec3 pos = ro + t * rd;
        vec3 nor = (m < 1.5) ? vec3(0.0, 1.0, 0.0) : calcNormal(pos);
        vec3 ref = reflect(rd, nor);
        // material        
        col = 0.2 + 0.2 * sin(m * 2.0 + vec3(0.0, 1.0, 2.0));
        float ks = 1.0;

        if (m < 1.5)
        {
            // project pixel footprint into the plane
            vec3 dpdx = ro.y * (rd / rd.y - rdx / rdx.y);
            vec3 dpdy = ro.y * (rd / rd.y - rdy / rdy.y);

            float f = checkersGradBox(3.0 * pos.xz, 3.0 * dpdx.xz, 3.0 * dpdy.xz);
            col = 0.05 + f * vec3(0.05);
            ks = 0.4;
        }

        // lighting
        float occ = calcAO(pos, nor);

        vec3 lin = vec3(0.0);

        // sun
        {
            vec3  lig = normalize(vec3(-0.5, 0.4, -0.6));
            vec3  hal = normalize(lig - rd);
            float dif = clamp(dot(nor, lig), 0.0, 1.0);
            //if( dif>0.0001 )
            dif *= calcSoftshadow(pos, lig, 0.002, 2.5);
            float spe = pow(clamp(dot(nor, hal), 0.0, 1.0), 16.0);
            spe *= dif;
            spe *= 0.04 + 0.96 * pow(clamp(1.0 - dot(hal, lig), 0.0, 1.0), 5.0);
            lin += col * 2.20 * dif * vec3(1.30, 1.00, 0.70);
            lin += 5.00 * spe * vec3(1.30, 1.00, 0.70) * ks;
        }
        // sky
        {
            float dif = sqrt(clamp(0.5 + 0.5 * nor.y, 0.0, 1.0));
            dif *= occ;
            float spe = smoothstep(-0.2, 0.2, ref.y);
            spe *= dif;
            spe *= 0.04 + 0.96 * pow(clamp(1.0 + dot(nor, rd), 0.0, 1.0), 5.0);
            //if( spe>0.001 )
            spe *= calcSoftshadow(pos, ref, 0.002, 2.5);
            lin += col * 0.60 * dif * vec3(0.40, 0.60, 1.15);
            lin += 2.00 * spe * vec3(0.40, 0.60, 1.30) * ks;
        }
        // back
        {
            float dif = clamp(dot(nor, normalize(vec3(0.5, 0.0, 0.6))), 0.0, 1.0) * clamp(1.0 - pos.y, 0.0, 1.0);
            dif *= occ;
            lin += col * 0.55 * dif * vec3(0.25, 0.25, 0.25);
        }
        // sss
        {
            float dif = pow(clamp(1.0 + dot(nor, rd), 0.0, 1.0), 2.0);
            dif *= occ;
            lin += col * 0.25 * dif * vec3(1.00, 1.00, 1.00);
        }

        col = lin;

        col = mix(col, vec3(0.7, 0.7, 0.9), 1.0 - exp(-0.000001 * t * t * t));
    }

    return vec3(clamp(col, 0.0, 1.0));
}


void main() {
   
    float time = 32.0;
    vec2 iResolution = u_iResolution;
    vec2 fragCoord = gl_FragCoord.xy;

    // camera	
    vec3 ta = vec3(0.25, -0.75, -0.75);
    vec3 ro = cameraPos;

    // camera-to-world transformation
    mat3 ca = setCamera(ro, ta, 0.0);
    
    vec3 tot = vec3(0.0);
    vec2 p = (2.0 * fragCoord - iResolution.xy) / iResolution.y;

    // focal length
    const float fl = 2.5;

    mat4 transView = transpose(view);

    // ray direction
    vec3 rd = (transView *  normalize(vec4(p, fl,1.0))).xyz;

    // ray differentials
    vec2 px = (2.0 * (fragCoord + vec2(1.0, 0.0)) - iResolution.xy) / iResolution.y;
    vec2 py = (2.0 * (fragCoord + vec2(0.0, 1.0)) - iResolution.xy) / iResolution.y;
    vec3 rdx = (transView * normalize(vec4(px, fl, 1.0))).xyz;
    vec3 rdy = (transView * normalize(vec4(py, fl, 1.0))).xyz;

    // render	
    vec3 col = render(ro, rd, rdx, rdy);

    // gain
    // col = col*3.0/(2.5+col);
    if (col.x == 0.0)
    {
        color = vec4(0.0, 0.0, 0.0, 0.0);
    }
    else
    {
        // gamma
        col = pow(col, vec3(0.4545));

        tot += col;

        color = vec4(tot, 1.0);
    }
    //color = texture(sdfTexture, vec3(p,.0));
}