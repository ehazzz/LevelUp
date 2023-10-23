#include "Image.h"

#include <filesystem>
#include <fstream>
#include <string>
#include <vector>



using namespace std;
using namespace std::filesystem;


static vector<path> GetFiles(const path& inPath, const vector<string>& filters)
{
	vector<typename path> ret;
	for (auto& entry : recursive_directory_iterator(inPath))
	{
		if (!entry.is_regular_file())
			continue;

		for(auto& filter : filters)
			if (filter == entry.path().extension())
			{
				ret.push_back(entry);
				break;
			}
	}

	return ret;
}

static Void ForceTo24BitForBMP(const path& inPath)
{
	for (auto& selected : GetFiles(inPath, { ".bmp" }))
	{
		SizeType imageSize = file_size(selected);
		Byte* image = Allocator::Allocate<Byte>(imageSize);

		{
			fstream in(selected, std::ios::in | std::ios::binary);
			in.read(reinterpret_cast<char*>(image), imageSize);
		}

		Bitmap from = Bitmap::Decode(image, imageSize);
		if (from.GetChannelCount() == 3) continue;

		Bitmap to(from.GetWidth(), from.GetHeight(), Bitmap::Format::B8G8R8);
		for (SizeType y = 0; y < from.GetHeight(); y++)
			for (SizeType x = 0; x < from.GetHeight(); x++)
				to.SetPixel(
					x, y,
					from.GetPixel(x, y)
				);

		{
			auto encoded = Bitmap::Encode(to);

			fstream out(selected, std::ios::out | std::ios::binary | std::ios::trunc);
			out.write(reinterpret_cast<const char*>(encoded.GetBuffer()), encoded.GetByteCount());
		}
	}
}


int EntryPoint()
{
	constexpr Image::Format To = Image::Format::BMP;
	//vector<string> filters = { ".bmp", ".png", ".jpg" };
	vector<string> filters = { ".png", ".jpg" };

	path InFolder("Input");
	path OutFolder("Output");


	if (!is_directory(InFolder)) return -1;

	if (is_directory(OutFolder))
		remove_all(OutFolder);

	create_directory(OutFolder);

	for (auto& selected : GetFiles(InFolder, filters))
	{
		SizeType imageSize = file_size(selected);
		Byte* image = Allocator::Allocate<Byte>(imageSize);

		{
			fstream in(selected, std::ios::in | std::ios::binary);
			in.read(reinterpret_cast<char*>(image), imageSize);
		}

		HeapMemory converted;
		{
			auto extension = selected.extension();

			if (extension == ".bmp")
				converted = Image::Convert<Image::Format::BMP, To>(image, imageSize);

			if (extension == ".png")
				converted = Image::Convert<Image::Format::PNG, To>(image, imageSize);

			if (extension == ".jpg")
				converted = Image::Convert<Image::Format::JPG, To>(image, imageSize);
		}

		{
			string extension;
			if (To == Image::Format::BMP) extension = ".bmp";
			if (To == Image::Format::PNG) extension = ".png";
			if (To == Image::Format::JPG) extension = ".jpg";

			fstream out(OutFolder / selected.replace_extension(extension).filename(), std::ios::out | std::ios::binary);
			out.write(reinterpret_cast<char*>(converted.GetBuffer()), converted.GetByteCount());
		}

		Allocator::Release(image);
	}

	if (To == Image::Format::BMP) ForceTo24BitForBMP(OutFolder);

	return 0;
}


int main(int count, char** arguments)
{
	return EntryPoint();
}