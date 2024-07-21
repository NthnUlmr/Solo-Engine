#pragma once
#include <string>

#include <glm.hpp>
namespace Solo {

	class Image3D
	{
	public:
		Image3D(std::string path);
		Image3D() = default;
		~Image3D();

		void free();


		void change();

		inline float* getData() const
		{
			return data_;
		};


		inline int getWidth() const
		{
			return width_;
		};

		inline int getHeight() const
		{
			return height_;
		};

		inline int getDepth() const
		{
			return depth_;
		};

		inline int getNChannels() const
		{
			return nChannels_;
		};

	private:
		float* data_ = nullptr;
		bool isLoaded_ = false;
		int width_ = 0;
		int height_ = 0;
		int depth_ = 0;
		int nChannels_ = 0;
		std::string path_ = "";

		double sdfVoxelCube(glm::vec3 p, glm::vec3 b, double size);
	};

} //namespace Image3D
