#import <Foundation/Foundation.h>
#include <iostream>
#include "impl.h"

void GetFileHandlerInfo(const std::string& filePath, FileHandlerInfo& info) {
  // Ask launch services for the application which handles this file path
  NSString* filePathNSStr =
    [NSString
      stringWithCString:filePath.c_str()
      encoding:[NSString defaultCStringEncoding]];
  NSURL* filePathNSUrl = [NSURL fileURLWithPath:filePathNSStr];

  CFURLRef handlerUrl = LSCopyDefaultApplicationURLForURL(
    (CFURLRef)filePathNSUrl,
    kLSRolesAll,
    NULL
  );
  if (handlerUrl == NULL) {
    // Likely either no file exists at that path, or there is no application
    // that handles the file.
    return;
  }

  // Convert the url back to a file path & set it on the result struct
  NSString* handlerPathNSStr = [(NSURL*)handlerUrl path];
  info.handler_path = std::string([handlerPathNSStr UTF8String]);

  // Get a friendly name for the handler & set it on the result struct
  NSString* handlerFriendlyNameNSStr =
    [[NSFileManager defaultManager] displayNameAtPath:handlerPathNSStr];
  
  // The `displayNameAtPath:` message returns the path argument if something
  // went wrong.
  if (![handlerFriendlyNameNSStr isEqualToString:handlerPathNSStr]) {
    info.friendly_name = std::string([handlerFriendlyNameNSStr UTF8String]);
  }

  CFRelease(handlerUrl);
}
