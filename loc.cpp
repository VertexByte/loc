#include <stdio.h>
#include <stdlib.h>

#include "loc.h"
#include "win32_loc.h"

/* TODO:
   - Implement an easy way to add new file types as valid inputs.
   - Sorting the file types in lexicographical order.
   - Binary search on input when processing.
 */


internal void
Win32ProcessPath(char *Path, final_process_result *GlobalResult)
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
	  
	  Win32ProcessPath(FullPath, GlobalResult);
	}
      }
    }
  }
  else
  {
    file_read_result FileReadResult = Win32ReadFile(Path);

    if(FileReadResult.Size > 0)
    {
      text_buffer Buffer = {};
      Buffer.Text = (char *)FileReadResult.Memory;
      Buffer.Size = FileReadResult.Size;
      
      final_process_result Result = ProcessTextBuffer(&Buffer);

      char Extension[100];
      GetFileExtension(Path, Extension);
      printf("%s [code: %d, blank: %d, comment:%d]\n",
	     Extension, Result.Code, Result.Blank, Result.Comment);

      GlobalResult->Code += Result.Code;
      GlobalResult->Blank += Result.Blank;
      GlobalResult->Comment += Result.Comment;
    }
    else
    {
      printf("Couldn't open %s\n", Path);
    }
  }
}

  
int main(int ArgCount, char **ArgV)
{
  final_process_result GlobalResult = {};
  
  if(ArgCount > 1)
  {
    for(u32 Iterator = 1;
	Iterator < ArgCount;
	++Iterator)
    {
      char *Path = ArgV[Iterator];
      Win32ProcessPath(Path, &GlobalResult);
    }
  }
  else
  {
    printf("Usage: loc [filename] [filename] [filename] ... \n");
  }

  if(ArgCount > 1)
  {
    printf("\n\nTotal [code: %d  blank: %d  comment: %d]\n",
	   GlobalResult.Code, GlobalResult.Blank, GlobalResult.Comment);
  }
  return(0);
}
