#include <stdio.h>
#include <stdlib.h>

#include "loc.h"
#include "win32_loc.h"

/* TODO:
   - Make directories as a valid input.

   - Implement an easy way to add a new programming language to program.

   - Sorting programming entries by file extension.
     (The program could be optimized by just not doing this step, by
     already having the entries sorted)
     
   - Binary search for files expensions when processing.
 */


internal void
Win32ProcessPath(char *Path)
{
  b32 IsDir = Win32IsDir(Path);
  if(IsDir)
  {
    Assert(!"Directories aren't a valid input for now.");
  }
  else
  {
    file_read_result FileReadResult = Win32ReadFile(Path);
    
    text_buffer Buffer = {};
    Buffer.Text = (char *)FileReadResult.Memory;
    Buffer.Size = FileReadResult.Size;

    final_process_result Result = ProcessTextBuffer(&Buffer);
    printf("%s [code: %d, blank: %d, comment:%d]\n",
	   Path, Result.Code, Result.Blank, Result.Comment);
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
      Win32ProcessPath(Path);
    }
  }
  else
  {
    printf("Usage: loc [filename] [filename] [filename] ... \n");
  }

  return(0);
}
