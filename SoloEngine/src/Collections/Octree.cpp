#include "Octree.h"

namespace Solo
{
	Octree::Octree()
	{

	}

	Octree::~Octree()
	{
	}

	void Octree::insert(depth_t depth, octIdx_t idx, data_t value)
	{
	}

	void Octree::insert(float x, float y, float z, data_t value)
	{
	}

	data_t Octree::remove(depth_t depth, octIdx_t idx)
	{
		return data_t();
	}

	data_t Octree::remove(float x, float y, float z)
	{
		return data_t();
	}

	data_t Octree::get(depth_t depth, octIdx_t idx)
	{
		return data_t();
	}

	data_t Octree::get(float x, float y, float z)
	{
		return data_t();
	}

	Octree::Node* Octree::getNode(depth_t depth, octIdx_t idx)
	{
		
		return root;
	}


}
