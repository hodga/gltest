#include "testObject.h"

using namespace hwge;
using namespace std;

TestObject::TestObject() {}
TestObject::TestObject(string nImagePath) 
{
	imagePath = nImagePath;
}

string TestObject::getImagePath() 
{
	return imagePath;
}