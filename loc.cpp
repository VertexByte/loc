#include <stdio.h>
#include <stdlib.h>

#include "loc.h"
#include "win32_loc.h"

/* TODO:
   - Code clean up.
   - Sorting the file types in lexicographical order.
   - Binary search on input when processing.
 */



internal prog_lang_config
MakeProgLangConfig(char *Comment, char *OpenComment, char *CloseComment)
{
  prog_lang_config Result = {};
  CopyString(Result.Comment, Comment);
  CopyString(Result.OpenComment, OpenComment);
  CopyString(Result.CloseComment, CloseComment);

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


// TODO(faruk): Global!!!
global_variable valid_input_entry ValidEntries[100];
u32 ValidEntriesCount = 0;

global_variable bin_search_entry BinEntries[300];
u32 BinEntriesCount = 0;

internal void
PushBinEntry(char *Extension, u32 Index)
{
  bin_search_entry *Entry = &BinEntries[BinEntriesCount];
  CopyString(Entry->Extension, Extension);
  Entry->ValidEntryIndex = Index;

  ++BinEntriesCount;
}

internal void
PushEntry(file_type_config TypeConfig, prog_lang_config LangConfig)
{
  u32 PushIndex = ValidEntriesCount;
  valid_input_entry *Entry = &ValidEntries[PushIndex];

  Entry->TypeConfig = TypeConfig;
  Entry->LangConfig = LangConfig;

  for(u32 Index = 0;
      Index < Entry->TypeConfig.ExtensionCount;
      ++Index)
  {
    PushBinEntry(Entry->TypeConfig.Extensions[Index], PushIndex);
  }
  
  ++ValidEntriesCount;
}

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

    char Extension[64] = {};
    GetFileExtension(Path, Extension);
    b32 FoundExtension = false;

    valid_input_entry *Entry = NULL;
    
    for(u32 Index = 0;
	Index < BinEntriesCount;
	++Index)
    {
      char *CheckExtension = BinEntries[Index].Extension;
      if(StringCompare(Extension, CheckExtension) == 0)
      {
	FoundExtension = true;
	u32 EntryIndex = BinEntries[Index].ValidEntryIndex;
	Entry = &ValidEntries[EntryIndex];
	break;
      }
    }
    
    if(FileReadResult.Size > 0 && FoundExtension)
    {
      text_buffer Buffer = {};
      Buffer.Text = (char *)FileReadResult.Memory;
      Buffer.Size = FileReadResult.Size;
      
      final_process_result Result = ProcessTextBuffer(&Buffer, Entry);
      
      /*printf("%s [code: %d, blank: %d, comment:%d]\n",
	Extension, Result.Code, Result.Blank, Result.Comment);*/
      
      GlobalResult->Code += Result.Code;
      GlobalResult->Blank += Result.Blank;
      GlobalResult->Comment += Result.Comment;
    }
    else
    {
      //printf("Couldn't open %s\n", Path);
    }
  }
}



int main(int ArgCount, char **ArgV)
{
  final_process_result GlobalResult = {};

  // NOTE(faruk): No sorting is needed if the entries are already
  // sorted.
  file_type_config Type = {};
  prog_lang_config Lang = {};
  
  // A
  
  // B
  Type = MakeFileTypeConfig("Batch", "bat", "cmd", "btm", 3);
  Lang = MakeProgLangConfig("rem", "REM", "REM");
  PushEntry(Type, Lang);
  
  // C
  Type = MakeFileTypeConfig("C++", "cpp", NULL_STR, NULL_STR, 1);
  Lang = MakeProgLangConfig("//", "/*", "*/");
  PushEntry(Type, Lang);

  Type = MakeFileTypeConfig("C", "c", NULL_STR, NULL_STR, 1);
  Lang = MakeProgLangConfig(NULL_STR, "/*", "*/");
  PushEntry(Type, Lang);
  
  Type = MakeFileTypeConfig("C#", "cs", NULL_STR, NULL_STR, 1);
  Lang = MakeProgLangConfig("//", "/*", "*/");
  PushEntry(Type, Lang);

  Type = MakeFileTypeConfig("Css", "css", NULL_STR, NULL_STR, 1);
  Lang = MakeProgLangConfig("//", "/*", "*/");
  PushEntry(Type, Lang);
  
  // D
  
  // E
  
  // F
  
  // G
  Type = MakeFileTypeConfig("Go", "go", NULL_STR, NULL_STR, 1);
  Lang = MakeProgLangConfig("//", "/*", "*/");
  PushEntry(Type, Lang);
  
  // H
  Type = MakeFileTypeConfig("C/C++ Header", "hpp", "h", NULL_STR, 2);
  Lang = MakeProgLangConfig("//", "/*", "*/");
  PushEntry(Type, Lang);

  Type = MakeFileTypeConfig("HTML", "html", NULL_STR, NULL_STR, 1);
  Lang = MakeProgLangConfig(NULL_STR, "<!--", "-->");
  PushEntry(Type, Lang);
  
  // I
  
  // J
  Type = MakeFileTypeConfig("JavaScript", "js", NULL_STR, NULL_STR, 1);
  Lang = MakeProgLangConfig("//", "/*", "*/");
  PushEntry(Type, Lang);

  Type = MakeFileTypeConfig("Java", "java", NULL_STR, NULL_STR, 1);
  Lang = MakeProgLangConfig("//", "/*", "*/");
  PushEntry(Type, Lang);
  
  // K
  
  // L
  
  // M
  
  // N
  
  // O
  
  // P
  Type = MakeFileTypeConfig("Python", "py", "pyc", "pyo", 3);
  Lang = MakeProgLangConfig("#", NULL_STR, NULL_STR);
  PushEntry(Type, Lang);

  Type = MakeFileTypeConfig("PHP", "php", NULL_STR, NULL_STR, 1);
  Lang = MakeProgLangConfig("//", "/*", "*/");
  PushEntry(Type, Lang);
  
  
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
