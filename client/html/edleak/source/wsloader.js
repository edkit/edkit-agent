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
* @date     2012/01/28
* 
*****************************************************************************/

var ws_ui = undefined;

WsLoader = function()
{
}

WsLoader.prototype.enabled    = false;
WsLoader.prototype.data       = undefined;
WsLoader.prototype.timer      = undefined;
WsLoader.prototype.period     = 3;  // capture period in seconds.
WsLoader.prototype.duration   = 300; // capture duration in seconds. default is minutes
WsLoader.prototype.tick       = 0;
WsLoader.prototype.cbkStarted = function() {};
WsLoader.prototype.cbkStopped = function() {};

WsLoader.prototype.setCbk = function(cbkStopped, cbkStarted)
{
   this.cbkStopped = cbkStopped;
   this.cbkStarted = cbkStarted;
}


WsLoader.prototype.getData = function()
{
   return(this.data);
}

WsLoader.prototype.onSliceData = function(data)
{
   this.data.slice.push(data.slice[0]);
   this.data.allocer = data.allocer;
}

WsLoader.prototype.onTimer = function()
{
   this.tick += this.period;
   if(this.tick >= this.duration)
   {
      this.stop();
   }
   else
   {
      request = { 
         "InterfaceName": "Edleak",
         "MethodName": "GetSlice",
         "MethodParams": {}
         };
      var serialized_data = JSON.stringify(request);

      $.ajax( { "type": 'POST', "url": "http://" + this.ip + ":" + this.port + "/ws",
         "success": function(me)
         {
            return( function(data)
            {
               if(data.status == 0)
               {
                  me.onSliceData(data.data);
               }
            });
         }(this),
         "error": function(jqXHR, textStatus, errorThrown)
         {
            return 0;
         },
         "dataType": "json",
         "processData" : false,
         "data" : serialized_data
         }
         );

   }

   return;
}

WsLoader.prototype.start = function(ip, port, duration, period)
{
   if(this.enabled)
      return(-1);

   this.period    = period;
   this.duration  = duration;
   this.ip        = ip;
   this.port      = port;
   this.tick      = 0;
   this.data      = { "slice" : [], "allocer": []};
   this.timer     = setInterval( function(me)
         {
         return( function()
            {
               me.onTimer();
            });
         }(this),
         this.period*1000);
   this.enabled = true;
   this.cbkStarted();
   return(0);
}
WsLoader.prototype.isStarted = function()
{
   return(this.enabled);
}

WsLoader.prototype.stop = function()
{
   if(!this.enabled)
      return(-1);

   this.enabled = false;
   clearInterval(this.timer);
   this.timer = undefined;
   this.cbkStopped();
   return(0);
}


WsLoaderUI = function(dom_root)
{
   var menu_item  = '<div class="menu-item" id="ws_loader">';
   menu_item      += '<h4>Network control</h4>';
   menu_item      += '<ul>';
   menu_item      += '<li><input type="text" id="edkit_ws_ip" value="127.0.0.1" name="edkit_ws_ip" title="ip:port" />';
   menu_item      += '<input type="button" id="edkit_ws_start" value=">" name="edkit_ws_start"  /></li>';
   menu_item      += '<li><input type="text" id="edkit_ws_duration" value="20" name="edkit_ws_duration" title="duration (s)"  />';
   menu_item      += '<input type="text" id="edkit_ws_period" value="3" name="edkit_ws_period" title="period (s)" /></li>';
   menu_item      += '</ul>';
   menu_item      += '</div>';
   $(dom_root).append(menu_item);

   this.domRoot = dom_root;
}

WsLoader.prototype.domRoot = "";

WsLoaderUI.prototype.setClickCbk = function(cbk_click)
{
   $(this.domRoot + " " + "#edkit_ws_start").click(
      function(evt)
      {
         cbk_click();
      });
}

WsLoaderUI.prototype.getSettings = function()
{
   var hostname = $(this.domRoot + " " + "#edkit_ws_ip").val();
   hostname = hostname.split(':');
   settings = {};
   settings.ip       = hostname[0];
   if(hostname.length > 1) {
      settings.port     = hostname[1];
   }
   else {
      settings.port     = 8080;
   }
   settings.duration = parseInt($(this.domRoot + " " + "#edkit_ws_duration").val());
   settings.period   = parseInt($(this.domRoot + " " + "#edkit_ws_period").val());
   return(settings);
}

$(document).ready( function()
{
   ws_ui = new WsLoaderUI('#menu-bar');
   var ws_loader = new WsLoader();

   ws_loader.setCbk(
      function()
      {
         data = ws_loader.getData();
         vis_display();
      },
      function()
      {
      });

   ws_ui.setClickCbk( function()
      {
         if(ws_loader.isStarted() == false)
         {
            var settings = ws_ui.getSettings();
            ws_loader.start(settings.ip, settings.port, 
                           settings.duration,
                           settings.period);
         }
         else
         {
            ws_loader.stop();
         }
      });
});
