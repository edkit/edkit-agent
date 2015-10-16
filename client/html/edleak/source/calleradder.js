/*
*****************************************************************************
*                      ___       _   _    _ _
*                     / _ \ __ _| |_| |__(_) |_ ___
*                    | (_) / _` | / / '_ \ |  _(_-<
*                     \___/\__,_|_\_\_.__/_|\__/__/
*                          Copyright (c) 2013
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
* @date     2013/03/25
* 
*****************************************************************************/

WsCallerAdder = function()
{
}

WsCallerAdder.prototype.onAdd = function(id) {
    var settings = ws_ui.getSettings();

   request = { 
      "InterfaceName": "Edleak",
      "MethodName": "AddStackWatch",
      "MethodParams": { "id": id}
   };
   var serialized_data = JSON.stringify(request);

   $.ajax( { "type": 'POST', "url": "http://" + settings.ip + ":" + settings.port + "/ws",
      "success": function(me) {
         return( function(data) {
            }
         );
      }(this),
      "error": function(jqXHR, textStatus, errorThrown) {
         return 0;
      },
      "dataType": "json",
      "processData" : false,
      "data" : serialized_data
      }
   );

   return;
}


WsCallerAdderUI = function(dom_root) {
   var menu_item  = '<div class="menu-item" id="caller_adder">';
   menu_item      += '<h4>Callers</h4>';
   menu_item      += '<div class="menu-content" >';
   menu_item      += '<input type="text" id="edkit_caller_adder_id" value="0" name="edkit_caller_adder_id" title="context id" />';
   menu_item      += '<input type="button" id="edkit_caller_add" value="+" name="edkit_caller_add" />';
   menu_item      += '</div></div>';
   $(dom_root).append(menu_item);

   this.domRoot = dom_root;
}

WsCallerAdderUI.prototype.domRoot = "";

WsCallerAdderUI.prototype.setClickCbk = function(cbk_click)
{
   $(this.domRoot + " " + "#edkit_caller_add").click(
      function(evt)
      {
         cbk_click();
      });
}

WsCallerAdderUI.prototype.getId = function() {
   return(parseInt($(this.domRoot + " " + "#edkit_caller_adder_id").val()));
}

$(document).ready( function()
{
   var adder_ui = new WsCallerAdderUI('#menu-bar');
   var ws_caller_adder = new WsCallerAdder();

   adder_ui.setClickCbk( function() {
      ws_caller_adder.onAdd(adder_ui.getId());
   });
});
