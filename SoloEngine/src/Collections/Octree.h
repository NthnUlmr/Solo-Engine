#pragma once

#include <numeric>
#include <vector>
namespace Solo
{
	// The octree is a 3D spatial datastructure
	// which can be used to efficiently organize 
	// data based on its spatial coordinates.

	// This is particularly useful for a voxel engine
	// where there is a fixed grid of voxels. However,
	// Most voxels are going to either be air or have
	// neighbors of the same type of voxel.
	
	// This means the data is sparse, so the octree needs
	// to be able to both allow for fast access to neighbors
	// but also efficiency gains like LOD, reduced memory usage,
	// etc. from sparse approaches.

	// If the voxel grid is changing frequently, then the data-
	// structure needs to be optimized to reduce cache misses.

	// 

	typedef uint8_t octIdx_t;
	typedef uint8_t depth_t;
	typedef uint8_t data_t;
	typedef uint16_t width_t;
	// xyze format
	// Levels:
	// 0    1    2    3    4    5    6    7    8    9    10   11   12   13   14   15
	// 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000   
	// Side Lengths:
	// 8th  4th  2th  1m   2m   4m   8m   16m  32m  64m  128m 256  512  1024 2048 4096
	// This format should support a depth of 8 or 8^16 or approximately infinite nodes
	// This would result in practically infinite memory usage if the tree was fully 
	// populated and each node was only 1 byte of memory.
	//
	// 32 bit indices is probably safer if you plan to fully populate the tree
	// The index implicitly creates the structure of the tree.

	// To convert from the tree to 3d space:
	// depth of zero - center is at zero
	// depth of 1 - center is at (+/-) width/2 - width is maxWidth / 2^depth

	class Octree
	{
	public:


		struct Node
		{
			Node* parent = nullptr;
			Node* children[8] = { nullptr,nullptr,nullptr,nullptr,
								  nullptr,nullptr,nullptr,nullptr };
			data_t data = 0;  // value at current node
			depth_t depth = 0; // Depth in tree
			octIdx_t idx = 0; // 0-7 represents location inside parent
			width_t width2 = 1 << depth; // 2^depth
		};

		Octree();


		~Octree();

		// get nearest neighbors
		
		// insert by depth/index
		void insert(depth_t depth, octIdx_t idx, data_t value);

		// insert by world position
		void insert(float x, float y, float z, data_t value);

		// remove
		data_t remove(depth_t depth, octIdx_t idx);

		data_t remove(float x, float y, float z);

		// get
		data_t get(depth_t depth, octIdx_t idx);

		data_t get(float x, float y, float z);

		Node* getNode(depth_t depth, octIdx_t idx);

		// recursively prune

		//

	protected:

	private:
		Node* root = nullptr;

	};


}
