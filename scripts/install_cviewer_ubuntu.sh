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
sudo apt-get install git-core python-setuptools libvtk5.2 python-vtk python-numpy python-wxversion python2.6-dev python-sphinx g++ swig python-configobj glutg3 glutg3-dev libxtst-dev ipython python-lxml python-matplotlib python-qscintilla2 gcc scons python-xlib pyqt4-dev-tools python-scipy python-pyrex python-all-dev libxt-dev libglu1-mesa-dev python-pip wget python-wxgtk2.8 python-h5py python-envisagecore python-envisageplugins python-traitsbackendwx python-traitsbackendqt python-traitsgui python-traits python-enthought-traits-ui python-enthoughtbase python-chaco python-lxml python-h5py mayavi2 python-tables python-tables-doc python-apptools python-enthought-traits python-pip python-wxtools


echo "========================================================="
echo "Install/Update required packages for the ConnectomeViewer"
echo "========================================================="

echo "============"
echo "... NetworkX"
echo "============"
pip install networkx

echo "=========="
echo "... Cython"
echo "=========="
pip install Cython

echo "========================================="
echo "Download and install the ConnectomeViewer"
echo "========================================="

# sudo pip install -e git://github.com/unidesigner/connectomeviewer.git@master#egg=ConnectomeViewer

wget http://github.com/downloads/unidesigner/connectomeviewer/ConnectomeViewer-0.1.8.tar.gz
tar xzf ConnectomeViewer-0.1.8.tar.gz
cd ConnectomeViewer-0.1.8/
python setup.py install
cd ..
rm -rf ConnectomeViewer-0.1.8/

echo "==================================================="
echo "The installation script is finished. It may well be that errors have occured."
echo "If you got a Permission error. Try to rerun the script with 'sudo ./install_cviewer_ubuntu.sh'"
ECHO ""
echo "Test your ConnectomeViewer installation by typing in the terminal:
echo "-----------
echo "connectomeviewer -v
echo "-----------
echo ""
echo "If there are problems, rerun the installation script with:"
echo "-----------
echo "sh ./install_cviewer_ubuntu.sh > logfile.txt"
echo "-----------
echo "Send the logfile.txt together with ConnectomeViewer startup logfile (automatically generated) to info@connectomics.org."
echo "============================================================="

