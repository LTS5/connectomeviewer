* Write a new dataset class that load files in a dictionary whose keys
  are times. This will allow to load dynamically only a few images around 
  the current time, and therefore save on memory.

* [ENH] Changing the time field should update the visualization.

* An error should be generated when TimeVisualizer.time lies outside the 
  preloaded_range.

* Add keyword l_max in TimeVisualizer constructor to specify the size in
  pixels over which we sample by default.   

* Modify the DataSet class to have a generic loading function (can be
  np.load, tb.openFile, etc)
