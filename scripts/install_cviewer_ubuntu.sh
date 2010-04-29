echo "Type the full path to store ConnectomeViewer and Enthought Toolstack Source Files:"
read fullp
mkdir $fullp
cd $fullp

echo "Type username to download ConnectomeViewer:"
read usern
echo "Type password to download ConnecctomeViewer:"
read passw

# This removes Ubuntu Packages with are not uptodate
sudo apt-get remove mayavi2 python-enthoughtbase python-enthought-chaco2 python-enthought-traits python-enthought-traits-ui python-apptools python-chaco python-envisageplugins python-networkx

# This adds required packages, including header files for compilation
sudo apt-get install subversion python-setuptools libvtk5.2 python-vtk python-numpy python-wxversion python2.6-dev python-sphinx g++ swig python-configobj glutg3 glutg3-dev libxtst-dev ipython python-lxml python-matplotlib python-qscintilla2 cython gcc scons python-xlib pyqt4-dev-tools python-scipy python-pyrex python-all-dev libxt-dev libglu1-mesa-dev python-pip wget

# We need the Enthought Toolstack
sudo pip install ETSProjectTools

# You can also checkout the current version of ETS with:
#  ets co ETS
# This takes some time...please be patient
# The following projects are checked out
# Checking out "ETS==3.3.1" source from "https://svn.enthought.com/svn/enthought/ETS/trunk"
# Checking out "SciMath['traits']==3.0.5" source from "https://svn.enthought.com/svn/enthought/SciMath/trunk"
# Checking out "Enable['traits']==3.2.1" source from "https://svn.enthought.com/svn/enthought/Enable/trunk"
# Checking out "Chaco==3.2.1" source from "https://svn.enthought.com/svn/enthought/Chaco/trunk"
# Checking out "ETSDevTools==3.0.4" source from "https://svn.enthought.com/svn/enthought/ETSDevTools/trunk"
# Checking out "EnvisageCore==3.1.2" source from "https://svn.enthought.com/svn/enthought/EnvisageCore/trunk"
# Checking out "TraitsBackendQt==3.2.1" source from "https://svn.enthought.com/svn/enthought/TraitsBackendQt/trunk"
# Checking out "ETSProjectTools==0.6.0" source from "https://svn.enthought.com/svn/enthought/ETSProjectTools/trunk"
# Checking out "TraitsBackendWX==3.2.1" source from "https://svn.enthought.com/svn/enthought/TraitsBackendWX/trunk"
# Checking out "Mayavi==3.3.1" source from "https://svn.enthought.com/svn/enthought/Mayavi/trunk"
# Checking out "EnvisagePlugins==3.1.2" source from "https://svn.enthought.com/svn/enthought/EnvisagePlugins/trunk"
# Checking out "BlockCanvas==3.1.1" source from "https://svn.enthought.com/svn/enthought/BlockCanvas/trunk"
# Checking out "EnthoughtBase['traits', 'distribution', 'ui']==3.0.4" source from "https://svn.enthought.com/svn/enthought/EnthoughtBase/trunk"
# Checking out "TraitsGUI['dock']==3.1.1" source from "https://svn.enthought.com/svn/enthought/TraitsGUI/trunk"
# Checking out "SetupDocs==1.0.4" source from "https://svn.enthought.com/svn/enthought/SetupDocs/trunk"
# Checking out "Traits['ui']==3.2.1" source from "https://svn.enthought.com/svn/enthought/Traits/trunk"
# Checking out "CodeTools==3.1.1" source from "https://svn.enthought.com/svn/enthought/CodeTools/trunk"
# Checking out "AppTools==3.3.1" source from "https://svn.enthought.com/svn/enthought/AppTools/trunk"

# You can also directly download an archive
wget --user=$usern --password=$passw  http://www.connectomeviewer.org/files/ETS_3.3.1.tar.gz
mkdir ETS
cd ETS
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

