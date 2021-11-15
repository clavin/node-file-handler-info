#include <string>

#ifdef _WIN32
using os_string = std::u16string;
#else
using os_string = std::string;
#endif

struct FileHandlerInfo {
  os_string handler_path{};
  os_string friendly_name{};
};

/**
 * Gets information about the application that handles the given file path.
 */
void GetFileHandlerInfo(const os_string& filePath, FileHandlerInfo& info);