# ====================================================================
# Add required Ubuntu packages, including header files for compilation
# ====================================================================
# Paketlisten werden gelesen... Fertig
# Abhängigkeitsbaum wird aufgebaut       
# Status-Informationen einlesen... Fertig
# gcc ist schon die neueste Version.
# wget ist schon die neueste Version.
# Die folgenden zusätzlichen Pakete werden installiert:
#   blt freeglut3 freeglut3-dev g++-4.4 global libamd2.2.0 libaudio2 libavcodec52 libavformat52 libavutil49 libblas3gf libdigest-sha1-perl
#   libdrm-dev liberror-perl libgfortran3 libgl1-mesa-dev libgl1-mesa-glx libgl2ps0 libglu1-mesa libgsm1 libhdf5-serial-1.8.4 libibverbs1
# #   libice-dev liblapack3gf libmng1 libmysqlclient16 libnuma1 libopenmpi1.3 libphonon4 libpthread-stubs0 libpthread-stubs0-dev
#   libqscintilla2-5 libqt4-assistant libqt4-dbus libqt4-designer libqt4-help libqt4-network libqt4-script libqt4-scripttools libqt4-sql
#   libqt4-sql-mysql libqt4-svg libqt4-test libqt4-webkit libqt4-xml libqt4-xmlpatterns libqtcore4 libqtgui4 libschroedinger-1.0-0 libsm-dev
#   libstdc++6-4.4-dev libswscale0 libumfpack5.4.0 libwxbase2.8-0 libwxgtk2.8-0 libx11-dev libxau-dev libxcb1-dev libxdmcp-dev libxext-dev
#   libxi-dev mesa-common-dev mysql-common patch python-all python-dateutil python-dev python-docutils python-enable python-foolscap
#   python-jinja2 python-matplotlib-data python-pygments python-pyparsing python-qt4 python-roman python-sip python-tk python-tz tcl8.5 tk8.5
#   ttf-lyx x11proto-core-dev x11proto-input-dev x11proto-kb-dev x11proto-record-dev x11proto-xext-dev xtrans-dev
# Vorgeschlagene Pakete:
#   blt-demo g++-multilib g++-4.4-multilib gcc-4.4-doc libstdc++6-4.4-dbg git-doc git-arch git-cvs git-svn git-email git-daemon-run git-gui
#   gitk gitweb doxygen apache httpd id-utils python-profiler nas libqt4-dev qt4-qtconfig libstdc++6-4.4-doc openmpi-bin lam-runtime
#   libvtk5-dev vtk-examples vtk-doc libgnomeprintui2.2-0 diffutils-doc texlive-latex-recommended texlive-latex-base texlive-lang-french
#   python-jinja2-doc python-lxml-dbg dvipng python-excelerator python-matplotlib-doc texlive-extra-utils texlive-latex-extra python-qt3
#   python-numpy-doc python-numpy-dbg python-nose python-chardet python-qt4-dbg jsmath python-netcdf tix python-tk-dbg wx2.8-doc
#   wx2.8-examples python-wxtools ruby tcsh csh octave3.0 mksh pdksh swig-examples swig-doc tclreadline
# Die folgenden NEUEN Pakete werden installiert:
#   blt freeglut3 freeglut3-dev g++ g++-4.4 git-core global glutg3 glutg3-dev ipython libamd2.2.0 libaudio2 libavcodec52 libavformat52
#   libavutil49 libblas3gf libdigest-sha1-perl libdrm-dev liberror-perl libgfortran3 libgl1-mesa-dev libgl2ps0 libglu1-mesa-dev libgsm1
# # #   libhdf5-serial-1.8.4 libibverbs1 libice-dev liblapack3gf libmng1 libmysqlclient16 libnuma1 libopenmpi1.3 libphonon4 libpthread-stubs0
#   libpthread-stubs0-dev libqscintilla2-5 libqt4-assistant libqt4-dbus libqt4-designer libqt4-help libqt4-network libqt4-script
#   libqt4-scripttools libqt4-sql libqt4-sql-mysql libqt4-svg libqt4-test libqt4-webkit libqt4-xml libqt4-xmlpatterns libqtcore4 libqtgui4
# #   libschroedinger-1.0-0 libsm-dev libstdc++6-4.4-dev libswscale0 libumfpack5.4.0 libvtk5.2 libwxbase2.8-0 libwxgtk2.8-0 libx11-dev
#   libxau-dev libxcb1-dev libxdmcp-dev libxext-dev libxi-dev libxt-dev libxtst-dev mayavi2 mesa-common-dev mysql-common patch
#   pyqt4-dev-tools python-all python-all-dev python-apptools python-chaco python-configobj python-dateutil python-dev python-docutils
#   python-enable python-enthought-traits python-enthought-traits-ui python-enthoughtbase python-envisagecore python-envisageplugins
#   python-foolscap python-h5py python-jinja2 python-lxml python-matplotlib python-matplotlib-data python-numpy python-pip python-pygments
#   python-pyparsing python-pyrex python-qscintilla2 python-qt4 python-roman python-scipy python-setuptools python-sip python-sphinx
#   python-tables python-tables-doc python-tk python-traits python-traitsbackendqt python-traitsbackendwx python-traitsgui python-tz
#   python-vtk python-wxgtk2.8 python-wxversion python-xlib python2.6-dev scons swig tcl8.5 tk8.5 ttf-lyx x11proto-core-dev
#   x11proto-input-dev x11proto-kb-dev x11proto-record-dev x11proto-xext-dev xtrans-dev
# Die folgenden Pakete werden aktualisiert:
#   libgl1-mesa-glx libglu1-mesa
# # 2 aktualisiert, 129 neu installiert, 0 zu entfernen und 237 nicht aktualisiert.
# Es müssen 163MB an Archiven heruntergeladen werden.
# Nach dieser Operation werden 469MB Plattenplatz zusätzlich benutzt.
# Möchten Sie fortfahren [J/n]? 


