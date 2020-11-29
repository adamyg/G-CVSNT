#include "fileio.h"
#include <stdio.h>
#if !_MSC_VER
#include <unistd.h>
#endif

bool blob_rename_file(const char*from, const char*to)//the only common implementation on posix and windows
{
  return rename(from, to) >= 0;
}

#if _MSC_VER

#include <stdlib.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

size_t blob_get_file_size(const char* fn)
{
  struct __stat64 buf;
  if (_stat64(fn, &buf) != 0)
      return 0; // error, could use errno to find out more

  return buf.st_size;
}

bool blob_is_file_readable(const char* fn)
{
  DWORD fa = GetFileAttributes(fn);
  if (fa == INVALID_FILE_ATTRIBUTES)
	return false;
  return true;
}


void blob_unlink_file(const char* f) { _unlink(f); }

bool blob_ensure_dir(const char* name)
{
  return CreateDirectoryA(name, NULL) != FALSE;
}


FILE* blob_get_temp_file (std::string &fn, const char *tmp_path, const char *mode)
{
  char tempdir[_MAX_PATH];
  if (tmp_path == nullptr && !GetTempPathA(sizeof(tempdir), tempdir))
    return nullptr;
  char tempFileName[_MAX_PATH];
  if (!GetTempFileNameA(tmp_path ? tmp_path : tempdir, "hash_blob",0,tempFileName))
    return nullptr;
  fn = tempFileName;
  FILE *f;
  if (fopen_s(&f, fn.c_str(), mode ? mode : "wb") != 0)
    return nullptr;
  return f;
}

#else

size_t blob_get_file_size(const char* fn)
{
  struct stat sb;
  if (stat(fn, &sb) == -1)
    return 0;
  return sb.st_size;
}

bool blob_is_file_readable(const char* fn)
{
  return access(fn, R_OK) == 0;
}


void blob_unlink_file(const char* f) { unlink(f); }

bool blob_ensure_dir(const char* name)
{
  int ret = mkdir(name, 0777);
  if (ret == 0)
    return true;
  if (ret == -1 && errno == EEXIST)
    return true;
  return false;
}


FILE* blob_get_temp_file (std::string &fn, const char *tmp_path, const char *mode)
{
  FILE *fp = nullptr;

  char buf[1024];
  std::snprintf(buf, sizeof(buf), "%s/hash_blobXXXXXX", tmp_path ? tmp_path : "/tmp");
  int fd = mkstemp (buf);
  if (fd == -1)
    return nullptr;
  if ((fp = fdopen (fd, mode = mode ? mode : "wb")) == NULL)
  {
    close (fd);
    unlink(buf);
  } else
    fn = buf;
  return fp;
}
#endif