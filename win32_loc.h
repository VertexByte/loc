#if !defined(WIN32_LOC_H)

#include <windows.h>
#include <shlwapi.h>

struct file_read_result
{
  u32 Size;
  void *Memory;
};

// TODO(faruk): Look at CloseHandle, i think all handles will be freed
// when the aplication terminates.
internal file_read_result
Win32ReadFile(char *FileName)
{
  file_read_result Result = {};
  
  HANDLE FileHandle = CreateFileA(FileName, GENERIC_READ, 0, 0,
				  OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,
				  0);

  if(FileHandle != INVALID_HANDLE_VALUE)
  {
    LARGE_INTEGER LargeSize;
    if(GetFileSizeEx(FileHandle, &LargeSize))
    {
      u64 Size = LargeSize.QuadPart;
      if(Size < 0xFFFFFFFF)
      {
	u32 Size32 = (u32)Size;
	void *Memory = VirtualAlloc(0, Size32, MEM_COMMIT | MEM_RESERVE,
				    PAGE_READWRITE);

	if(Memory)
	{
	  DWORD BytesRead = 0;
	    
	  if(ReadFile(FileHandle, Memory, Size32, &BytesRead, 0))
	  {
	    Assert(BytesRead == Size32);
	    Result.Size = Size32;
	    Result.Memory = Memory;
	  }
	  else
	  {
	    // TODO(faruk) Logging.
	    Assert(!"Yo");
	  }
	}
	else
	{
	  // TODO(faruk): Logging.
	  Assert(!"yo");
	}
	  
      }
      else
      {
	// TODO(faruk): File is too big.
	Assert(!"yo");
      }
    }
    else
    {
      // TODO(faruk): Logging???
      Assert(!"yo");
    }
      
  }
  else
  {
    //TODO(faruk): Logging???
    Assert(!"yo");
  }

  return(Result);
}

internal b32
Win32IsDir(char *Path)
{
  b32 Result = PathIsDirectoryA(Path);
  return(Result);
}

#define WIN32_LOC_H
#endif

