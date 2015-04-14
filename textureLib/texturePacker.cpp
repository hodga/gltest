
#include "texturePacker.h"
#include <assert.h>
#include "lodepng.h"


void findArrangement();
int pow2roundup (int x);

void TexturePacker::addImage(string filename)
{
	assert(packedImage.empty);

	//only add a file once, the filename is the identifier
	list<image_t>::iterator imageIterator = find_if(lmageData.begin(), imageData.end(), [$filename] (const image_t& i) { return i.filemane == filename; } );
	if(imageIterator == imageData.end()) return;


	image_t subImage;
	subImage.filename = filename;
	unsigned width, height;

	unsigned error = lodepng::decode(subImage.image, subImage.width, subImage.height, filename);

	if(error != 0)
	{
		cout << "error reading file" << error << " : " << lodepng_error_text(error) << endl;
		throw runtime_error("error reading image file");
	}

	if(!error) imageData.push_back(subImage);
}

void TexturePacker::removeImage(string filename)
{
	assert(packedImage.empty);

	list<image_t>::iterator imageIterator = find_if(lmageData.begin(), imageData.end(), [$filename] (const image_t& i) { return i.filemane == filename; } );
	assert(imageIterator != imageData.end());

	imageData.erase(imageIterator);
}

void TexturePacker::packImages()
{
	assert(packedImage.empty);

	findArrangement();

}

std::tuple<unsigned, unsigned>& TexturePacker::getPackedImageDimensions()
{
	assert(!packedImage.empty);
	return packedImageDimensions;
}

vector<unsigned char>& TexturePacker::getPackedImage()
{
	assert(!packedImage.empty);
	return packedImage;
}

list<image_t>& TexturePacker::getPackedImageData()
{
	assert(!packedImage.empty);
	return imageData;
}

void findArrangement()
{
	//sort by height so that the highest images go first
	imageData.sort([$filename] (const image_t& a, const image_t& b) 
	 	{ 
	 		return a.height > b.height;
	 	});

	//find ideal image size;	
	unsigned s = 0;
	for(auto img : imageData)
	{
		s += (img.height * img.width);
	}
	unsigned a = s;
	s = pow2roundup(s);
	assert(s >= a);

	unsigned placedImages = 0;
	unsigned curX = 0, curY = 0, curHeight = 0;

	//attempt packing images and increasing frame until all can fit inside a square with sides s.
	while(placedImages < imageData.size)
	{
		placedImages = 0;

		for(image_t img : imageData)
		{
			if(curX+img.width <= s) //place images from left to right as long as there is space
			{
				img.x = curX;
				img.y = curY;
				curX = curX+img.width;
				placedImages++;
			}
			else if(curY + curHeight + img.height <= s) // start from left on line below
			{
				img.x = 0;
				img.y = curY + curHeight;
				curX = img.width;
				curY = curY + curHeight;
				curHeight = img.height;placedImages++;
			}
			else //if there is no space, start over with a bigger s. (probably just one time?)
			{
				s *=2
				break;
			}
		}
	}
}

unsigned pow2roundup (unsigned x)
{
    if (x < 0)
        return 0;
    --x;
    x |= x >> 1;
    x |= x >> 2;
    x |= x >> 4;
    x |= x >> 8;
    x |= x >> 16;
    return x+1;
}
