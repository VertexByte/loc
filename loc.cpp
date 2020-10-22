#include <stdio.h>
#include <stdlib.h>

#include "loc.h"
#include "win32_loc.h"

/* TODO:
*/

int main(int ArgCount, char **ArgV)
{
  total_result TotalResult = {};
  
  LARGE_INTEGER PerformanceFrequency;
  QueryPerformanceFrequency(&PerformanceFrequency);
  u64 CountsPerSecond = PerformanceFrequency.QuadPart;
  
  // Start counter
  LARGE_INTEGER Large;
  QueryPerformanceCounter(&Large);
  u64 StartCounter = Large.QuadPart;

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
      Win32ProcessPath(Path, &TotalResult, &EntriesState);
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
  }

  if(ArgCount > 1)
  {
    printf("\n  ---- Result ---- \n");
    printf("\nProcessed files: %d\n", TotalResult.RecognizedFiles);
    printf("Weirdo files: %d\n\n", TotalResult.WeirdoFiles);
      
    for(u32 Index = 0;
	Index < EntriesState.ValidEntriesCount;
	++Index)
    {
      valid_input_entry *Entry = &EntriesState.ValidEntries[Index];
      if(Entry->FileCount > 0)
      {
	printf("%s [files: %d code: %d  comment: %d  blank: %d]\n",
	       Entry->TypeConfig.Name, Entry->FileCount, Entry->Code,
	       Entry->Comment, Entry->Blank);
      }
    }
    
    printf("\n\nTOTAL  [code: %d  comment: %d  blank: %d]\n\n",
	   TotalResult.Count.Code, TotalResult.Count.Comment,
	   TotalResult.Count.Blank);
  }

  QueryPerformanceCounter(&Large);
  u64 EndCounter = Large.QuadPart;

  u64 Elapsed = EndCounter - StartCounter;
  r32 Seconds = (r32)Elapsed/(r32)CountsPerSecond;
  r32 Ms = Seconds*1000.0f;
  printf("Time: %.4f MS\n", Ms);

  r32 FilesPerSecond = (r32)TotalResult.RecognizedFiles/Seconds;
  printf("Speed: %.4f files per second\n", FilesPerSecond);
  return(0);
}
