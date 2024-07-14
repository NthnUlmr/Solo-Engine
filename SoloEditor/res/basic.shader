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



float sdfVoxelCube(vec3 p, vec3 b, float size) {
    vec3 d = abs(p - b) - vec3(size);
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
    // xor pattern
    return 0.5 - 0.5 * i.x * i.y;
}


vec2 opU(vec2 d1, vec2 d2)
{
    return (d1.x < d2.x) ? d1 : d2;
}

vec2 map(in vec3 pos)
{
    vec2 res = vec2(pos.y, 0.0);

    // bounding box
    for (float ii = 0.0f; ii < 50.0f; ii++)
    {
        for (float jj = 0.0f; jj < 6.0f; jj++)
        {
            res = opU(res, vec2(sdfVoxelCube(pos - vec3(ii / 10.0, 0.4, jj / 10.0), vec3(0.3, 0.25, 0.1), 0.1f / 3.0), ii / jj));
        }

    }

    return res;
}

// https://iquilezles.org/articles/nvscene2008/rwwtt.pdf
float calcAO(in vec3 pos, in vec3 nor)
{
    float occ = 0.0;
    float sca = 1.0;
    for (int i = ZERO; i < 5; i++)
    {
        float h = 0.01 + 0.12 * float(i) / 4.0;
        float d = map(pos + h * nor).x;
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
    float tp = (0.8 - ro.y) / rd.y; if (tp > 0.0) tmax = min(tmax, tp);

    float res = 1.0;
    float t = mint;
    for (int i = ZERO; i < 24; i++)
    {
        float h = map(ro + rd * t).x;
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

    float tmin = 1.0;
    float tmax = 20.0;

    // raytrace floor plane
    float tp1 = (0.0 - ro.y) / rd.y;
    if (tp1 > 0.0)
    {
        tmax = min(tmax, tp1);
        res = vec2(tp1, 1.0);
    }
    //else return res;

    // raymarch primitives   
    vec2 tb = iBox(ro - vec3(0.0, 0.4, -0.5), rd, vec3(2.5, 0.41, 3.0));
    if (tb.x < tb.y && tb.y>0.0 && tb.x < tmax)
    {
        //return vec2(tb.x,2.0);
        tmin = max(tb.x, tmin);
        tmax = min(tb.y, tmax);

        float t = tmin;
        for (int i = 0; i < 70 && t < tmax; i++)
        {
            vec2 h = map(ro + rd * t);
            if (abs(h.x) < (0.0001 * t))
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
#if 0
    vec2 e = vec2(1.0, -1.0) * 0.5773 * 0.0005;
    return normalize(e.xyy * map(pos + e.xyy).x +
        e.yyx * map(pos + e.yyx).x +
        e.yxy * map(pos + e.yxy).x +
        e.xxx * map(pos + e.xxx).x);
#else
    // inspired by tdhooper and klems - a way to prevent the compiler from inlining map() 4 times
    vec3 n = vec3(0.0);
    for (int i = ZERO; i < 4; i++)
    {
        vec3 e = 0.5773 * (2.0 * vec3((((i + 3) >> 1) & 1), ((i >> 1) & 1), (i & 1)) - 1.0);
        n += e * map(pos + 0.0005 * e).x;
        //if( n.x+n.y+n.z>100.0 ) break;
    }
    return normalize(n);
#endif    
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
            dif *= calcSoftshadow(pos, lig, 0.02, 2.5);
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
            spe *= calcSoftshadow(pos, ref, 0.02, 2.5);
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

        col = mix(col, vec3(0.7, 0.7, 0.9), 1.0 - exp(-0.0001 * t * t * t));
    }

    return vec3(clamp(col, 0.0, 1.0));
}


void main() {
    //vec3 box_pos = vec3(0.0f, 0.0f, 10.0f); // Center of the voxel cube
    //float box_size = 0.5f;                  // Size of the voxel cube

    //vec2 uv = (gl_FragCoord.xy / vec2(1280.0f, 720.0f)) * 2.0 - 1.0;
    //uv.x *= 1280.0f / 720.0f; 

    //vec3 ro = cameraPos; // Ray Origin
    //vec3 rd = normalize(vec3(uv, -1.0)); // Ray Direction

    //rd = (view * proj * vec4(rd, 0.0)).xyz;


    //color = vec4(0.0f,0.0f,0.0f,1.0f);




    //vec2 mo = iMouse.xy / iResolution.xy;
    float time = 32.0;// +iTime * 1.5;
    vec2 iResolution = vec2(1280.0f, 720.0f);
    vec2 fragCoord = gl_FragCoord.xy;

    // camera	
    vec3 ta = vec3(0.25, -0.75, -0.75);
    vec3 ro = cameraPos;// ta + vec3(4.5 * cos(0.1 * time), 2.2, 4.5 * sin(0.1 * time));
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

    // gamma
    col = pow(col, vec3(0.4545));

    tot += col;

    color = vec4(tot, 1.0);
    //color = vec4(1.0, 1.0, 1.0, 1.0);

}