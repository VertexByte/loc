#if !defined(LOC_H)

typedef int s32;
typedef unsigned int u32;
typedef char s8;
typedef unsigned int u8;
typedef short s16;
typedef unsigned short u16;
typedef long s64;
typedef unsigned long u64;

typedef float r32;
typedef double r64;

typedef s32 b32;

#define internal static
#define local_persist static
#define global_variable static

#define Assert(Expression) if(!(Expression)){ *(u32 *)0  = 0; }
#define NULL_STR "_nl_" 

#include "loc_str.h"


//
//  Type and Lang config stuff
//
struct prog_lang_config
{
  char Comment[8];
  u32 CommentLength;
  
  char OpenComment[8];
  u32 OpenCommentLength;
  
  char CloseComment[8];
  u32 CloseCommentLength;

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

  u32 FileCount;
  u32 Code;
  u32 Comment;
  u32 Blank;
};

struct extension_search_entry
{
  char Extension[8];
  u32 ValidEntryIndex;
};

struct entries_state
{
  valid_input_entry ValidEntries[50];
  u32 ValidEntriesCount;
  
  extension_search_entry BinEntries[100];
  u32 BinEntriesCount;

  void *FileReadBuffer;
  u32 FileReadBufferSize;
};

internal prog_lang_config
MakeProgLangConfig(char *Comment, char *OpenComment, char *CloseComment)
{
  prog_lang_config Result = {};
  CopyString(Result.Comment, Comment);
  Result.CommentLength = StringLength(Comment);
  
  CopyString(Result.OpenComment, OpenComment);
  Result.OpenCommentLength = StringLength(OpenComment);
  
  CopyString(Result.CloseComment, CloseComment);
  Result.CloseCommentLength = StringLength(CloseComment);
  
  // NOTE(faruk): This is for when we have a case like:
  // printf("//");
  
  CopyString(Result.CommentInString, "\"");
  ConcatString(Result.CommentInString, Comment);
  ConcatString(Result.CommentInString, "\"");

  CopyString(Result.OpenCommentInString, "\"");
  ConcatString(Result.OpenCommentInString, OpenComment);
  ConcatString(Result.OpenCommentInString, "\"");

  CopyString(Result.CloseCommentInString, "\"");
  ConcatString(Result.CloseCommentInString, CloseComment);
  ConcatString(Result.CloseCommentInString, "\"");
  
  return(Result);
}

internal file_type_config
MakeFileTypeConfig(char *Name, char *E0, char *E1, char *E2,
		   u32 ExtensionCount)
{
  file_type_config Result = {};
  CopyString(Result.Name, Name);

  CopyString(Result.Extensions[0], E0);
  CopyString(Result.Extensions[1], E1);
  CopyString(Result.Extensions[2], E2);

  Result.ExtensionCount = ExtensionCount;

  return(Result);
}

internal void
PushBinEntry(entries_state *State, char *Extension, u32 Index)
{
  u32 PushIndex = State->BinEntriesCount;
  extension_search_entry *Entry = &State->BinEntries[PushIndex];

  CopyString(Entry->Extension, Extension);
  Entry->ValidEntryIndex = Index;

  State->BinEntriesCount = PushIndex + 1;
}

internal void
PushEntry(entries_state *State,
	  file_type_config TypeConfig, prog_lang_config LangConfig)
{
  u32 PushIndex = State->ValidEntriesCount;
  valid_input_entry *Entry = &State->ValidEntries[PushIndex];

  Entry->TypeConfig = TypeConfig;
  Entry->LangConfig = LangConfig;

  for(u32 Index = 0;
      Index < Entry->TypeConfig.ExtensionCount;
      ++Index)
  {
    PushBinEntry(State, Entry->TypeConfig.Extensions[Index], PushIndex);
  }
  
  State->ValidEntriesCount = PushIndex + 1;
}

