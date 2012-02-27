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
* @date     2012/01/08
*
*****************************************************************************/
#include <new>
#include <stdlib.h>
#include <string.h>
#include "HttpdRequest.h"
#include "ListIterator.h"

/**
* @date     2012/01/08
*
*  Constructor.
*
******************************************************************************/
HttpdRequest::HttpdRequest(void):
   HttpMethod(NONE), Path(),
   Payload(NULL), PayloadLength(0),
   HeaderList(), Status(200)
{
   return;
}


/**
* @date     2012/01/08
*
*  Destructor.
*
******************************************************************************/
HttpdRequest::~HttpdRequest(void)
{
   Header  *p_CurHeader;

   while(HeaderList.GetItemAt(0, &p_CurHeader) == 0)
   {
      delete p_CurHeader;
      HeaderList.DelItemAt(0);
   }

   if(Payload != NULL)
   {
      free(Payload);
   }

   return;
}


/**
* @date     2012/01/08
*
*  Sets the request url path.
*
******************************************************************************/
int32_t HttpdRequest::SetPath(const String &NewPath)
{
   return(Path.SetTo(NewPath));
}


/**
* @date     2012/01/09
*
*  Sets the request payload.
*
******************************************************************************/
int32_t HttpdRequest::SetPayload(const uint8_t *NewPayload, uint32_t i_Length)
{
   if(NewPayload == NULL)
      return(-1);

   if(Payload != NULL)
   {
      free(Payload);
      PayloadLength = 0;
   }

   Payload = (uint8_t*)malloc(i_Length);
   if(Payload != NULL)
   {
      memcpy(Payload, NewPayload, i_Length);
      PayloadLength = i_Length;
   }

   if(Payload == NULL)
   {
      return(-1);
   }
   return(0);
}


/**
* @date     2012/01/09
*
*  Sets the request payload.
*
******************************************************************************/
int32_t  HttpdRequest::GetPayload(const uint8_t **ReqPayload, uint32_t *i_Length) const
{
   if( (ReqPayload == NULL) || (i_Length == NULL) || (Payload == NULL))
      return(-1);

   *ReqPayload = Payload;
   *i_Length = PayloadLength;
   return(0);
}


/**
* @date     2012/02/09
*
*  Sets the provided header value. If the header is already present, then it is
*  replaced.
*
******************************************************************************/
int32_t  HttpdRequest::SetHeader(const String &HeaderName, const String &HeaderValue)
{
   int32_t i_Ret;
   ListIterator<AList<Header*>, Header*> Iterator;
   Header  *p_CurHeader;

   /* If header is already present, then just update its value */
   Iterator.SetTo(&HeaderList);
   Iterator.First();
   while(Iterator.IsDone() == false)
   {
      i_Ret = Iterator.GetItem(&p_CurHeader);
      if( (i_Ret == 0) && (p_CurHeader != NULL) )
      {
         if(p_CurHeader->Name == HeaderName)
         {
            p_CurHeader->Value.SetTo(HeaderValue);
            return(0);
         }
      }
      Iterator.Next();
   }

   /* New header, add it */
   p_CurHeader = new(std::nothrow) Header();
   if(p_CurHeader != NULL)
   {
      p_CurHeader->Name.SetTo(HeaderName);
      p_CurHeader->Value.SetTo(HeaderValue);
      return(HeaderList.AddItem(p_CurHeader));
   }
   return(-1);
}


/**
* @date     2012/02/09
*
*  Gets the provided header value.
*
* @return 0 if success.
* @return Error otherwise.
******************************************************************************/
int32_t  HttpdRequest::GetHeader(const String &HeaderName, String *HeaderValue) const
{
   int32_t i_Ret;
   ListIterator<AList<Header*>, Header*> Iterator;
   Header  *p_CurHeader;

   if(HeaderValue == NULL)
      return(-1);

   Iterator.SetTo(&HeaderList);
   Iterator.First();
   while(Iterator.IsDone() == false)
   {
      i_Ret = Iterator.GetItem(&p_CurHeader);
      if( (i_Ret == 0) && (p_CurHeader != NULL) )
      {
         if(p_CurHeader->Name == HeaderName)
         {
            HeaderValue->SetTo(p_CurHeader->Value);
            return(0);
         }
      }
      Iterator.Next();
   }

   return(-1);
}

