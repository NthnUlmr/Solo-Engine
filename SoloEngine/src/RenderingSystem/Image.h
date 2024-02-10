#pragma once
#include <string>
namespace Solo {

	class Image
	{
	public:
		Image(std::string path);
		Image() = default;
		~Image();

		void free();

		inline unsigned char* getData() const
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

		inline int getNChannels() const
		{
			return nChannels_;
		};

	private:
		unsigned char* data_ = nullptr;
		bool isLoaded_ = false;
		int width_ = 0;
		int height_ = 0;
		int nChannels_ = 0;
		std::string path_ = "";

	};

} //namespace Image
