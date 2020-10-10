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

#define internal static
#define local_persist static
#define global_variable static

#define Assert(Expression) if(!(Expression)){ *(u32 *)0  = 0; }

/* TODO:
   - Make directories as a valid input.
     - Switch to Win32 File IO

   - Implement an easy way to add a new programming language to program.

   - Sorting programming entries by file extension.
     (The program could be optimized by just not doing this step, by
     already having the entries sorted)
     
   - Binary search for files expensions when processing.
 */

internal void
ReadFileText(char *Buffer, FILE *File)
{
  char *Buff = Buffer;
  
  char C = fgetc(File);
  while(!feof(File))
  {
    *Buff++ = C;
    C = fgetc(File);
  }
  
  *Buff = '\0';
}

internal char *
GetSubStringCopy(char *Buffer, u32 Start, u32 End)
{
  // NOTE(faruk): +1 is for the string terminator('\0').
  u32 Size = (End - Start) + 1;
  
  char *Result = (char *)malloc(Size);
  char *ResultPointer = Result;
  
  for(u32 Index = Start;
      Index < End;
      ++Index)
  {
    *ResultPointer++ = Buffer[Index];
  }

  *ResultPointer = '\0';
  return(Result);
}

struct text_buffer
{
  char *Text;
  u32 Size;
  u32 LocationInText;
  b32 AtEnd;
};
  
internal char *
GetLine(text_buffer *Buffer)
{
  if(Buffer->AtEnd)
  {
    return(NULL);
  }
  
  u32 StartLocation = Buffer->LocationInText;
  u32 Index = StartLocation;

  char *Text = Buffer->Text;
  
  while(Text[Index] != '\0' &&
	Text[Index] != '\n')
  {
    ++Index;
  }

  if(Text[Index] == '\0')
  {
    Buffer->AtEnd = true;
  }
  
  char *Line = GetSubStringCopy(Text, StartLocation, Index);
  Buffer->LocationInText = ++Index;
  
  return(Line);
}

enum
{
  BLANK = 0,
  COMMENT = 1,
  CODE = 2
};

internal s32
StringCompare(char *BaseString, char *CompareString)
{
  char *Base = BaseString;
  char *Compare = CompareString;
  
  while(*Base == *Compare &&
	*Base != '\0' &&
	*Compare != '\0')
  {
    ++Base;
    ++Compare;
  }

  s32 Result = *Base - *Compare;
  return(Result);
}

internal b32
StringCompare(char *BaseString, char *CompareString, u32 HowMany)
{
  char *Base = BaseString;
  char *Compare = CompareString;
  u32 Count = 0;
  
  while(*Base == *Compare &&
	*Base != '\0' &&
	*Compare != '\0')
  {
    ++Base;
    ++Compare;
    ++Count;
  }

  
  b32 Result = (Count == HowMany);
  return(Result);
}

internal s32
StringLength(char *String)
{
  s32 Result = 0;
  while(*String++ != '\0')
  {
    ++Result;
  }

  return(Result);
}

internal b32
SubString(char *BaseString, char *CheckString)
{
  b32 Result = false;
  
  char *Base = BaseString;
  char *Check = CheckString;

  s32 BaseLength = StringLength(Base);
  s32 CheckLength = StringLength(Check);

  for(s32 Outter = 0;
      Outter < (BaseLength - CheckLength) + 1;
      ++Outter)
  {
    u32 Inner = 0;
    while(Base[Outter + Inner] == Check[Inner])
    {
      ++Inner;
    }
    if(Inner >= CheckLength)
    {
      Result = true;
      break;
    }
  }

  return(Result);
}

internal b32
SubStringAtBeginning(char *BaseString, char *CheckString)
{
  b32 Result = true;
  
  char *Base = BaseString;
  char *Check = CheckString;

  s32 CheckLength = StringLength(Check);
  
  for(u32 Index = 0;
      Index < CheckLength;
      ++Index)
  {
    if(*Base++ != *Check++)
    {
      Result = false;
      break;
    }
  }

  return(Result);
}

// NOTE(faruk): There was a bug where when you don't close the multi line
// coment it will still count lines as comments.And that happens here
// if you notice, SubStringAtBeginning(Line, "/*") it has the multi line
// comment start and it counts all lines till end as comments.

// HAHAHA. And its fixed just by
// SubString(Line, "*/") && !SubString(Line, "\"*/\"")
// So just ingore comment syntax that is in a string "*/".
// Its beautifly lazy.

internal u32
ProcessLine(char *Line, b32 *InComment)
{
  u32 Result = BLANK;
  b32 IsComment = *InComment;

  // NOTE(faruk): Ignoring the whitespace at the begining.
  while(*Line == ' ' || *Line == '\t')
  {
    ++Line;
  }

  u32 LineLength = StringLength(Line);
  
  if(*Line != '\0')
  {
    if(SubString(Line, "*/") && !SubString(Line, "\"*/\""))
    {
      *InComment = false;
      IsComment = true;
    }
    else if(SubString(Line, "/*") && !SubString(Line, "\"/*\""))
    {
      *InComment = true;
      if(SubStringAtBeginning(Line, "/*")) 
      {
	IsComment = true;
      }
    }
    else if(SubStringAtBeginning(Line, "//"))
    {
      IsComment = true;
    }
  }

  if(LineLength > 0)
  {
    if(IsComment)
    {
      Result = COMMENT;
    }
    else
    {
      Result = CODE;
    }
  }
  
  return(Result);
}
  
int main(int ArgCount, char **ArgV)
{
  u32 GlobalCode = 0;
  u32 GlobalBlank = 0;
  u32 GlobalComment = 0;
  
  if(ArgCount > 1)
  {
    for(u32 Iterator = 1;
	Iterator < ArgCount;
	++Iterator)
    {
      char *FileName = ArgV[Iterator];
      FILE *File = fopen(FileName, "r");
      
      if(File)
      {
	// Getting the file size
	fseek(File, 0, SEEK_END);
	u32 FileSize = ftell(File);
	fseek(File, 0, SEEK_SET);

	char *Buffer = (char *)malloc(FileSize);
	ReadFileText(Buffer, File);

	text_buffer TextBuffer = {};
	TextBuffer.Text = Buffer;
	TextBuffer.Size = FileSize;

	u32 Code = 0;
	u32 Blank = 0;
	u32 Comment = 0;
	
	char *Line = GetLine(&TextBuffer);
	b32 InComment = false;
	  
	while(Line)
	{
	  u32 Switch = ProcessLine(Line, &InComment);
	  
	  switch(Switch)
	  {
	    case BLANK:
	    {
	      ++Blank;
	    }break;
	    case CODE:
	    {
	      ++Code;
	    }break;
	    case COMMENT:
	    {
	      ++Comment;
	    }break;
	  }

	  Line = GetLine(&TextBuffer);
	}

	// NOTE(faruk): Last line is blank.But we don't count that
	// line.
	--Blank;
	
	printf("%s   Blank: %d   Comment: %d   Code: %d\n",
	       FileName, Blank, Comment, Code);

	GlobalCode += Code;
	GlobalBlank += Blank;
	GlobalComment += Comment;
      }
      else
      {
	printf("Couldn't find the file: %s\n", FileName);
      }
    }

    printf("\nTotal:   Blank: %d   Comment: %d   Code: %d\n",
	   GlobalBlank, GlobalComment, GlobalCode);
  }
  else
  {
    printf("Usage: loc [filename] [filename] [filename] ... \n");
  }

  return(0);
}
