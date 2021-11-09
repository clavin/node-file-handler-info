#include <optional>
#include <string>

#ifdef _WIN32
using os_string = std::u16string;
#else
using os_string = std::string;
#endif

struct FileHandlerInfo {
  std::optional<os_string> handler_path;
  std::optional<os_string> friendly_name;
};

/**
 * Gets information about the application that handles the given file path.
 */
void GetFileHandlerInfo(os_string filePath, FileHandlerInfo& info);
