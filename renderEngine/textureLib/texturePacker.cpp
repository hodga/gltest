
#include "texturePacker.h"
#include <assert.h>
#include <iostream>
#include <algorithm>
#include <math.h>
#include "lodepng.h"

using namespace hwgl;
using namespace std;

unsigned pow2roundup (unsigned x);

void TexturePacker::addImage(string filename)
{
	assert(packedImage.empty());
cout << "filename: " << filename << endl;
	//only add a file once, the filename is the identifier
	list<image_t>::iterator imageIterator = find_if(imageData.begin(), imageData.end(), [filename](const image_t& i) -> bool { return i.filename == filename;} );
	if(imageIterator != imageData.end()) return;
cout << "bb" << endl;
	image_t subImage;
	subImage.filename = filename;

	unsigned error = lodepng::decode(subImage.image, subImage.width, subImage.height, filename);
	cout << "dimensions: " << subImage.height << " : " << subImage.width << endl;
	if(error != 0)
	{
		cout << "error reading file" << error << " : " << lodepng_error_text(error) << endl;
		throw runtime_error("error reading image file");
	}

	if(!error) imageData.push_back(subImage);

	for(auto image : imageData)
	{
		cout << "image: " << image.filename << " h: " << image.height << " w: " << image.width << " vector size: " << image.image.size() << endl;
	}
}

void TexturePacker::removeImage(string filename)
{
	assert(packedImage.empty());

	list<image_t>::iterator imageIterator = find_if(imageData.begin(), imageData.end(), [filename](const image_t& i) -> bool { return i.filename == filename; } );
	assert(imageIterator != imageData.end());

	imageData.erase(imageIterator);
}

void TexturePacker::packImages()
{
	assert(packedImage.empty());
	assert(!imageData.empty());
	cout << "hello!" << endl;
	findArrangement();
cout << "aaa" << endl;
	packedImage.resize(packedWidth*packedHeight*4, 255);

	for(auto img : imageData) 
	{
		for(int j = 0; j < img.height;j++) 
		{
			for(int i = 0; i < img.width;i++) 
			{
				unsigned x = img.x + i;
				unsigned y = img.y + j;
				unsigned packedIndex = (x *4) + (y*4) * packedWidth;

				unsigned imageIndex = (i*4) + (j*4) * img.width;
				packedImage[packedIndex] = img.image[imageIndex];
				packedImage[packedIndex+1] = img.image[imageIndex+1];
				packedImage[packedIndex+2] = img.image[imageIndex+2];
				packedImage[packedIndex+3] = img.image[imageIndex+3];
			}
		}
		img.image.clear();
	}
cout << "ccc" << endl;
	unsigned error = lodepng::encode("./packed.png", packedImage, packedWidth, packedHeight);
	if(error != 0)
	{
		cout << "error reading file" << error << " : " << lodepng_error_text(error) << endl;
		throw runtime_error("error reading image file");
	}
}

void TexturePacker::findImageDimensions()
{
	assert(packedImage.empty());
	findArrangement();
}

unsigned TexturePacker::getPackedHeight() {
	return packedHeight;
}

unsigned TexturePacker::getPackedWidth() {
	return packedWidth;
}

vector<unsigned char>& TexturePacker::getPackedImage()
{
	assert(!packedImage.empty());
	return packedImage;
}

map<string, image_t>& TexturePacker::getPackedImageData()
{
	assert(!packedImage.empty());

	if(imageMap.empty())
	{
		for(auto &img : imageData) 
		{
			imageMap[img.filename] = img;
		}
	}

	return imageMap;
}

void TexturePacker::findArrangement()
{
	packedWidth = packedHeight = 0;

	if(imageData.size() == 0) return;

	//sort by height so that the highest images go first
	imageData.sort([](const image_t& a, const image_t& b) 
	 	{ 
	 		return a.height > b.height;
	 	});

	//find optimal image size;	
	unsigned tempS = 0;
	for(auto img : imageData)
	{
		tempS += (img.height * img.width);
	}
	tempS = sqrt(tempS);
	unsigned s = tempS;//pow2roundup(tempS);
	//assert(s >= tempS);

	unsigned placedImages = 0;
	packedWidth = packedHeight = 0;
	//attempt packing images and increasing frame until all can fit inside a square with sides s.
	while(placedImages < imageData.size())
	{
		placedImages = 0;
		unsigned curX = 0, curY = 0;
		unsigned curHeight = imageData.begin()->height;

		packedHeight = curY + curHeight;

		for(image_t &img : imageData)
		{
			if(curX+img.width <= s) //place images from left to right as long as there is space
			{
				img.x = curX;
				img.y = curY;
				curX += img.width;
				placedImages++;

				if(curX > packedWidth)
					packedWidth = curX;
			}
			else if(curY + curHeight + img.height <= s) // start from left on line below
			{
				img.x = 0;
				img.y = curY + curHeight;
				curX = img.width;
				curY = curY + curHeight;
				curHeight = img.height;
				placedImages++;

				packedHeight = curY + curHeight;
			}
			else //if there is no space, start over with a bigger s. (probably just one time?)
			{
				s *= 1.01;
				break;
			}
		}
	}
}

unsigned pow2roundup (unsigned x)
{
    --x;
    x |= x >> 1;
    x |= x >> 2;
    x |= x >> 4;
    x |= x >> 8;
    x |= x >> 16;
    return x+1;
}