internal void
InitializeValidEntries(entries_state *State)
{
  
  // NOTE(faruk): No sorting is needed if the entries are already
  // sorted.
  file_type_config Type = {};
  prog_lang_config Lang = {};
  
  // A
  
  // B
  Type = MakeFileTypeConfig("Batch", "bat", "cmd", "btm", 3);
  Lang = MakeProgLangConfig("rem", "REM", "REM");
  PushEntry(State, Type, Lang);
  
  // C
  Type = MakeFileTypeConfig("C++", "cpp", NULL_STR, NULL_STR, 1);
  Lang = MakeProgLangConfig("//", "/*", "*/");
  PushEntry(State, Type, Lang);

  Type = MakeFileTypeConfig("C", "c", NULL_STR, NULL_STR, 1);
  Lang = MakeProgLangConfig(NULL_STR, "/*", "*/");
  PushEntry(State, Type, Lang);
  
  Type = MakeFileTypeConfig("C#", "cs", NULL_STR, NULL_STR, 1);
  Lang = MakeProgLangConfig("//", "/*", "*/");
  PushEntry(State, Type, Lang);

  Type = MakeFileTypeConfig("Css", "css", NULL_STR, NULL_STR, 1);
  Lang = MakeProgLangConfig("//", "/*", "*/");
  PushEntry(State, Type, Lang);
  
  // D
  
  // E
  
  // F
  
  // G
  Type = MakeFileTypeConfig("Go", "go", NULL_STR, NULL_STR, 1);
  Lang = MakeProgLangConfig("//", "/*", "*/");
  PushEntry(State, Type, Lang);
  
  // H
  Type = MakeFileTypeConfig("C/C++ Header", "hpp", "h", NULL_STR, 2);
  Lang = MakeProgLangConfig("//", "/*", "*/");
  PushEntry(State, Type, Lang);

  Type = MakeFileTypeConfig("HTML", "html", NULL_STR, NULL_STR, 1);
  Lang = MakeProgLangConfig(NULL_STR, "<!--", "-->");
  PushEntry(State, Type, Lang);
  
  // I
  
  // J
  Type = MakeFileTypeConfig("JavaScript", "js", NULL_STR, NULL_STR, 1);
  Lang = MakeProgLangConfig("//", "/*", "*/");
  PushEntry(State, Type, Lang);

  Type = MakeFileTypeConfig("Java", "java", NULL_STR, NULL_STR, 1);
  Lang = MakeProgLangConfig("//", "/*", "*/");
  PushEntry(State, Type, Lang);
  
  // K
  
  // L
  
  // M
  
  // N
  
  // O
  
  // P
  Type = MakeFileTypeConfig("Python", "py", "pyc", "pyo", 3);
  Lang = MakeProgLangConfig("#", NULL_STR, NULL_STR);
  PushEntry(State, Type, Lang);

  Type = MakeFileTypeConfig("PHP", "php", NULL_STR, NULL_STR, 1);
  Lang = MakeProgLangConfig("//", "/*", "*/");
  PushEntry(State, Type, Lang);
  
  
  // Q
  
  // R
  
  // S
  
  // T
  
  // U
  
  // V
  
  // W
  
  // X
  
  // Y
  
  // Z

}

//
//  Extension Check
//
struct extension_check_result
{
  b32 Found;
  valid_input_entry *Entry;
};

internal extension_check_result
CheckExtension(entries_state *State, char *Extension)
{
  extension_check_result Result = {};
  
  extension_search_entry *BinEntries = State->BinEntries;
  u32 BinEntriesCount = State->BinEntriesCount;

  valid_input_entry *ValidEntries = State->ValidEntries;
  u32 ValidEntriesCount = State->ValidEntriesCount;
  
  for(u32 Index = 0;
      Index < BinEntriesCount;
      ++Index)
  {
    char *CheckExtension = BinEntries[Index].Extension;
    if(StringCompare(Extension, CheckExtension) == 0)
    {
      u32 EntryIndex = BinEntries[Index].ValidEntryIndex;
      
      Result.Entry = &ValidEntries[EntryIndex];
      Result.Found = true;
      break;
    }
  }

  return(Result);
}

