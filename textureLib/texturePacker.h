#pragma once

#include <string>

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
		unsigned filename; //identifies the image so it won´t repeat many times, alos works as id
		vector<unsigned char> image;
		unsigned width;
		unsigned height;
		unsigned x;
		unsigned y;
	};

	class TexturePacker
	{
		public:
			//void addImage(vector<unsigned char> image, std::tuple<unsigned, unsigned> dimensions);
			void addImage(string filename);
			void removeImage(string filename);

			void packImages();

			std::tuple<unsigned, unsigned>& getPackedImageDimensions();
			vector<unsigned char>& getPackedImage();

			list<image_t>& getPackedImageData();

		private:

			//products
			list<image_t> imageData;
			std::tuple<unsigned, unsigned> packedImageDimensions;
			vector<unsigned char> packedImage;
			
	};
}