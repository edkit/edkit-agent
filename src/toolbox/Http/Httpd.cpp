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
* @date     2011/12/12
* 
*****************************************************************************/
#include "Httpd.h"

/**
* @date     2011/12/12
* 
*  Constructor. 
*
******************************************************************************/
Httpd::Httpd(void):
   HandlerList()
{
   return;
}


/**
* @date     2011/12/12
* 
*  Destructor. 
*
******************************************************************************/
Httpd::~Httpd(void)
{
   return;
}


/**
* @date     2011/12/21
* 
*  Adds a new url handler. The handler must not be deleted as long as it has not
*  been removed from the handler list.
*
******************************************************************************/
int32_t Httpd::AddUrlHandler(const UrlHandler &Handler)
{
   HandlerList.AddItem(&Handler);
   return(0);
}


/**
* @date     2011/12/21
* 
*  Removes an url handler. The handler can then be deleted.
*
******************************************************************************/
int32_t Httpd::DelUrlHandler(const UrlHandler &Handler)
{
   HandlerList.DelItem(&Handler);
   return(0);
}
