#shader vertex
#version 430

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;

uniform mat4 u_View;
uniform mat4 u_Proj;
uniform mat4 u_Model;

out vec4 vertPos;
out vec4 eye_dir;

out mat4 view;
out mat4 proj;

void main()
{
	gl_Position = vec4(position, 1.0);// u_Proj* u_View* u_Model* vec4(position, 1.0);
	vertPos = vec4(position, 1.0);// color;

	eye_dir = u_Proj * u_View * vec4(0.0, 0.0, -1.0, 0.0);
	view = u_View;
	proj = u_Proj;
}




#shader fragment
#version 430

out vec4 color;

in vec4 vertPos;
in vec4 eye_dir;

in mat4 view;
in mat4 proj;

uniform vec4 u_Color;
uniform sampler2D u_Tex;
uniform vec3 u_iResolution;

// From Inigo Quilez - possibly the most powerful person in existence
// Returns the value of the SDF of a box 
// p is the location of a point (x,y,z) relative to the center of the box
// b represents the extents of the box from its center (length/2, width/2, height/2)
float sdfBox(vec3 p, vec3 b)
{
	vec3 q = abs(p) - b;
	return length(max(q, 0.0)) + min(max(q.x, max(q.y, q.z)), 0.0);
}

// Calculates the sdf of an individual voxel - for arbitrary voxels
// Inputs: p_world : the location of a a point of interest in world space
// b_world : the location of the center of the voxel in world space
// b_extent : the size of a voxel in each dimension 
float sdfVoxel(vec3 p_world, vec3 b_world, vec3 b_extent)
{
	vec3 q = abs(p_world - b_world) - b_extent;
	return length(max(q, 0.0)) + min(max(q.x, max(q.y, q.z)), 0.0);
}

// Calculates the sdf of an individual voxel - for cubes only
// Inputs: p_world : the location of a a point of interest in world space
// b_world : the location of the center of the voxel in world space
// b_extent : the size of a voxel - for cubic voxels
float sdfVoxelCube(vec3 p_world, vec3 b_world, float b_extent)
{
	vec3 q = abs(p_world - b_world) - b_extent;
	return length(max(q, 0.0f)) + min(max(q.x, max(q.y, q.z)), 0.0f);
}


void main()
{
	// Most basic raytracer imaginable
	// 1. There is only one object in the scene - a single voxel at 0,0,0
	// 2. The user can move the camera (the view/projection matrix)
	// 3. There is no lighting for now
	// 4. Just walk down sdf and see if hit

	//vec3 eye_dir = vec3(1.0, 0.0, 0.0); // Look direction in projection-view space

	vec3 box_pos = vec3(0.0f, 0.0f, 10.0f); // Hopefully the center of the screen in front of the viewer! (probably will end up behind :( )
	float box_size = 0.5f;
	vec4 tmp_ray_posa = view * vertPos;
	vec4 tmp_ray_pos = inverse(proj) * vec4(tmp_ray_posa.xy, -1.0,1.0);
	vec3 ray_pos = tmp_ray_pos.xyz;// (2.0f * (vertPos.xyz - u_iResolution.xyz)) / u_iResolution.y; // vec3(vertPos.xy / u_iResolution.xy, 0.0);//vec3(0.0, 0.0, 0.0); // TODO Ray starts fragment position 
	//eye_dir
	// Debug to show that the screen quad is drawing correctly
	//color = vertPos;
	//return;


	for (int i = 0; i < 255; i++)
	{
		float step_size = sdfVoxelCube(ray_pos, box_pos, box_size);

		if (step_size <= 0.0)
		{
			// Norms
			vec3 difference = normalize(ray_pos - box_pos);
			difference[0] = abs(difference[0]);
			difference[1] = abs(difference[1]);
			difference[2] = abs(difference[2]);
			// HIT!
			color = vec4(difference, 1.0f);
			return;
		}
		else
		{
			// NO HIT YET!
			ray_pos = ray_pos + step_size * eye_dir.xyz;
		}
	}
	vec4 up = view * proj * vec4(0.0f, ray_pos.y, 0.0f, 1.0f);
	
	
	color = vec4(vec3(0.4f, 0.5f, 0.9f) - 0.5f * (0.5f * ray_pos.y),1.0);
	color = vec4(mix(color.xyz, vec3(0.5f), exp(-10.0f * ray_pos.y)),1.0);

	vec3 sun = normalize(vec3(0.8f, -0.4f, 0.2f));



	float sunDisc = smoothstep(0.995f, 1.0f, dot(ray_pos.xyz, sun.xyz)) * 0.25f;
	color += sunDisc * vec4(7.0f, 4.5f, 3.0f,0.0f);
	//color = vec4(clamp(acos(clamp(up.z,0.0f,1.0f)),0.0f,1.0f), clamp(acos(clamp(up.z, 0.0f, 1.0f)), 0.0f, 1.0f),1.0,1.0f); // Black

};