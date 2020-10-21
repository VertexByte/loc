#if !defined(WIN32_LOC_H)

#include <windows.h>
#include <shlwapi.h>

struct file_read_result
{
  u32 Size;
  void *Memory;
};

internal file_read_result
Win32ReadFile(char *FileName, entries_state *State)
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

	// NOTE(faruk): Uses the same buffer 64mb buffer for every file
	// which means only one allocation is needed.
	void *Memory = State->FileReadBuffer;
	
	if(Memory)
	{
	  DWORD BytesRead = 0;
	    
	  if(ReadFile(FileHandle, Memory, Size32, &BytesRead, 0))
	  {
	    Assert(BytesRead == Size32);
	    Result.Size = Size32;

	    // NOTE(faruk): Apparently we have to set the end of the file.
	    char *MemoryChar = (char *)Memory;
	    MemoryChar[Size32] = '\0';
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
Win32ProcessPath(char *Path, total_result *TotalResult,
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
	  
	  Win32ProcessPath(FullPath, TotalResult, State);
	}
      }
    }
  }
  else
  {
    char Extension[64] = {};
    GetFileExtension(Path, Extension);

    extension_check_result ExtensionCheck = {};
    ExtensionCheck = CheckExtension(State, Extension);
      
    if(ExtensionCheck.Found)
    {
      file_read_result FileReadResult = Win32ReadFile(Path, State);

      if(FileReadResult.Size > 0)
      {
	valid_input_entry *Entry = ExtensionCheck.Entry;
	
	text_buffer Buffer = {};
	Buffer.Text = (char *)FileReadResult.Memory;
	Buffer.Size = FileReadResult.Size;
	
	lines_process_result Result = ProcessTextBuffer(&Buffer, Entry);
	
	/*printf("%s [code: %d, blank: %d, comment:%d]\n",
	  Extension, Result.Code, Result.Blank, Result.Comment);*/

	
	// NOTE(faruk): The toatal lines of a ceration file type is
	// stored in the entry.
	++Entry->FileCount;
	Entry->Code += Result.Code;
	Entry->Comment += Result.Comment;
	Entry->Blank += Result.Blank;
	
	TotalResult->Count.Code += Result.Code;
	TotalResult->Count.Blank += Result.Blank;
	TotalResult->Count.Comment += Result.Comment;
	++TotalResult->RecognizedFiles;
      }
    }
    else
    {
      ++TotalResult->WeirdoFiles;
    }
  }
}

#define WIN32_LOC_H
#endif

