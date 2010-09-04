#ifndef FILE_STORAGE
#define FILE_STORAGE

#include "someplayer.h"
#include "storage.h"

// represents file-level storage
// it store data into separate files (e.g. playlist)

namespace SomePlayer {
	namespace Storage {

		class FileStorage : public Storage {
		};

	};
};

#endif
