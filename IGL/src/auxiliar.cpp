#include <IGL/auxiliar.h>
#include <IGL/BOX.h>

#ifdef _WIN32
#include <windows.h>
#endif

#include <fstream>
#include <cstring>

// Texture loading
#include <stb_image.h>

//////////////////////////////////////////
// Already implemented auxiliar functions

// Shader loading function
char *loadStringFromFile(const char *fileName, unsigned int &fileLen)
{
	// File load
	std::ifstream file;
	file.open(fileName, std::ios::in);
	if (!file)
		return 0;

	// Compute file length
	file.seekg(0, std::ios::end);
	fileLen = (unsigned int)file.tellg();
	file.seekg(std::ios::beg);

	// File reading
	char *source = new char[fileLen + 1];

	int i = 0;
	while (file.good())
	{
		source[i] = char(file.get());
		if (!file.eof())
			i++;
		else
			fileLen = i;
	}
	source[fileLen] = '\0';
	file.close();

	return source;
}

unsigned char *loadTexture(const char *fileName, unsigned int &w, unsigned int &h)
{
	// OpenGL expects the first row of the image to be the bottom one, but
	// stb_image reads top-down, so we ask it to flip the image while loading.
	stbi_set_flip_vertically_on_load(1);

	int width = 0, height = 0, channels = 0;

	// The last argument forces 4 channels (RGBA), whatever the file stores
	unsigned char *img = stbi_load(fileName, &width, &height, &channels, 4);
	if (img == NULL)
		return NULL;

	w = (unsigned int)width;
	h = (unsigned int)height;

	// stb_image allocates with malloc, but callers free this buffer with
	// delete[], so we hand back a copy owned by new[]
	unsigned char *map = new unsigned char[4 * w * h];
	memcpy(map, img, 4 * w * h);
	stbi_image_free(img);

	return map;
}
