#if !defined(LOC_H)

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
#define NULL_STR "_nl_"

#include "loc_str.h"


struct prog_lang_config
{
  char Comment[8];
  char OpenComment[8];
  char CloseComment[8];

  char CommentInString[8];
  char OpenCommentInString[8];
  char CloseCommentInString[8];
};

struct file_type_config
{
  char Name[32];
  char Extensions[3][8];
  u32 ExtensionCount;
};

struct valid_input_entry
{
  file_type_config TypeConfig;
  prog_lang_config LangConfig;
};

struct bin_search_entry
{
  char Extension[8];
  u32 ValidEntryIndex;
};



struct text_buffer
{
  char *Text;
  u32 Size;
  u32 LocationInText;
  b32 AtEnd;
};


struct final_process_result
{
  u32 Comment;
  u32 Blank;
  u32 Code;
  //u32 Type;
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




// NOTE(faruk): There was a bug where when you don't close the multi line
// coment it will still count lines as comments.And that happens here
// if you notice, SubStringAtBeginning(Line, "/*") it has the multi line
// comment start and it counts all lines till end as comments.

// HAHAHA. And its fixed just by
// SubString(Line, "*/") && !SubString(Line, "\"*/\"")
// So just ingore comment syntax that is in a string "*/".
// Its beautifly lazy.
internal u32
ProcessLine(char *Line, b32 *InComment, valid_input_entry *Entry)
{
  u32 Result = BLANK;
  b32 IsComment = *InComment;

  // NOTE(faruk): Ignoring the whitespace at the begining.
  while(*Line == ' ' || *Line == '\t')
  {
    ++Line;
  }

  u32 LineLength = StringLength(Line);
  prog_lang_config *Conf = &Entry->LangConfig;
  
  if(*Line != '\0')
  {
    if(SubString(Line, Conf->CloseComment) &&
       !SubString(Line, Conf->CloseCommentInString))
    {
      *InComment = false;
      IsComment = true;
    }
    else if(SubString(Line, Conf->OpenComment) &&
	    !SubString(Line, Conf->OpenCommentInString))
    {
      *InComment = true;
      if(SubStringAtBeginning(Line, Conf->OpenComment)) 
      {
	IsComment = true;
      }
    }
    else if(SubStringAtBeginning(Line, Conf->Comment))
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

  // NOTE(faruk): For some reason there are carage returns '\r' at the and
  // but its not just charage returns its also new lines so an end of
  // a line looks like this \r\n. And when we read a blank line we still
  // read the \r meaning that its not blank and this is the fix for that.
  if(LineLength == 1 && Line[0] == '\r')
  {
    Result = BLANK;
  }
  
  return(Result);
}

internal final_process_result
ProcessTextBuffer(text_buffer *Buffer, valid_input_entry *Entry)
{
  final_process_result Result = {};
  
  char *Line = GetLine(Buffer);
  b32 InComment = false;
	  
  while(Line)
  {
    u32 Switch = ProcessLine(Line, &InComment, Entry);
	  
    switch(Switch)
    {
      case BLANK:
      {
	++Result.Blank;
      }break;
      case CODE:
      {
	++Result.Code;
      }break;
      case COMMENT:
      {
	++Result.Comment;
      }break;
    }

    Line = GetLine(Buffer);
  }

  // NOTE(faruk): Last line is blank.But we don't count that
  // line.
  --Result.Blank;

  return(Result);
}

#define LOC_H
#endif
