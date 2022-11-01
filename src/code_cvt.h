#include <windows.h>

__forceinline
char *Curl_convert_wchar_to_UTF8(const wchar_t *str_w) {
  char *str_utf8 = NULL;

  if (str_w) {
    int bytes = WideCharToMultiByte(CP_UTF8, 0, str_w, -1, NULL, 0, NULL, NULL);
    if (bytes > 0) {
      str_utf8 = (char *)malloc(bytes);
      if (str_utf8) {
        if (WideCharToMultiByte(CP_UTF8, 0, str_w, -1, str_utf8, bytes, NULL,
                                NULL) == 0) {
          free(str_utf8);
          return NULL;
        }
      }
    }
  }

  return str_utf8;
}