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
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#include "ListIterator.h"
#include "UrlHandler.h"
#include "Httpd.h"
#include "HttpdRequest.h"
#include "mongoose/mongoose.h"


#define MG_READ_ALLOC_SIZE    1024

static void* MgEventHandler(enum mg_event Event,
                     struct mg_connection *Handle,
                     const struct mg_request_info *RequestInfo);
static int32_t Mg2Req(struct mg_connection *Handle,
               const struct mg_request_info *RequestInfo,
               HttpdRequest   *Request);
static int32_t Req2Mg(struct mg_connection *Handle,
               const HttpdRequest   &Answer);

int32_t Mg2Req(struct mg_connection *Handle,
               const struct mg_request_info *RequestInfo,
               HttpdRequest   *Request)
{
   if( (Handle == NULL) || (RequestInfo == NULL) || (Request == NULL) )
      return(-1);

   // get method
   if(strcmp(RequestInfo->request_method, "GET") == 0)
      Request->SetMethod(HttpdRequest::GET);
   else if(strcmp(RequestInfo->request_method, "HEAD") == 0)
      Request->SetMethod(HttpdRequest::HEAD);
   else if(strcmp(RequestInfo->request_method, "PUT") == 0)
      Request->SetMethod(HttpdRequest::PUT);
   else if(strcmp(RequestInfo->request_method, "POST") == 0)
      Request->SetMethod(HttpdRequest::POST);
   else if(strcmp(RequestInfo->request_method, "DELETE") == 0)
      Request->SetMethod(HttpdRequest::DELETE);
   else if(strcmp(RequestInfo->request_method, "OPTIONS") == 0)
      Request->SetMethod(HttpdRequest::OPTIONS);
   else
      return(-1);

   // get uri
   if(Request->SetPath(String(RequestInfo->uri)) != 0)
      return(-1);

   // get data
   int Size = 0;
   const char *sz_DataSize = mg_get_header(Handle, "Content-Length");
   if(sz_DataSize != NULL)
   {
      errno = 0;
      char *EndPtr;
      Size = strtol(sz_DataSize, &EndPtr, 10);
      if( (errno != 0) || (*EndPtr != '\0') )
         return(-1);
   }

   if(Size != 0)
   {
      int ReadSize = 0;
      uint8_t Data[Size];
      ReadSize = mg_read(Handle, Data, Size);
      if(ReadSize != Size)
         return(-1);

      if(Request->SetPayload(Data, ReadSize) != 0)
         return(-1);
   }
   return(0);
}


int32_t Req2Mg(struct mg_connection *Handle,
               const HttpdRequest   &Answer)
{
   if(Handle == NULL)
      return(-1);

   // status
   ///@todo status error string
   mg_printf(Handle, "HTTP/1.1 %d OK\r\n", Answer.GetStatus());

   // get payload
   const uint8_t *p_Payload;
   uint32_t       i_PayloadSize;
   if(Answer.GetPayload(&p_Payload, &i_PayloadSize) != 0)
      return(-1);

   // common headers
   mg_printf(Handle, "Pragma: no-cache\r\n");
   mg_printf(Handle, "Content-Length: %d\r\n", i_PayloadSize);
   mg_printf(Handle, "Keep-Alive: timeout=5, max=92\r\n");
   mg_printf(Handle, "Connection: Keep-Alive\r\n");
   mg_printf(Handle, "Content-Type: text/plain; charset=utf-8\r\n");

   // answer headers
   HttpdRequest::Header *p_CurHeader;
   const AList<HttpdRequest::Header*>  &HeaderList = Answer.GetHeaderList();
   ListIterator<AList<HttpdRequest::Header*>, HttpdRequest::Header*> Iterator;

   Iterator.SetTo(&HeaderList);
   Iterator.First();
   while(Iterator.IsDone() == false)
   {
      if(Iterator.GetItem(&p_CurHeader) == 0)
      {
         mg_printf(Handle, "%s: %s\r\n",
               p_CurHeader->Name.GetString(),
               p_CurHeader->Value.GetString());
      }

      Iterator.Next();
   }
   // end of headers
   mg_printf(Handle, "\r\n");


   // write payload
   mg_write(Handle, p_Payload, i_PayloadSize);
   return(0);
}


void* MgEventHandler(enum mg_event Event,
                     struct mg_connection *Handle,
                     const struct mg_request_info *RequestInfo)
{
   HttpdRequest   Request;
   Httpd          *Server;

   if((Handle == NULL) || (RequestInfo == NULL) )
      return(NULL);

   if (Event != MG_NEW_REQUEST)
      return(NULL);

   Server = static_cast<Httpd*>(RequestInfo->user_data);
   if(Server == NULL)
      return(NULL);

   if(Mg2Req(Handle, RequestInfo, &Request) != 0)
      return(NULL);

   HttpdRequest Answer;
   Server->EventHandler(Request, &Answer);
   if(Req2Mg(Handle, Answer) == 0)
   {
      return (void*)"yes";
   }
   return(NULL);

}


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
   Stop();
   return;
}


/**
* @date     2012/01/23
*
*  Starts the server.
*
******************************************************************************/
int32_t Httpd::Start(uint16_t TcpPort)
{
   char sz_Port[16];
   const char *sz_Options[] =
   {
      "listening_ports", sz_Port,
      // "document_root", "",
      NULL
   };

   sprintf(sz_Port, "%d", TcpPort);
   MgHandle = mg_start(&MgEventHandler, this, sz_Options);
   if(MgHandle == NULL)
      return(-1);
   return(0);
}


/**
* @date     2012/01/23
*
*  Stops the server.
*
******************************************************************************/
int32_t Httpd::Stop(void)
{
   if(MgHandle != NULL)
   {
      mg_stop(MgHandle);
      MgHandle = NULL;
   }
   return(0);
}


/**
* @date     2011/12/21
*
*  Adds a new url handler. The handler must not be deleted as long as it has not
*  been removed from the handler list.
*
******************************************************************************/
int32_t Httpd::AddUrlHandler(UrlHandler *Handler)
{
   if(Handler == NULL)
      return(-1);

   HandlerList.AddItem(Handler);
   return(0);
}


/**
* @date     2011/12/21
*
*  Removes an url handler. The handler can then be deleted.
*
******************************************************************************/
int32_t Httpd::DelUrlHandler(UrlHandler *Handler)
{
   if(Handler == NULL)
      return(-1);

   HandlerList.DelItem(Handler);
   return(0);
}


/**
* @date     2011/12/21
*
*  Removes an url handler. The handler can then be deleted.
*
******************************************************************************/
void Httpd::EventHandler(const HttpdRequest &Request, HttpdRequest *Answer)
{
   UrlHandler *p_Handler = NULL;
   ListIterator<AList<UrlHandler*>, UrlHandler*> Iterator;
   UrlHandler *p_CurHandler;

   Iterator.SetTo(&HandlerList);
   Iterator.First();
   while( (Iterator.IsDone() == false) && (p_Handler == NULL) )
   {
      if(Iterator.GetItem(&p_CurHandler) == 0)
      {
         if(p_CurHandler->Handles(Request.GetPath()) == true)
            p_Handler = p_CurHandler;
      }
      Iterator.Next();
   }

   if(p_Handler != NULL)
   {
      p_Handler->RequestReceived(Request, Answer);
   }

}


