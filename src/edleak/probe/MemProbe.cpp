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
* @date     2011/05/01
*
*****************************************************************************/
#include "MemProbe.h"


/**
* @date     2011/05/01
*
*  Constructor.
*
******************************************************************************/
MemProbe::MemProbe(void):
   Probe()
{
   return;
}


/**
* @date     2011/05/01
*
*  Destructor.
*
******************************************************************************/
MemProbe::~MemProbe()
{
   return;
}


/**
* @date     2012/11/18
*
*  Returns the required padding size to ensure i_Boundary alignment when an
*  object of i_ObjectSize is added before an address align with i_Boundary.
*
* @param    i_Boundary (in): Alignment size to respect.
* @param    i_ObjectSize (in): Object Size.
* @return   Required padding.to keep i_Boundary alignment.
******************************************************************************/
uint32_t MemProbe::GetAlignmentPadding(size_t i_Boundary, size_t i_ObjectSize)
{
   uint32_t Padding = 0;

   if(i_Boundary != 0)
   {
      if(i_Boundary > i_ObjectSize)
         Padding = i_Boundary - i_ObjectSize;
      else
         Padding = i_ObjectSize % i_Boundary;
   }

   return(Padding);
}
