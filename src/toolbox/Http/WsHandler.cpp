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
* @date     2011/12/21
*
*****************************************************************************/
#include "OakString.h"
#include "WsHandler.h"
#include "HttpdRequest.h"
#include "WsInterface.h"
#include "ListIterator.h"
#include "JsonFlattener.h"


/**
* @date     2011/12/21
*
*  Constructor.
*
******************************************************************************/
WsHandler::WsHandler(const String &Url):
   UrlHandler(Url),
   InitStatus(UrlHandler::InitCheck()),
   InterfaceList()
{
   return;
}


/**
* @date     2011/12/21
*
*  Destructor.
*
******************************************************************************/
WsHandler::~WsHandler(void)
{
   return;
}


/**
* @date     2012/01/08
*
*  Adds an interface to this handler.
*
******************************************************************************/
int32_t  WsHandler::AddInterface(WsInterface *p_Interface)
{
   if(p_Interface == NULL)
      return(-1);

   return(InterfaceList.AddItem(p_Interface));
}


/**
* @date     2012/01/08
*
*  Removes an interface to this handler.
*
******************************************************************************/
int32_t  WsHandler::DelInterface(WsInterface *p_Interface)
{
   if(p_Interface == NULL)
      return(-1);

   return(InterfaceList.DelItem(p_Interface));
}


/**
* @date     2011/12/21
*
*  Handler of a request. The WebService request si built from the request, and
*  forwarded to the correct WsInterface.
*
******************************************************************************/
int32_t  WsHandler::RequestReceived(const HttpdRequest &Request,
                                    HttpdRequest *Answer)
{
   int32_t i_Ret = -1;
   ListIterator<AList<WsInterface*>, WsInterface*>  Iterator;
   WsInterface    *p_Interface = NULL;
   const String   *RequestInterface;

   if(HandleCors(Request, Answer) == 0)
      return(0);

   DynObject   Params;
   const uint8_t *Payload;
   uint32_t i_Length;
   i_Ret = Request.GetPayload(&Payload, &i_Length);
   if(i_Ret == 0)
   {
      JsonFlattener Flattener;
      const String PayloadString((const char*)(Payload), i_Length);

      i_Ret = Flattener.UnFlatten(&PayloadString, &Params);
      if(i_Ret == 0)
      {
         i_Ret = Params.FindString("InterfaceName", &RequestInterface);
      }
   }
   if(i_Ret != 0)
      return(-1);

   Iterator.SetTo(&InterfaceList);
   Iterator.First();
   while(Iterator.IsDone() == false)
   {
      if( (Iterator.GetItem(&p_Interface) == 0) && (p_Interface != NULL) )
      {
         if(p_Interface->GetName() == *RequestInterface)
         {
            String   AnswerString;
            i_Ret = p_Interface->Call(Params, &AnswerString);
            if(i_Ret == 0)
            {
               i_Ret = Answer->SetPayload((const uint8_t*)AnswerString.GetString(),
                                          AnswerString.GetSize());
            }
            break;
         }
      }
      Iterator.Next();
   }

   return(i_Ret);
}

/**
* @date     2012/01/28
*
*  Cors handler (Cross Origin Resource Sharing). All Ws interfaces are
*  considered allowing cors.
*
* @return 0 if the request is Cors and has been served.
* @return -1 otherwise.
******************************************************************************/
int32_t  WsHandler::HandleCors(const HttpdRequest &Request,
                               HttpdRequest *Answer)
{
   if(Answer == NULL)
      return(-1);

   // always add this header to handle simple request
   Answer->SetHeader("Access-Control-Allow-Origin", "*");

   // handle preflight requests
   if(Request.GetMethod() != HttpdRequest::OPTIONS)
      return(-1);

   String AcrmHeader;
   if(Request.GetHeader("Access-Control-Request-Method", &AcrmHeader) == 0)
   {
      Answer->SetHeader("Access-Control-Allow-Methods", "POST");
      return(0);
   }
   return(-1);
}
