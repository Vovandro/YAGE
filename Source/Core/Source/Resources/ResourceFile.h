//
// Created by mrjaqbq on 01.06.16.
//

#ifndef GAME_RESOURCEFILE_H
#define GAME_RESOURCEFILE_H

#include <Utils/include/utils/File.h>
#include <Utils/include/utils/Handle.h>

namespace Core
{
	class ResourceFile
	{
		Utils::RawHandle handle;
		Utils::File file;
	};

	class FileTrait
	{

	};
}

#endif //GAME_RESOURCEFILE_H
