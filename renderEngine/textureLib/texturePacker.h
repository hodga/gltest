#pragma once

#include <string>
#include <vector>
#include <map>
#include <list>

/*
Texture packer is used in these steps:
1. add images as filenames and check getPackedImageDimensions()
2. remove images if the dimensions become too large
3. pack images
4.1 get packed image data to use with asset instances
4.2 get packed image and packed image dimensions to add as a texture in a asset or store as a file
*/

namespace hwgl 
{
	struct image_t
	{
		std::string filename; //identifies the image so it won´t repeat many times, also works as id
		std::vector<unsigned char> image;
		unsigned width;
		unsigned height;
		unsigned x;
		unsigned y;
	};

	class TexturePacker
	{
		public:
			//void addImage(vector<unsigned char> image, std::tuple<unsigned, unsigned> dimensions);
			void addImage(std::string filename);
			void removeImage(std::string filename);

			void packImages();

			void findImageDimensions();
			unsigned getPackedHeight();
			unsigned getPackedWidth();
			std::vector<unsigned char>& getPackedImage();

			std::map<std::string, image_t>& getPackedImageData();

		private:

			//raw data
			std::list<image_t> imageData;

			//products
			std::map<std::string, image_t> imageMap;
			std::vector<unsigned char> packedImage;
			unsigned packedWidth;
			unsigned packedHeight;
			

			//functions
			void findArrangement();
			
	};
}