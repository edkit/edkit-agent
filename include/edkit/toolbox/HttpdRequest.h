#ifndef HTTPDREQUEST_H
#define HTTPDREQUEST_H
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
#include "OakString.h"
#include "AList.h"

class HttpdRequest
{
   public:
      enum Method
      {
         DELETE,
         GET,
         HEAD,
         OPTIONS,
         POST,
         PUT,
         NONE
      };

      struct  Header
      {
         String Name;
         String Value;
      };

                                       HttpdRequest();
      virtual                          ~HttpdRequest(void);

               int32_t                 SetPath(const String &NewPath);
               const String&           GetPath(void) const { return(Path);};

               void                    SetStatus(const uint32_t i_Status) {Status=i_Status;};
               uint32_t                GetStatus(void) const { return(Status);};

               void                    SetMethod(Method NewMethod) {HttpMethod = NewMethod;};
               Method                  GetMethod(void) const { return(HttpMethod);};

               int32_t                 SetPayload(const uint8_t *NewPayload, uint32_t i_Length);
               int32_t                 GetPayload(const uint8_t **ReqPayload, uint32_t *i_Length) const;

               int32_t                 SetHeader(const String &HeaderName, const String &HeaderValue);
               int32_t                 GetHeader(const String &HeaderName, String *HeaderValue) const;
               const AList<Header*>&   GetHeaderList(void) const { return(HeaderList);};

   private:
               Method         HttpMethod;
               String         Path;
               uint8_t*       Payload;
               uint32_t       PayloadLength;
               AList<Header*> HeaderList;
               uint32_t       Status;
};

#endif
