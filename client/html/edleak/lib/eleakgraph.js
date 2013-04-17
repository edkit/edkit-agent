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
vis = function(container) 
{
   this.container = container;
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
   h = data.allocer.length*10;
   h += 100; // x axis
   $('#scatter-graph').css("height", h);

   var graph_scale;
   if(scale == "log")
      graph_scale = "logarithmic";
   else
      graph_scale =  "linear";

     var options =   {
            chart: {
                renderTo: this.container,
                type: 'scatter',
                zoomType: 'x'
            },
            title: {
                text: null
            },
            xAxis: {
                title: {
                    enabled: true,
                    text: 'Size (Byte)'
                },
                startOnTick: true,
                endOnTick: true,
                showLastLabel: true,
                type: graph_scale
            },
            yAxis: {
                title: {
                    text: null
                }
            },
            tooltip: {
                formatter: function() {
                        var size = this.x;
                        var size_unit = 'B';
                        if(size > 1024) {
                           size = size/1024;
                           var size_unit = 'KiB';
                        }
                        if(size > 1024) {
                           size = size/1024;
                           var size_unit = 'MiB';
                        }
                        size = Math.round(size);
                        var result = 'size: ' + size + size_unit + '<br/>' +
                           'id: '+ data.allocer[this.y].id + '<br/>' +
                           'backtrace: <br/> ';
                        var i;
                        for(i=0;i<data.allocer[this.y].stack.length;i++) {
                           result += ' - ' + data.allocer[this.y].stack[i] + '<br/>';
                        }
                        return(result);
                }
            },
            legend: {
                layout: 'vertical',
                align: 'left',
                verticalAlign: 'top',
                x: 100,
                y: 70,
                floating: true,
                backgroundColor: '#FFFFFF',
                borderWidth: 1
            },
            plotOptions: {
                scatter: {
                    marker: {
                        radius: 5,
                        states: {
                            hover: {
                                enabled: true,
                                lineColor: 'rgb(100,100,100)'
                            }
                        }
                    },
                    states: {
                        hover: {
                            marker: {
                                enabled: false
                            }
                        }
                    }
                }
            },
            series : [{ data: plot_data,  turboThreshold: 500000}],
               };

 

   var chart = new Highcharts.Chart(options);
   return(chart);

}


function convert_data()
{
   var   i,j;
   var   slice_count = data.slice.length;
   var   slice_color_index;

   plot_data = [];
   memory_max = 0;

   for(i=0; i<slice_count; i++)
   {
      slice_color_index = Math.round(i*100/slice_count)+1;
      var color_obj = c(slice_color_index);

      for(j=0; j<data.slice[i].length; j++)
      {
         /* remove null values. For two reasons:
          * - A 0 entry cannot leak. This should be replaced with a real
          *    algorithm to remove unleaked entries (first value == last value)
          * - 0 is not valid on log scale. */
         /* remove identical values to avoid adding useless points on the graph */
         if( (data.slice[i][j].mem != 0) && 
             ( (j>0) && (data.slice[i][j].mem != data.slice[i][j-1].mem) )
            )
         {
            plot_data.push( {
               x: data.slice[i][j].mem,
               y: data.slice[i][j].alc,
               marker: { fillColor: color_obj.color} });
            if(data.slice[i][j].mem > memory_max)
               memory_max = data.slice[i][j].mem;
         }
      }
   }
   plot_data.reverse();
}


