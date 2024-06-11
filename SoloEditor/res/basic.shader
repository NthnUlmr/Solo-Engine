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
}



#shader fragment
#version 330 core

in vec3 vertPos; // Interpolated vertex position

uniform mat4 view;
uniform mat4 proj;

out vec4 color; // Output color of the fragment

float sdfVoxelCube(vec3 p, vec3 b, float size) {
    vec3 d = abs(p - b) - vec3(size);
    return min(max(d.x, max(d.y, d.z)), 0.0) + length(max(d, 0.0));
}

void main() {
    vec3 box_pos = vec3(0.0f, 0.0f, 10.0f); // Center of the voxel cube
    float box_size = 0.5f;                  // Size of the voxel cube

    // Calculate the ray direction in world space
    vec4 tmp_ray_posa = view * vec4(vertPos, 1.0);
    vec4 tmp_ray_pos = inverse(proj) * vec4(tmp_ray_posa.xy, -1.0, 1.0);
    vec3 ray_pos = tmp_ray_pos.xyz;

    // Extract the eye direction from the view matrix
    vec3 eye_dir = normalize(-view[2].xyz); // Third column of the view matrix, negated

    // Step through the ray to find intersection with the voxel
    for (int i = 0; i < 255; i++) {
        float step_size = sdfVoxelCube(ray_pos, box_pos, box_size);
        if (step_size <= 0.0) {
            vec3 difference = normalize(ray_pos - box_pos);
            difference = abs(difference);
            color = vec4(0.0, 0.0, 0.0, 1.0);// vec4(difference, 1.0f);
            return;
        }
        else {
            ray_pos += step_size * eye_dir;
        }
    }

    // Sky color calculations
    vec3 ray_dir = normalize(ray_pos);
    vec3 skyColor = mix(vec3(0.4f, 0.5f, 0.9f), vec3(0.5f), exp(-10.0f * ray_pos.y));
    vec3 sun = normalize(vec3(0.8f, -0.4f, 0.2f));
    float sunDisc = smoothstep(0.995f, 1.0f, dot(ray_dir, sun)) * 0.25f;
    skyColor += sunDisc * vec3(7.0f, 4.5f, 3.0f);
    color = vec4(skyColor, 1.0f);
}