#include <stdio.h>
#include <stdlib.h>

typedef int s32;
typedef unsigned int u32;
typedef char s8;
typedef unsigned int u8;
typedef short s16;
typedef unsigned short u16;
typedef long s64;
typedef unsigned long u64;

typedef s32 b32;

// VERSION(0.01): 


int main(int ArgCount, char **ArgV)
{
  if(ArgCount > 1)
  {
    for(u32 Iterator = 1;
	Iterator < ArgCount;
	++Iterator)
    {
      char *FileName = ArgV[Iterator];
      FILE *File = fopen(FileName, "rb");
      
      if(File)
      {
	// NOTE(faruk): Get the file size.
	fseek(File, 0, SEEK_END);
	u32 FileSize = ftell(File);
	rewind(File);
	
	// NOTE(faruk): Allocate the text buffer
	void *Memory = (void *)malloc(FileSize);

	if(fread(Memory, sizeof(char), FileSize, File))
	{
	  char *Buffer = (char *)Memory;
	  char C = *Buffer++;
	  while(C != 0)
	  {
	    putchar(C);
	    C = *Buffer++;
	  }
	}
	// NOTE(faruk): Final output.
	printf("%s (%d bytes)\n", FileName, FileSize);
      }
      else
      {
	printf("Couldn't find the file: %s\n", FileName);
      }
    }
  }
  else
  {
    printf("Usage: loc [filename] [filename] [filename] ... \n");
  }
  return(0);
}
