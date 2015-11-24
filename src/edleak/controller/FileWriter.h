#ifndef __FILEWRITER_H
#define __FILEWRITER_H

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
#include "Thread.h"
#include "OakString.h"

class FileWriter : edkit::Thread
{
   public:
#define ENV_FILENAME    "edkit_fw_file"
#define ENV_POLL_PERIOD "edkit_fw_period"


                        FileWriter(void);
      virtual           ~FileWriter(void);


   protected:
      virtual  void     Loop(void);

   private:
#define  LOOP_STEP_MS_TIME 100

               int32_t  Start(void);
               int32_t  Stop(void);
               void     Sleep(void);

               bool     StopRequested;
               String   FileName;
               uint32_t PollPeriod; ///< Sleep time in seconds;
};

#endif
