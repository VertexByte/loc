#include <stdio.h>
#include <stdlib.h>

#include "loc.h"
#include "win32_loc.h"

/* TODO:

   (Switches)
   - Switches to control the output.

   (Speed)
      "And down the rabbit hole we go!!"
                             -VertexByte
				     
   -Add a simple way to mesure performance of the program.

   -Have one big buffer for a file's memory rather than allocation memory
   every time we want to read a file.(The buffer will be about 256mb)

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
  lines_process_result GlobalResult = {};

  entries_state EntriesState = {};
  InitializeValidEntries(&EntriesState);
  
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

  return(0);
}
