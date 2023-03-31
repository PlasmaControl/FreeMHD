from paraview.simple import *
from paraview import coprocessing


#--------------------------------------------------------------
# Code generated from cpstate.py to create the CoProcessor.
# paraview version 5.5.0

#--------------------------------------------------------------
# Global screenshot output options
imageFileNamePadding=4
rescale_lookuptable=False


# ----------------------- CoProcessor definition -----------------------

def CreateCoProcessor():
  def _CreatePipeline(coprocessor, datadescription):
    class Pipeline:
      # state file generated using paraview version 5.5.0

      # ----------------------------------------------------------------
      # setup views used in the visualization
      # ----------------------------------------------------------------

      # trace generated using paraview version 5.5.0

      #### disable automatic camera reset on 'Show'
      paraview.simple._DisableFirstRenderCameraReset()

      # Create a new 'Render View'
      renderView1 = CreateView('RenderView')
      renderView1.ViewSize = [1077, 763]
      renderView1.AxesGrid = 'GridAxes3DActor'
      renderView1.CenterOfRotation = [-1840.0, 130.5, 1801.3900146484375]
      renderView1.StereoType = 0
      renderView1.CameraPosition = [-696.8141215453355, 4985.004213421186, 5728.631468968153]
      renderView1.CameraFocalPoint = [-1879.8749783277472, 159.71276405861283, 1777.6527129842225]
      renderView1.CameraViewUp = [-0.16012263584209638, -0.6015546920663194, 0.7826191244430301]
      renderView1.CameraParallelScale = 1019.0865679302492
      renderView1.CameraParallelProjection = 1
      renderView1.Background = [0.0, 0.0, 0.0]

      # init the 'GridAxes3DActor' selected for 'AxesGrid'
      renderView1.AxesGrid.XTitleFontFile = ''
      renderView1.AxesGrid.YTitleFontFile = ''
      renderView1.AxesGrid.ZTitleFontFile = ''
      renderView1.AxesGrid.XLabelFontFile = ''
      renderView1.AxesGrid.YLabelFontFile = ''
      renderView1.AxesGrid.ZLabelFontFile = ''

      # register the view with coprocessor
      # and provide it with information such as the filename to use,
      # how frequently to write the images, etc.
      coprocessor.RegisterView(renderView1,
          filename='image_%t.png', freq=4, fittoscreen=0, magnification=1, width=1077, height=763, cinema={})
      renderView1.ViewTime = datadescription.GetTime()

      # ----------------------------------------------------------------
      # restore active view
      SetActiveView(renderView1)
      # ----------------------------------------------------------------

      # ----------------------------------------------------------------
      # setup the data processing pipelines
      # ----------------------------------------------------------------

      # create a new 'XML MultiBlock Data Reader'
      # create a producer from a simulation input
      input = coprocessor.CreateProducer(datadescription, 'area')

      # create a new 'Cell Data to Point Data'
      cellDatatoPointData1 = CellDatatoPointData(Input=input)

      # create a new 'Warp By Scalar'
      warpByScalar1 = WarpByScalar(Input=cellDatatoPointData1)
      warpByScalar1.Scalars = ['POINTS', 'h']
      warpByScalar1.ScaleFactor = 3.0

      # ----------------------------------------------------------------
      # setup the visualization in view 'renderView1'
      # ----------------------------------------------------------------

      # show data from warpByScalar1
      warpByScalar1Display = Show(warpByScalar1, renderView1)

      # get color transfer function/color map for 'h'
      hLUT = GetColorTransferFunction('h')
      hLUT.AutomaticRescaleRangeMode = 'Never'
      hLUT.RGBPoints = [0.0, 1.0, 1.0, 0.988235, 0.04, 1.0, 1.0, 0.988235, 1.0, 0.984314, 0.988235, 0.843137, 2.0, 0.988235, 0.988235, 0.741176, 3.0, 0.980392, 0.968627, 0.654902, 4.0, 0.980392, 0.945098, 0.576471, 5.0, 0.968627, 0.905882, 0.486275, 6.0, 0.968627, 0.862745, 0.388235, 7.0, 0.960784, 0.803922, 0.286275, 8.0, 0.94902, 0.741176, 0.219608, 9.0, 0.941176, 0.678431, 0.14902, 10.0, 0.929412, 0.607843, 0.094118, 11.0, 0.921569, 0.545098, 0.054902, 12.0, 0.909804, 0.486275, 0.035294, 13.0, 0.890196, 0.411765, 0.019608, 14.0, 0.8, 0.305882, 0.0, 15.0, 0.760784, 0.239216, 0.0, 16.0, 0.678431, 0.180392, 0.011765, 17.0, 0.6, 0.121569, 0.023529, 18.0, 0.501961, 0.054902, 0.031373, 19.0, 0.4, 0.039216, 0.058824, 20.0, 0.301961, 0.047059, 0.090196]
      hLUT.ColorSpace = 'Lab'
      hLUT.NanColor = [0.25, 0.0, 0.0]
      hLUT.ScalarRangeInitialized = 1.0

      # trace defaults for the display properties.
      warpByScalar1Display.Representation = 'Surface'
      warpByScalar1Display.ColorArrayName = ['POINTS', 'h']
      warpByScalar1Display.LookupTable = hLUT
      warpByScalar1Display.OSPRayScaleArray = 'Sd'
      warpByScalar1Display.OSPRayScaleFunction = 'PiecewiseFunction'
      warpByScalar1Display.SelectOrientationVectors = 'None'
      warpByScalar1Display.ScaleFactor = 243.9
      warpByScalar1Display.SelectScaleArray = 'None'
      warpByScalar1Display.GlyphType = 'Arrow'
      warpByScalar1Display.GlyphTableIndexArray = 'None'
      warpByScalar1Display.GaussianRadius = 12.195
      warpByScalar1Display.SetScaleArray = ['POINTS', 'Sd']
      warpByScalar1Display.ScaleTransferFunction = 'PiecewiseFunction'
      warpByScalar1Display.OpacityArray = ['POINTS', 'Sd']
      warpByScalar1Display.OpacityTransferFunction = 'PiecewiseFunction'
      warpByScalar1Display.DataAxesGrid = 'GridAxesRepresentation'
      warpByScalar1Display.SelectionCellLabelFontFile = ''
      warpByScalar1Display.SelectionPointLabelFontFile = ''
      warpByScalar1Display.PolarAxes = 'PolarAxesRepresentation'

      # init the 'PiecewiseFunction' selected for 'ScaleTransferFunction'
      warpByScalar1Display.ScaleTransferFunction.Points = [0.0, 0.0, 0.5, 0.0, 1.1757813367477812e-38, 1.0, 0.5, 0.0]

      # init the 'PiecewiseFunction' selected for 'OpacityTransferFunction'
      warpByScalar1Display.OpacityTransferFunction.Points = [0.0, 0.0, 0.5, 0.0, 1.1757813367477812e-38, 1.0, 0.5, 0.0]

      # init the 'GridAxesRepresentation' selected for 'DataAxesGrid'
      warpByScalar1Display.DataAxesGrid.XTitleFontFile = ''
      warpByScalar1Display.DataAxesGrid.YTitleFontFile = ''
      warpByScalar1Display.DataAxesGrid.ZTitleFontFile = ''
      warpByScalar1Display.DataAxesGrid.XLabelFontFile = ''
      warpByScalar1Display.DataAxesGrid.YLabelFontFile = ''
      warpByScalar1Display.DataAxesGrid.ZLabelFontFile = ''

      # init the 'PolarAxesRepresentation' selected for 'PolarAxes'
      warpByScalar1Display.PolarAxes.PolarAxisTitleFontFile = ''
      warpByScalar1Display.PolarAxes.PolarAxisLabelFontFile = ''
      warpByScalar1Display.PolarAxes.LastRadialAxisTextFontFile = ''
      warpByScalar1Display.PolarAxes.SecondaryRadialAxesTextFontFile = ''

      # setup the color legend parameters for each legend in this view

      # get color legend/bar for hLUT in view renderView1
      hLUTColorBar = GetScalarBar(hLUT, renderView1)
      hLUTColorBar.Title = 'h'
      hLUTColorBar.ComponentTitle = ''
      hLUTColorBar.TitleFontFile = ''
      hLUTColorBar.LabelFontFile = ''

      # set color bar visibility
      hLUTColorBar.Visibility = 1

      # show color legend
      warpByScalar1Display.SetScalarBarVisibility(renderView1, True)

      # ----------------------------------------------------------------
      # setup color maps and opacity mapes used in the visualization
      # note: the Get..() functions create a new object, if needed
      # ----------------------------------------------------------------

      # get opacity transfer function/opacity map for 'h'
      hPWF = GetOpacityTransferFunction('h')
      hPWF.Points = [0.0, 0.0, 0.5, 0.0, 20.0, 1.0, 0.5, 0.0]
      hPWF.ScalarRangeInitialized = 1

      # ----------------------------------------------------------------
      # finally, restore active source
      SetActiveSource(warpByScalar1)
      # ----------------------------------------------------------------
    return Pipeline()

  class CoProcessor(coprocessing.CoProcessor):
    def CreatePipeline(self, datadescription):
      self.Pipeline = _CreatePipeline(self, datadescription)

  coprocessor = CoProcessor()
  # these are the frequencies at which the coprocessor updates.
  freqs = {'input': [4, 4, 4]}
  coprocessor.SetUpdateFrequencies(freqs)
  return coprocessor