//
//  Text buffer processing stuff
//
struct text_buffer
{
  char *Text;
  u32 Size;
  u32 LocationInText;
  b32 AtEnd;
  char ProcessLine[64000];
};


struct lines_process_result
{
  u32 Comment;
  u32 Blank;
  u32 Code;
  //u32 Type;
};

struct total_result
{
  lines_process_result Count;
  u32 WeirdoFiles;
  u32 RecognizedFiles;
};

internal char *
GetLine(text_buffer *Buffer)
{
  char *Line = 0;
  
  if(Buffer->AtEnd)
  {
    return(0);
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

  // NOTE(faruk): Now we just reuse the same memory(stack memory) all
  // the time and return the line's address as the result, and if we
  // are at the end of the buffer we just return 0.
  CopyMemoryChar(Text, Buffer->ProcessLine, StartLocation, Index);
  Buffer->LocationInText = ++Index;

  Line = Buffer->ProcessLine;
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

  if(*Line != '\0' && LineLength > 2)
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

  // NOTE(faruk): For some reason there are carage returns '\r' at the and
  // but its not just charage returns its also new lines so an end of
  // a line looks like this \r\n. And when we read a blank line we still
  // read the \r meaning that its not blank and we need to check
  // LineLength > 1
  if(LineLength > 1)
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


// NOTE(faruk): This way of processing is lines better that the
// one above.Idk if its really faster but the first implementation
// has a bug and thats if we have something like this:
//
//  Example:
//    int a = 32; /* Counts this line as a comment */
//
// And that line is counted as a comment and it should be counted as a
// code line, so in this newer implementation that bug is fixed.
internal u32
ProcessLineBetter(char *Line, b32 *InComment, valid_input_entry *Entry)
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

  u32 LineCursor = 0;

  b32 CodeFirst = false;

  // NOTE(faruk): If line langht isnt blank LineLength it will be
  // 2 charecters long because for some reason we read a
  // '\r' (charage return)
  if(LineLength > 1)
  {
    while(Line[LineCursor] != '\r' &&
	  Line[LineCursor] != '\0')
    {
      char *LineLoc = Line + LineCursor;
      
      if(!(*InComment) && StringCompare(LineLoc, Conf->OpenComment,
					Conf->OpenCommentLength))
      {
	if(LineCursor > 0)
	{
	  Result = CODE;
	  CodeFirst = true;
	  if(*(LineLoc - 1) != '\"')
	  {
	    *InComment = true;
	  }
	}
	else
	{
	  *InComment = true;
	  Result = COMMENT;
	}

	LineCursor += Conf->OpenCommentLength - 1;
      }
      else if(*InComment && StringCompare(LineLoc, Conf->CloseComment,
					  Conf->CloseCommentLength))
      {
	*InComment = false;
	Result = COMMENT;
	if(CodeFirst)
	{
	  Result = CODE;
	}
	else
	{
	  Result = COMMENT;
	}
      
	LineCursor += Conf->CloseCommentLength - 1;
      }
      else if(LineCursor == 0 && StringCompare(LineLoc, Conf->Comment,
					       Conf->CommentLength))
      {
	LineCursor += Conf->CommentLength - 1;
	Result = COMMENT;
	break;
      }
      else
      {
	if(*InComment)
	{
	  Result = COMMENT;
	}
	else
	{
	  Result = CODE;
	}
      }
    
      ++LineCursor;
    }
  }
  
  return(Result);
}

internal lines_process_result
ProcessTextBuffer(text_buffer *Buffer, valid_input_entry *Entry)
{
  lines_process_result Result = {};

  char *Line = GetLine(Buffer);
  b32 InComment = false;
	  
  while(Line)
  {
    u32 Switch = ProcessLineBetter(Line, &InComment, Entry);
	  
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
