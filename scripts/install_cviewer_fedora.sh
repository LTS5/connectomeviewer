su

yum remove python-AppTools python-EnvisageCore python-EnvisagePlugins python-Traits python-TraitsBackendQt python-TraitsGUI

yum update python
yum install python-pip
yum install numpy scipy python-devel ipython
pip install ETSProjectTools
ets co ETS
yum install vtk vtk-devel swig python-configobj libXtst libXtst-devel freeglut freeglut-devel python-matplotlib python-lxml Cython qscintilla-python python-sphinx wxPython wxPython-devel subversion gcc gcc-c++ libXt libXt-devel libX11-devel

ets install
pip install networkx

wget http://unidesign.ch/neuron/ConnectomeViewer-0.1.6.tar.gz
tar xzf ConnectomeViewer-0.1.6.tar.gz
cd ConnectomeViewer-0.1.6/
python setup.py install
