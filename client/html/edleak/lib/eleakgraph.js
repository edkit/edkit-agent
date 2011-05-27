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
* @date     2011/05/11
* 
*****************************************************************************/


var data = undefined;
var c = pv.Scale.linear(0, 100).range("lightgrey", "darkgreen");
var plot_data = [];
var allocer_label = [];
var memory_max = 0;
var scale = "linear";

// constructor 
vis = function(title) 
{ 
   this.title = title; 
}; 

vis.prototype.title = "";

vis.prototype.getMainPanel = function()
{
   var self = this;
   var x, h;
   var label_width = 250;
   var margin_right = 20;
   var w = $(window).width() - label_width - margin_right;

   convert_data();
   h = allocer_label.length*15;

   if(scale == "log")
      x = pv.Scale.log(1, memory_max).range(0, w);
   else
      x = pv.Scale.linear(0, memory_max).range(0, w);
   var y = pv.Scale.linear(0, allocer_label.length-1).range(0, h);
   var labels = pv.range(0, allocer_label.length, 1);

   /* The root panel. */
   var vis = new pv.Panel()
      .width(w)
      .height(h+15)
      .bottom(20)
      .left(label_width)
      .right(margin_right)
      .top(10);


   /* Y-axis and ticks. */
   vis.add(pv.Rule)
      .data(labels)
      .bottom(y)
      .strokeStyle(function(d) { return d ? "#eee" : "#000"} )
      .anchor("left").add(pv.Label)
         .textAlign("right")
         .text( function(d) { 
               return allocer_label[d] });

   /* X-axis and ticks. */
   vis.add(pv.Rule)
      .data(x.ticks())
      .left(x)
      .strokeStyle(function(d) { return d ? "#eee" : "#000"} )
      .anchor("bottom").add(pv.Label)
      .text(x.tickFormat);

   /* The dot plot! */
   vis.add(pv.Panel)
      .data(plot_data)
      .add(pv.Dot)
         .left(function(d) { return x(d.x)})
         .bottom(function(d) { return y(d.y)})
         .strokeStyle(function(d) { 
               return c(d.c)
               })
         .fillStyle(function() { return this.strokeStyle() })
         .size(function(d) { return 20});

   return(vis);

}


function find_allocer(allocer, value)
{
   var i;

   for(i=0; i<allocer.length; i++)
   {
      if(allocer[i] == value)
         return(i);
   }
   return undefined;
}


function convert_data()
{
   var   i,j;
   var   point_index;
   var   slice_count = data.length;
   var   slice_color_index;

   plot_data = [];
   allocer_label = [];
   memory_max = 0;

   point_index = 0;
   for(i=0; i<slice_count; i++)
   {
      slice_color_index = Math.round(i*100/slice_count)+1;
      for(j=0; j<data[i].length; j++)
      {
         /* do not print null values. This should be replaced with a real
          * algorithm to remove unleaked entries (first value == last value) */
         if(data[i][j].memory != 0)
         {
            allocer_index = find_allocer(allocer_label, [data[i][j].eip]);
            if(allocer_index == undefined)
            {
               allocer_label.push(data[i][j].eip);
               allocer_index = allocer_label.length - 1;
            }
            plot_data[point_index] = {};
            plot_data[point_index].c = slice_color_index;
            plot_data[point_index].x = data[i][j].memory;
            plot_data[point_index].y = allocer_index;
            if(data[i][j].memory > memory_max)
               memory_max = data[i][j].memory;
            point_index++;
         }
      }
   }
}


$(document).ready( function()
{
});
