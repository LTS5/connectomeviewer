#!/usr/bin/env python

import os, sys, shutil, os.path
from distutils.sysconfig import get_python_lib
pjoin = os.path.join

def mkshortcut(target,description,link_file,*args,**kw):
    """make a shortcut if it doesn't exist, and register its creation"""
    
    create_shortcut(target, description, link_file,*args,**kw)
    file_created(link_file)
    
def install():
    """Routine to be run by the win32 installer with the -install switch."""

    # Get some system constants
    prefix = sys.prefix
    python = pjoin(prefix, 'python.exe')

    from win32com.shell import shell
    from win32com.shell import shellcon
    
    # copy the libraries to system32 dir
    startmenu = shell.SHGetSpecialFolderPath(0, shellcon.CSIDL_COMMON_STARTMENU)
    if startmenu == '':
        startmenu = shell.SHGetSpecialFolderPath(0, shellcon.CSIDL_STARTMENU)
    #startmenu = get_special_folder_path('CSIDL_COMMON_STARTMENU')
    #if startmenu == '':
	#startmenu = get_special_folder_path('CSIDL_STARTMENU')

    if not startmenu == '':
	cv_start_menu = pjoin(startmenu, 'Connectome')
	print "Adding Startmenu link ..."
      
    	if not os.path.isdir(cv_start_menu):
	        os.mkdir(cv_start_menu)
        	directory_created(cv_start_menu)
        	print "done."
    
    # Create .py and .bat files to make things available from
    # the Windows command line.  Thanks to the Twisted project
    # for this logic! (from IPython windows post installer)
    programs = [
        'connectomeviewer.py',
    ]
    scripts = pjoin(prefix,'scripts')
    for program in programs:
        raw = pjoin(scripts, program)
        bat = raw + '.bat'
        py = raw + '.py'
        # Create .py versions of the scripts
        shutil.copy(raw, py)
        # Create .bat files for each of the scripts
        bat_file = file(bat,'w')
        bat_file.write("@%s %s %%*" % (python, py))
        bat_file.close()
            
    # Now move onto setting the Start Menu up
    cviewer = pjoin(scripts, 'connectomeviewer.py')
    print "Creating Shortcut"
    link = pjoin(cv_start_menu, 'ConnectomeViewer.lnk')
    cmd = '"%s"' % cviewer
    mkshortcut(python,'ConnectomeViewer BETA',link,cmd)
    print "Done."
    #import win32api
    #lib_dir = win32api.GetSystemDirectory()
    lib_dir = shell.SHGetSpecialFolderPath(0, shellcon.CSIDL_SYSTEM)
    pref = get_python_lib()
        
    if not os.path.isfile(pjoin(lib_dir, 'libexpat.dll')):
        print "Try to copy libexpat.dll to sytem folder..."
        shutil.copy(pjoin(pref, 'cviewer\io\gifti\win32', 'libexpat.dll'), lib_dir)
        print "...succeeded!"

    if not os.path.isfile(pjoin(lib_dir, 'libgiftiio.so')):
        print "Try to copy libgiftiio.so to sytem folder"
        shutil.copy(pjoin(pref, 'cviewer\io\gifti\win32', 'libgiftiio.so'), lib_dir)
        print "...succeeded!"

    if not os.path.isfile(pjoin(lib_dir, 'zlib1.dll')):
        print "Try to copy zlib1.dll to sytem folder"
        shutil.copy(pjoin(pref, 'cviewer\io\gifti\win32', 'zlib1.dll'), lib_dir)
        print "...succeeded!"
        
    
def remove():
    """Routine to be run by the win32 installer with the -remove switch."""
    pass

# main()
if len(sys.argv) > 1:
    if sys.argv[1] == '-install':
        install()
    elif sys.argv[1] == '-remove':
        remove()
    else:
        print "Script was called with option %s" % sys.argv[1]
