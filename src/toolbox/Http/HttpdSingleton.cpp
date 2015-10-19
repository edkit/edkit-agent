/*
*****************************************************************************
*                      ___       _   _    _ _
*                     / _ \ __ _| |_| |__(_) |_ ___
*                    | (_) / _` | / / '_ \ |  _(_-<
*                     \___/\__,_|_\_\_.__/_|\__/__/
*                          Copyright (c) 2012
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
* @date     2012/01/24
*
*****************************************************************************/
#include <new>
#include <stdlib.h>
#include <errno.h>
#include "HttpdSingleton.h"


/**
* @date     2011/12/12
*
*  Constructor.
*
******************************************************************************/
HttpdSingleton::HttpdSingleton(void):
   Httpd()
{
   return;
}


/**
* @date     2011/12/12
*
*  Destructor.
*
******************************************************************************/
HttpdSingleton::~HttpdSingleton(void)
{
   return;
}


/**
* @date     2011/12/12
*
*  Destructor.
*
******************************************************************************/
HttpdSingleton* HttpdSingleton::Instantiate(void)
{
   static HttpdSingleton *Server = NULL;

   if(Server == NULL)
   {
      Server  = new(std::nothrow) HttpdSingleton();
      if(Server != NULL)
      {
         int32_t i_Ret = 0;
         uint16_t Port = 8080;
         char *sz_Port = getenv(ENV_PORT);
         if(sz_Port != NULL)
         {
            errno = 0;
            char *EndPtr;
            Port = strtol(sz_Port, &EndPtr, 10);
            if( (errno != 0) || (*EndPtr != '\0') )
               i_Ret = -1;
         }
         if(i_Ret == 0)
         {
            i_Ret = Server->Start(Port);
         }
         if(i_Ret != 0)
         {
            delete Server;
            Server = NULL;
         }
      }
   }
   return(Server);
}


