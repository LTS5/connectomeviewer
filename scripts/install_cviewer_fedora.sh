echo "Type the full path to store ConnectomeViewer and Enthought Toolstack Source Files:"
read fullp
mkdir $fullp
cd $fullp

echo "Type username to download ConnectomeViewer:"
read usern
echo "Type password to download ConnectomeViewer:"
read passw

# This removes Ubuntu Packages with are not uptodate
sudo yum remove python-AppTools python-EnvisageCore python-EnvisagePlugins python-Traits python-TraitsBackendQt python-TraitsGUI

# This adds required packages, including header files for compilation
sudo yum update python
yum install python-pip numpy scipy python-devel ipython vtk vtk-devel swig python-configobj libXtst libXtst-devel freeglut freeglut-devel python-matplotlib python-lxml Cython qscintilla-python python-sphinx wxPython wxPython-devel subversion gcc gcc-c++ libXt libXt-devel libX11-devel

# We need the Enthought Toolstack
sudo pip install ETSProjectTools

# You can also directly download an archive
mkdir ETS
cd ETS
wget --user=$usern --password=$passw  http://www.connectomeviewer.org/files/ETS_3.3.1.tar.gz
tar xzf ETS_3.3.1.tar.gz
sudo ets develop
cd ..

# Now we install required packages for the ConnectomeViewer using pip
# You can easily uninstall them
sudo easy_install networkx
sudo pip install h5py

# To access MySQL databases, we need PyMySQL
sudo easy_install http://pymysql.googlecode.com/files/PyMySQL-0.2-py2.6.egg

# Finally, install the ConnectomeViewer
wget --user=$usern --password=$passw http://www.connectomeviewer.org/files/ConnectomeViewer-0.1.6.tar.gz
tar xzf ConnectomeViewer-0.1.6.tar.gz
cd ConnectomeViewer-0.1.6
sudo python setup.py install
cd ..
# Download Example datasets
mkdir ExampleDatasets
cd ExampleDatasets
wget http://www.connectomeviewer.org/datasets/homo_sapiens_01.cff
wget http://www.connectomeviewer.org/datasets/homo_sapiens_02.cff
wget http://www.connectomeviewer.org/datasets/homo_sapiens_03.cff
wget http://www.connectomeviewer.org/datasets/macaca_mulatta_01.cff
cd ..
