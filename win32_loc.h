#if !defined(WIN32_LOC_H)

#include <windows.h>
#include <shlwapi.h>

struct file_read_result
{
  u32 Size;
  void *Memory;
};

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
	  }
	}
	else
	{
	  // TODO(faruk): Logging.
	}
	  
      }
      else
      {
	// TODO(faruk): File is too big.
      }
    }
    else
    {
      // TODO(faruk): Logging???
    }
      
  }
  else
  {
    //TODO(faruk): Logging???
  }

  return(Result);
}

internal b32
Win32IsDir(char *Path)
{
  b32 Result = PathIsDirectoryA(Path);
  return(Result);
}

internal void
Win32ProcessPath(char *Path, lines_process_result *GlobalResult,
		 entries_state *State)
{  
  b32 IsDir = Win32IsDir(Path);
  if(IsDir)
  {
    char DirPath[100] = {};
    CopyString(DirPath, Path);
    ConcatString(DirPath, "/*");

    WIN32_FIND_DATA Data = {};
    HANDLE Handle = FindFirstFileA(DirPath, &Data);

    if(Handle != INVALID_HANDLE_VALUE)
    {
      while(FindNextFile(Handle, &Data))
      {
	char FullPath[100] = {};
	char *SubPath = Data.cFileName;
	
	if(SubPath[0] != '.')
	{
	  CopyString(FullPath, Path);
	  ConcatString(FullPath, "/");
	  ConcatString(FullPath, SubPath);
	  
	  Win32ProcessPath(FullPath, GlobalResult, State);
	}
      }
    }
  }
  else
  {
    file_read_result FileReadResult = Win32ReadFile(Path);

    char Extension[64] = {};
    GetFileExtension(Path, Extension);

    extension_check_result ExtensionCheck = {};
    ExtensionCheck = CheckExtension(State, Extension);
      
    if(FileReadResult.Size > 0 && ExtensionCheck.Found)
    {
      valid_input_entry *Entry = ExtensionCheck.Entry;
      
      text_buffer Buffer = {};
      Buffer.Text = (char *)FileReadResult.Memory;
      Buffer.Size = FileReadResult.Size;
      
      lines_process_result Result = ProcessTextBuffer(&Buffer, Entry);
      
      /*printf("%s [code: %d, blank: %d, comment:%d]\n",
	Extension, Result.Code, Result.Blank, Result.Comment);*/
      
      GlobalResult->Code += Result.Code;
      GlobalResult->Blank += Result.Blank;
      GlobalResult->Comment += Result.Comment;
    }
    else
    {
      //printf("Couldn't open %s\n", Path);
    }
  }
}

#define WIN32_LOC_H
#endif

