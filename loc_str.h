#if !defined(LOC_STR_H)

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

internal void
ConcatString(char *Base, char *Append)
{
  while(*Base++ != '\0'){}
  --Base;
  
  while(*Append != '\0')
  {
    *Base++ = *Append++;
  }

  *Base = '\0';
}

internal void
CopyString(char *Dest, char *Source)
{
  while(*Source != '\0')
  {
    *Dest++ = *Source++;
  }
  
  *Dest = '\0';
}

internal void
GetFileExtension(char *Path, char *Extension)
{
  b32 IsFileName = false;
  while(*Path != '\0')
  {
    if(IsFileName)
    {
      *Extension++ = *Path;
    }
    else if(*Path == '.')
    {
      IsFileName = true;
    }

    ++Path;
  }

  *Extension = '\0';
}

internal void
CopyMemoryChar(char *Source, char *Dest, u32 Start, u32 End)
{
  // NOTE(faruk): +1 is for the string terminator('\0').
  u32 Size = (End - Start) + 1;
  
  for(u32 Index = Start;
      Index < End;
      ++Index)
  {
    *Dest++ = Source[Index];
  }

  *Dest = '\0';
}

#define LOC_STR_H
#endif
