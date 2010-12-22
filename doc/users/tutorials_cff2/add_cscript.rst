Add a text file or script to the Connectome File
================================================

You need to know the absolute filename path of the file that could be a script to add.

Enter in the IPython Shell::

  import cfflib as cf
  
Create a new CScript object::

  cs = cf.CScript.create_from_file(name='myscript name', filename = '/tmp/myfile.py')
  
Now add it to the connectome file::

  cfile.obj.add_connectome_script(cs)
  
And update the connectome file view ::

  cfile.update_children()
  
Now you can save the file including the script with File->Save Connectome File...