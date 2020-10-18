internal void
Win32ReadFile(char *FileName)
{
  HANDLE FileHandle = CreateFileA(FileName, GENERIC_READ, 0, 0,
				  OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,
				  0);

  if(FileHandle != INVALID_HANDLE_VALUE)
  {
    LARGE_INTEGER LargeSize;
    if(GetFileSizeEx(FileHandle, &LargeSize))
    {
      u64 Size = LargeSize.QuadPart;
      if(Size < 0xFFFFFFFF)
      {
	u32 Size32 = (u32)Size;
	void *Memory = VirtualAlloc(0, Size32, MEM_COMMIT | MEM_RESERVE,
				    PAGE_READWRITE);

	if(Memory)
	{
	  u32 BytesRead = 0;
	    
	  if(ReadFile(FileHandle, Memory, Size32, &BytesRead, 0))
	  {
	    Assert(BytesRead == Size32);
	  }
	  else
	  {
	    // TODO(faruk) Logging.
	    Assert(!"Yo");
	  }
	}
	else
	{
	  // TODO(faruk): Logging.
	  Assert(!"yo");
	}
	  
      }
      else
      {
	// TODO(faruk): File is too big.
	Assert(!"yo");
      }
    }
    else
    {
      // TODO(faruk): Logging???
      Assert(!"yo");
    }
      
  }
  else
  {
    //TODO(faruk): Logging???
    Assert(!"yo");
  }
}

