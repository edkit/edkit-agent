import rpc.ws
import edleak.api
import edleak.slice_runner
import edleak.plot

ws_rpc = rpc.ws.WebService("localhost", 8080)
el = edleak.api.EdLeak(ws_rpc)
runner = edleak.slice_runner.SliceRunner(el)
asset = runner.run(1, 60)
elplot = edleak.plot.SlicePlotter(asset)
elplot.sizePlot()
