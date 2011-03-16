#!/bin/sh

echo "Welcome to the Installation Script for ConnectomeViewer BETA"
echo "============================================================"
echo "Do you want to download the example datasets ? [Y]es / [N]o "
read exdata
case "$exdata" in
# Note variable is quoted.

  "Y" | "y" | "yes" | "Yes" )
  echo "========================================="
  echo "Download Connectome File Example datasets"
  echo "========================================="
  echo "Type the full path to store ConnectomeViewer datasets:"
  read fullp
  mkdir -p $fullp
  cd $fullp
  wget http://www.connectomeviewer.org/datasets/homo_sapiens_01.cff
  wget http://www.connectomeviewer.org/datasets/homo_sapiens_02.cff
  wget http://www.connectomeviewer.org/datasets/homo_sapiens_03.cff
  wget http://www.connectomeviewer.org/datasets/macaca_mulatta_01.cff
  cd ..
  ;;
          * )
   # Default option.	  
   # Empty input (hitting RETURN) fits here, too.
   echo
   echo "Not downloading Connectome File example datasets."
  ;;

esac


echo "===================================================================="
echo "Add required Ubuntu packages, including header files for compilation"
echo "===================================================================="
sudo apt-get update
sudo apt-get install git-core python-setuptools libvtk5.4 python-vtk python-numpy python-wxversion python2.6-dev g++ swig python-configobj glutg3 glutg3-dev libxtst-dev ipython python-lxml
sudo apt-get install python-matplotlib python-qscintilla2 gcc scons python-xlib pyqt4-dev-tools python-scipy python-pyrex python-all-dev python-dicom
sudo apt-get install libxt-dev libglu1-mesa-dev python-pip wget python-wxgtk2.8 python-h5py python-envisagecore python-envisageplugins python-traitsbackendwx python-traitsbackendqt python-traitsgui python-enthoughtbase python-chaco python-lxml python-h5py mayavi2 python-tables python-tables-doc python-apptools python-pip python-wxtools python-sphinx

echo "========================================================="
echo "Install/Update required packages for the ConnectomeViewer"
echo "========================================================="

echo "============"
echo "... NetworkX"
echo "============"
sudo pip install --upgrade networkx

echo "=========="
echo "... Cython"
echo "=========="
sudo pip install --upgrade Cython

echo "==========="
echo "... Nibabel"
echo "==========="
sudo pip install --upgrade nibabel

echo "========================================="
echo "Download and install the ConnectomeViewer"
echo "========================================="

# sudo pip install -e git://github.com/LTS5/connectomeviewer.git@master#egg=ConnectomeViewer

wget --no-check-certificate http://github.com/downloads/LTS5/connectomeviewer/ConnectomeViewer-0.1.9.tar.gz
tar xzf ConnectomeViewer-0.1.9.tar.gz
cd ConnectomeViewer-0.1.9/
sudo python setup.py install
cd ..
sudo rm -rf ConnectomeViewer-0.1.9/

echo "==================================================="
echo "The installation script is finished. It may well be that errors have occured."
echo "If you got a Permission error. Try to rerun the script with 'sudo ./install_cviewer_ubuntu.sh'"
ECHO ""
echo "Test your ConnectomeViewer installation by typing in the terminal:
echo "-----------
echo "connectomeviewer.py -v
echo "-----------
echo ""
echo "If there are problems, rerun the installation script with:"
echo "-----------
echo "sh ./install_cviewer_ubuntu10_10.sh > logfile.txt"
echo "-----------
echo "Send the logfile.txt together with ConnectomeViewer startup logfile (automatically generated) to info@connectomics.org."
echo "============================================================="

