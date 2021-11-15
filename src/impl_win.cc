#include <windows.h>
#include <shellapi.h>
#include <shlwapi.h>
#include "impl.h"

// sanity check
static_assert(
  sizeof(wchar_t) == sizeof(std::u16string::value_type),
  "std::u16string is expected to have the same character size as std::wstring"
);

namespace {

/**
 * Construct a std::u16string from a pointer to a UTF-16, null-terminated, wide
 * C string.
 */
std::u16string U16StrFromWStr(wchar_t* str) {
  return std::u16string(reinterpret_cast<char16_t*>(str));
  // SAFETY: str is UTF-16 and we statically asserted that wchar_t has the same
  // size as char16_t
}

} // namespace

void GetFileHandlerInfo(const std::u16string& filePath, FileHandlerInfo& info) {
  // Get the file path as a wide string
  LPCWSTR filePathW = reinterpret_cast<LPCWSTR>(filePath.c_str());
  // SAFETY: str is UTF-16 and we statically asserted that wchar_t has the same
  // size as char16_t

  // Find the executable for this file path
  wchar_t execPath[MAX_PATH];
  HINSTANCE findExecResult = ::FindExecutableW(
    filePathW,
    NULL,
    execPath
  );

  // Handle errors
  bool findExecSucceeded = reinterpret_cast<int>(findExecResult) > 32;
  // SAFETY: the windows docs literally says it returns an int <= 32 for errors
  if (findExecSucceeded) {
    // Set the path on the result struct
    info.handler_path = ::U16StrFromWStr(execPath);
  }

  // Try to get a friendly name for the executable

  // First pluck the file extension off the file name
  LPCWSTR fileExt = ::PathFindExtensionW(filePathW);
  
  // Then try to query a file extension association from the registry
  const size_t kFriendlyNameBufferSize = 1024;
  // NOTE: 1024 is arbitrarily chosen and could be a dynamic value (by calling
  // AssocQueryString twice) if needed.
  wchar_t friendlyNameBuf[kFriendlyNameBufferSize];
  DWORD assocFriendlyNameSize = kFriendlyNameBufferSize;
  HRESULT assocQuerySizeResult = ::AssocQueryStringW(
    ASSOCF_NONE,
    ASSOCSTR_FRIENDLYAPPNAME,
    fileExt,
    L"open",
    friendlyNameBuf,
    &assocFriendlyNameSize
  );
  if (SUCCEEDED(assocQuerySizeResult)) {
    // Set the name on the result struct
    info.friendly_name = ::U16StrFromWStr(friendlyNameBuf);
  } else if (findExecSucceeded) {
    // Fallback to the display name of the executable shown by the shell
    SHFILEINFOW execInfo;
    DWORD_PTR execInfoResult = ::SHGetFileInfoW(
      execPath,
      0,
      &execInfo,
      sizeof(SHFILEINFOW),
      SHGFI_DISPLAYNAME
    );
    if (execInfoResult != 0) {
      // Call succeeded, set the name on the result struct
      info.friendly_name = ::U16StrFromWStr(execInfo.szDisplayName);
    }
  }
}