#--------------------------------------------------------------
# Global variable that will hold the pipeline for each timestep
# Creating the CoProcessor object, doesn't actually create the ParaView pipeline.
# It will be automatically setup when coprocessor.UpdateProducers() is called the
# first time.
coprocessor = CreateCoProcessor()

#--------------------------------------------------------------
# Enable Live-Visualizaton with ParaView and the update frequency
coprocessor.EnableLiveVisualization(True, 1)

# ---------------------- Data Selection method ----------------------

def RequestDataDescription(datadescription):
    "Callback to populate the request for current timestep"
    global coprocessor
    if datadescription.GetForceOutput() == True:
        # We are just going to request all fields and meshes from the simulation
        # code/adaptor.
        for i in range(datadescription.GetNumberOfInputDescriptions()):
            datadescription.GetInputDescription(i).AllFieldsOn()
            datadescription.GetInputDescription(i).GenerateMeshOn()
        return

    # setup requests for all inputs based on the requirements of the
    # pipeline.
    coprocessor.LoadRequestedData(datadescription)

# ------------------------ Processing method ------------------------

def DoCoProcessing(datadescription):
    "Callback to do co-processing for current timestep"
    global coprocessor

    # Update the coprocessor by providing it the newly generated simulation data.
    # If the pipeline hasn't been setup yet, this will setup the pipeline.
    coprocessor.UpdateProducers(datadescription)

    # Write output data, if appropriate.
    coprocessor.WriteData(datadescription);

    # Write image capture (Last arg: rescale lookup table), if appropriate.
    coprocessor.WriteImages(datadescription, rescale_lookuptable=rescale_lookuptable,
        image_quality=0, padding_amount=imageFileNamePadding)

    # Live Visualization, if enabled.
    coprocessor.DoLiveVisualization(datadescription, "localhost", 22222)
