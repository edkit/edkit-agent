/*
*****************************************************************************
*                      ___       _   _    _ _
*                     / _ \ __ _| |_| |__(_) |_ ___
*                    | (_) / _` | / / '_ \ |  _(_-<
*                     \___/\__,_|_\_\_.__/_|\__/__/
*                          Copyright (c) 2011
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
*****************************************************************************/
/**
* @author   R. Picard
* @date     2011/05/08
*
*****************************************************************************/
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <new>

#include "FileWriter.h"
#include "ContextUtils.h"


/**
* @date     2011/05/08
*
*  Constructor.
*  The filename and poll period (in second) is retrieved from environment
*  variables. If the Poll period is 0, then the filewriter is disabled.
*
******************************************************************************/
FileWriter::FileWriter(void): edkit::Thread(),
   StopRequested(false), FileName(), PollPeriod(FW_DEFAULT_POLL_PERIOD)
{
   char *sz_FileName;
   sz_FileName = getenv(ENV_FILENAME);
   if(sz_FileName == NULL)
      FileName << FW_DEFAULT_FILENAME << "." << getpid() ;
   else
      FileName << sz_FileName << "." << getpid();

   char *sz_PollPeriod = getenv(ENV_POLL_PERIOD);
   if(sz_PollPeriod != NULL)
   {
      PollPeriod = strtoul(sz_PollPeriod, NULL,10);
      if(PollPeriod == ULONG_MAX)
         PollPeriod = FW_DEFAULT_POLL_PERIOD;
   }

   if(PollPeriod > 0)
      Start();
   return;
}


/**
* @date     2011/05/08
*
*  Constructor.
*
******************************************************************************/
FileWriter::~FileWriter(void)
{
   if(IsStarted() == true)
      Stop();
   return;
}


/**
* @date     2011/05/08
*
*  Constructor.
*
******************************************************************************/
int32_t FileWriter::Start(void)
{
   return(Thread::Start());
}


/**
* @date     2011/05/08
*
*  Constructor.
*
******************************************************************************/
int32_t FileWriter::Stop(void)
{
   StopRequested = true;
   WaitForEnd();
   return(0);
}


/**
* @date     2011/05/08
*
*  Loop entry of the thread. The ExeContext list is flushed as a json object.
*
******************************************************************************/
void FileWriter::Loop(void)
{
   int   i_File = -1;
   bool  b_FirstSlice;
   String JsonSlice, JsonAllocers;
   String Json;
   off_t  i_Offset = 0;

   Json << "{\n";
   Json << "\"slice\": [\n";
   b_FirstSlice = true;
   while(1)
   {
      Sleep();
      if(StopRequested == true)
         break;

      if(CU_GetSlice(&JsonSlice, &JsonAllocers) == 0)
      {
         if(b_FirstSlice == false)
         {
            lseek(i_File, i_Offset, SEEK_SET);
            Json.SetTo(",\n");
            write(i_File, Json.GetString(), Json.GetSize());
            i_Offset += Json.GetSize();
         }
         else
         {
            i_File = open(FileName.GetString(), O_RDWR|O_CREAT|O_TRUNC, S_IRWXU|S_IRGRP|S_IROTH);
            if(i_File == -1)
               break;

            write(i_File, Json.GetString(), Json.GetSize());
            i_Offset += Json.GetSize();
            b_FirstSlice = false;
         }
         write(i_File, JsonSlice.GetString(), JsonSlice.GetSize());
         i_Offset += JsonSlice.GetSize();
         Json.SetTo("\n],\n");
         write(i_File, Json.GetString(), Json.GetSize());
         write(i_File, JsonAllocers.GetString(), JsonAllocers.GetSize());
         Json.SetTo("\n}");
         write(i_File, Json.GetString(), Json.GetSize());
      }
   }

   if(i_File != -1)
      close(i_File);
}


/**
* @date     2011/05/08
*
*  Sleep for PollPeriod. The sleep is done by 100ms steps to be able to
*  interrupt the long wait if we must quit.
*
* @todo Find a better way than pollin to interrupt the sleep.
*
******************************************************************************/
void FileWriter::Sleep(void)
{
   uint32_t i;
   uint32_t i_LoopCount = PollPeriod * 1000 / LOOP_STEP_MS_TIME;
   struct timespec   s_WaitTime, s_RemainTime;

   s_WaitTime.tv_sec = 0;
   s_WaitTime.tv_nsec = LOOP_STEP_MS_TIME*1000*1000;

   for(i=0; i<i_LoopCount; i++)
   {
      if(StopRequested == true)
         break;

      if(nanosleep(&s_WaitTime, &s_RemainTime) == -1)
      {
         while( (StopRequested == false) &&
                (errno == EINTR) &&
                (nanosleep(&s_RemainTime, &s_RemainTime) == -1)
              );
      }
   }
   return;
}

