#include "storage.h"


namespace hondo {

std::string StorageEngine::get_file_contents(std::string filename)
{
	std::FILE *fp = std::fopen(filename.c_str(), "rb");
	if (fp)
	{
		std::string contents;
		std::fseek(fp, 0, SEEK_END);
		contents.resize(std::ftell(fp));
		std::rewind(fp);
		std::fread(&contents[0], 1, contents.size(), fp);
		std::fclose(fp);
		return(contents);
	}
	throw(errno);
}

bool StorageEngine::dir_exists(const char* dir_name)
{
	struct stat buffer;
	return (stat(dir_name, &buffer) == 0 && buffer.st_mode & S_IFDIR);
}

}