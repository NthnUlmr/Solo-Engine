#include "Image3D.h"
#include <assert.h>
#include <stb_image.h>
#include <glm.hpp>
#include <vector>
#include <iostream>
namespace Solo {

	Image3D::Image3D(std::string path)
		: path_(path)
	{
		/// \todo Double check path is valid so can catch error and give user useful error
		//stbi_set_flip_vertically_on_load(false);
		//data_ = stbi_load(path_.c_str(), &width_, &height_, &nChannels_, 0);
		width_ = 10;
		height_ = 10;
		depth_ = 10;
		nChannels_ = 2;
		

		data_ = new float[width_ * height_ * depth_ * nChannels_];

		for (int ii = 0; ii < width_ * height_ * depth_ * nChannels_; ii ++)
		{
			data_[ii] = 100.0;
		}

		int scale = 1.0;

		for (int ii = 0; ii < width_; ii++)
		{
			//std::cout << "i " << ii << std::endl;
			for (int jj = 0; jj < height_; jj++)
			{
				//std::cout << " j " << jj << std::endl;
				for (int kk = 0; kk < depth_; kk++)
				{

					//std::cout << "  k " << kk << std::endl;
					float intermed = (float)(sdfVoxelCube(glm::vec3((double)ii, (double)jj, (double)kk), glm::vec3(2.5 * scale), 0.5 * scale));
					intermed = std::min(intermed, (float)(sdfVoxelCube(glm::vec3((double)ii, (double)jj, (double)kk), glm::vec3(1.5 * scale), 0.5 * scale)));
					intermed = std::min(intermed, (float)(sdfVoxelCube(glm::vec3((double)ii, (double)jj, (double)kk), glm::vec3(3.5 * scale), 0.5 * scale)));
					intermed = std::min(intermed, (float)(sdfVoxelCube(glm::vec3((double)ii, (double)jj, (double)kk), glm::vec3(5.5 * scale), 0.5 * scale)));
					intermed = std::min(intermed, (float)(sdfVoxelCube(glm::vec3((double)ii, (double)jj, (double)kk), glm::vec3(4.5 * scale), 0.5 * scale)));
					intermed = std::min(intermed, (float)(sdfVoxelCube(glm::vec3((double)ii, (double)jj, (double)kk), glm::vec3(3.5 * scale, 5.5 * scale, 5.5 * scale), 0.5 * scale)));
					intermed = std::min(intermed, (float)(sdfVoxelCube(glm::vec3((double)ii, (double)jj, (double)kk), glm::vec3(6.5 * scale), 0.5 * scale)));
					intermed = std::min(intermed, (float)(sdfVoxelCube(glm::vec3((double)ii, (double)jj, (double)kk), glm::vec3(7.5 * scale), 0.5 * scale)));
					//intermed = std::min(intermed, (float)(sdfVoxelCube(glm::vec3((double)ii, (double)jj, (double)kk), glm::vec3(8.5 * scale), 0.5* scale)));
					intermed = std::min(intermed, (float)(sdfVoxelCube(glm::vec3((double)ii, (double)jj, (double)kk), glm::vec3(1.5 * scale, 6.5 * scale, 5.5 * scale), 0.5 * scale)));
					intermed = std::min(intermed, (float)(sdfVoxelCube(glm::vec3((double)ii, (double)jj, (double)kk), glm::vec3(1.5 * scale, 7.5 * scale, 5.5 * scale), 0.5 * scale)));
					intermed = std::min(intermed, (float)(sdfVoxelCube(glm::vec3((double)ii, (double)jj, (double)kk), glm::vec3(1.5 * scale, 5.5 * scale, 5.5 * scale), 0.5 * scale)));
					intermed = std::min(intermed, (float)(sdfVoxelCube(glm::vec3((double)ii, (double)jj, (double)kk), glm::vec3(2.5 * scale, 5.5 * scale, 5.5 * scale), 0.5 * scale)));
					intermed = std::min(intermed, (float)(sdfVoxelCube(glm::vec3((double)ii, (double)jj, (double)kk), glm::vec3(3.5 * scale, 5.5 * scale, 5.5 * scale), 0.5 * scale)));
					intermed = std::min(intermed, (float)(sdfVoxelCube(glm::vec3((double)ii, (double)jj, (double)kk), glm::vec3(4.5 * scale, 5.5 * scale, 5.5 * scale), 0.5 * scale)));
					intermed = std::min(intermed, (float)(sdfVoxelCube(glm::vec3((double)ii, (double)jj, (double)kk), glm::vec3(6.5 * scale, 5.5 * scale, 5.5 * scale), 0.5 * scale)));
					intermed = std::min(intermed, (float)(sdfVoxelCube(glm::vec3((double)ii, (double)jj, (double)kk), glm::vec3(7.5 * scale, 5.5 * scale, 5.5 * scale), 0.5 * scale)));
					//std::cout << intermed / 10.0f << "\t";
					data_[ii * nChannels_ + jj * height_ * nChannels_ + kk * height_*depth_ * nChannels_] = intermed / 10.0f;
					if (intermed < 0.00001)
					{
						data_[ii * nChannels_ + jj * height_ * nChannels_ + kk * height_ * depth_ * nChannels_ + 1] = (2.5 + kk / 10.0) / 10.0;
					} else
					{
						data_[ii * nChannels_ + jj * height_ * nChannels_ + kk * height_ * depth_ * nChannels_ + 1] = 0;
					}
				
				}
				//std::cout << std::endl;
			}
		}

		//data_[888] = 1;
		////data_[777] = 1;
		////data_[666] = 1;
		//data_[555] = 1;
		////data_[444] = 1;
		////data_[333] = 1;
		////data_[222] = 1;
		//data_[111] = 1;
		//
		//
		////data_[544] = 1;
		////data_[633] = 1;
		////data_[722] = 1;
		////data_[811] = 1;

		////data_[454] = 1;
		////data_[363] = 1;
		////data_[272] = 1;
		//data_[181] = 1;

		////data_[445] = 1;
		////data_[336] = 1;
		////data_[227] = 1;
		//data_[118] = 1;

		//data_[811] = 1;

		//data_[188] = 1;
		//data_[818] = 1;
		//data_[881] = 1;

		assert(data_);
		if (data_)
			isLoaded_ = true;
		else isLoaded_ = false;

	}

	Image3D::~Image3D()
	{
	}

	void Image3D::free()
	{
		if (isLoaded_)
		{
			/*stbi_image_free(data_);*/
			isLoaded_ = false;
		}
	}



	double Image3D::sdfVoxelCube(glm::vec3 p, glm::vec3 b, double size) {
		glm::vec3 d = abs(p - b) - glm::vec3((float)size);
		return glm::min((double)glm::max(d.x, glm::max(d.y, d.z)), 0.0) + glm::length(glm::max(d, glm::vec3(0.0)));
	}
}
