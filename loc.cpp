#include <stdio.h>
#include <stdlib.h>

#include "loc.h"
#include "win32_loc.h"

/* TODO:
   (Output)
   - Show lines of code per programming language in project.

   (Switches)
   - Switches to control the output.

   (Speed)
      "And down the rabbit hole we go!!"
                             -VertexByte
				     
   - And when we have that big buffer see can we fit multiple files into
   it, so we can batch the files.

  - Look at the way we process lines, try to make it as fast as posible.
  - NOTE(to future faruk): If you learn SIMD use it here.

  - Sorting the file types in lexicographical order.
  - Binary search on input when processing.

  (Stuff)
   - bin_search_entry is a strage name.
 */

int main(int ArgCount, char **ArgV)
{
  LARGE_INTEGER PerformanceFrequency;
  QueryPerformanceFrequency(&PerformanceFrequency);
  u64 CountsPerSecond = PerformanceFrequency.QuadPart;
  
  // Start counter
  LARGE_INTEGER Large;
  QueryPerformanceCounter(&Large);
  u64 StartCounter = Large.QuadPart;
  
  lines_process_result GlobalResult = {};

  entries_state EntriesState = {};
  InitializeValidEntries(&EntriesState);

  // NOTE(faruk): THe size is 64 mb
  EntriesState.FileReadBufferSize = (1024*1024)*64;
  EntriesState.FileReadBuffer = VirtualAlloc(0, EntriesState.FileReadBufferSize,
					     MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
  
  if(ArgCount > 1)
  {
    for(u32 Iterator = 1;
	Iterator < ArgCount;
	++Iterator)
    {
      char *Path = ArgV[Iterator];
      Win32ProcessPath(Path, &GlobalResult, &EntriesState);
    }
  }
  else
  {
    printf("\n   ---Usage--- \n\n");
    printf("Parameters: files or directories\n");
    printf("      Example:\n");
    printf("      >loc source.cpp              -Single file\n");
    printf("      >loc c:/work or c:/work/     -Directory\n\n");
    printf("      >loc source.cpp main.cpp c:/work/project c:/work/project2/\n");
    printf("\n");
    printf("Switches: TODO(faruk): Switches need to be added.\n");
  }

  if(ArgCount > 1)
  {
    printf("\n\nTotal [code: %d  blank: %d  comment: %d]\n",
	   GlobalResult.Code, GlobalResult.Blank, GlobalResult.Comment);
  }

  QueryPerformanceCounter(&Large);
  u64 EndCounter = Large.QuadPart;

  u64 Elapsed = EndCounter - StartCounter;
  r32 Seconds = (r32)Elapsed/(r32)CountsPerSecond;
  r32 Ms = Seconds*1000.0f;
  printf("Time: %.4f MS\n", Ms);
  return(0);
}
