echo "Welcome to the Installation Script for ConnectomeViewer"
echo "======================================================="
echo "Type the full path to store ConnectomeViewer and Enthought Toolsuite Source Files:"
read fullp
mkdir $fullp
cd $fullp

echo "============================================="
echo "Remove Ubuntu Packages which are not uptodate"
echo "============================================="
sudo apt-get remove mayavi2 python-enthoughtbase python-enthought-chaco2 python-enthought-traits python-enthought-traits-ui python-apptools python-chaco python-envisageplugins python-networkx python-wxgtk2.6

echo "===================================================================="
echo "Add required Ubuntu packages, including header files for compilation"
echo "===================================================================="
sudo apt-get install subversion python-setuptools libvtk5.2 python-vtk python-numpy python-wxversion python2.6-dev python-sphinx g++ swig python-configobj glutg3 glutg3-dev libxtst-dev ipython python-lxml python-matplotlib python-qscintilla2 gcc scons python-xlib pyqt4-dev-tools python-scipy python-pyrex python-all-dev libxt-dev libglu1-mesa-dev python-pip wget python-wxgtk2.8

echo "==============================="
echo "We need the Enthought Toolsuite"
echo "==============================="
sudo pip install ETSProjectTools

# currently includes
# "ETS==3.3.1"
# "SciMath['traits']==3.0.5" "Enable['traits']==3.2.1" "Chaco==3.2.1"
# "ETSDevTools==3.0.4" "EnvisageCore==3.1.2" "TraitsBackendQt==3.2.1"
# "ETSProjectTools==0.6.0" "TraitsBackendWX==3.2.1" "Mayavi==3.3.1"
# "EnvisagePlugins==3.1.2" "BlockCanvas==3.1.1" "EnthoughtBase['traits', 'distribution', 'ui']==3.0.4"
# "TraitsGUI['dock']==3.1.1" "SetupDocs==1.0.4" "Traits['ui']==3.2.1" "CodeTools==3.1.1" "AppTools==3.3.1"

mkdir ETS
cd ETS
wget http://www.connectomeviewer.org/files/ETS_3.3.1.tar.gz
tar zxvf ETS_3.3.1.tar.gz
sudo ets install
cd ..
echo "============================================================================================="
echo "Enthought Toolsuite Download, Compilation and Installation completed (There might be errors)."
echo "============================================================================================="
echo "Install some required packages for the ConnectomeViewer"
echo "======================================================="

echo "============"
echo "... NetworkX"
echo "============"
sudo easy_install networkx

echo "=========="
echo "... Cython"
echo "=========="
sudo easy_install cython

echo "==============="
echo "... h5py (HDF5)"
echo "==============="
sudo pip install h5py

echo "==========="
echo "... PyMySQL"
echo "==========="
sudo easy_install http://pymysql.googlecode.com/files/PyMySQL-0.2-py2.6.egg

echo "========================================="
echo "Download and install the ConnectomeViewer"
echo "========================================="
wget http://www.connectomeviewer.org/files/ConnectomeViewer-0.1.6.tar.gz
tar xzf ConnectomeViewer-0.1.6.tar.gz
cd ConnectomeViewer-0.1.6
sudo python setup.py install
cd ..

echo "========================================="
echo "Download Connectome File Example datasets"
echo "========================================="
mkdir ExampleDatasets
cd ExampleDatasets
wget http://www.connectomeviewer.org/datasets/homo_sapiens_01.cff
wget http://www.connectomeviewer.org/datasets/homo_sapiens_02.cff
wget http://www.connectomeviewer.org/datasets/homo_sapiens_03.cff
wget http://www.connectomeviewer.org/datasets/macaca_mulatta_01.cff
cd ..

echo "==================================================="
echo "The script went through. Errors might have occured."
echo ""
echo "Try to start ConnectomeViewer with typing:"
echo " connectomeviewer -v"
echo "If the ConnectomeViewer does not work properly, please re-run this installation script with:"
echo " sh ./install_cviewer_ubuntu.sh > logfile.txt"
echo "and send the logfile.txt to info@connectomics.org. Thank you!"
echo "============================================================="