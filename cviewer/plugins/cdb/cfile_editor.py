""" The view of the currently existing stored cfiles in the ConnectomeDB

"""
# Copyright (C) 2009-2010, Ecole Polytechnique Federale de Lausanne (EPFL) and
# University Hospital Center and University of Lausanne (UNIL-CHUV)
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# Standard library imports
import os.path
import sys
from threading import Thread

# Enthought library imports
from enthought.traits.api import HasTraits, HasStrictTraits, Str, Int, Regex, List, Bool, Any, Directory
from enthought.traits.ui.api import View, Group, Item, TableEditor,VGroup, HGroup, Handler
from enthought.traits.ui.table_column import ObjectColumn, ExpressionColumn
from enthought.traits.ui.table_filter import EvalFilterTemplate, MenuFilterTemplate, RuleFilterTemplate, EvalTableFilter
from enthought.traits.ui.extras.checkbox_column import CheckboxColumn
from enthought.traits.ui.menu import OKCancelButtons

# Logging imports
import logging
logger = logging.getLogger('root.'+__name__)

# a helper class for a cfile tuple
class CFileTuple(HasTraits):
    """ Wrapper classe for CFile tuples from the database """
    
    # modify when database schema changes
    id = Int
    download = Bool(False)
    name = Str
    legalnotice = Str
    reference = Str
    description = Str
    species = Str
    nrofnetworks = Int
    url = Str
    generator = Str
    created = Str
    modified = Str
    
# modify when database schema changes
table_editor = TableEditor(
    columns = [
                CheckboxColumn( name = 'download', label="Download?"),
                ObjectColumn( name = 'name', label="Name"),
                ObjectColumn( name = 'species', label="Species"),
                ObjectColumn( name = 'legalnotice', label="Legal notice"),
                ObjectColumn( name = 'nrofnetworks', label="# networks"),
              ],
    deletable   = True,   
    sort_model  = True,   
    #auto_size   = False,
    orientation = 'vertical',
    edit_view   = View(
                      Group( Item('name', style = 'readonly'),
                             Item('species', style = 'readonly'),
                             Item('description', style = 'custom'),
                             Item('legalnotice', style = 'readonly'),
                             Item('reference', style = 'readonly'),
                             Item('nrofnetworks', style = 'readonly'),
                             Item('url', style = 'readonly'),
                             Item('generator', style = 'readonly'),
                             Item('created', style = 'readonly'),
                             Item('modified', style = 'readonly'),
                             show_border = True
                      ),
                      height = 0.9,
                      resizable = True
                  ),
    filters     = [ EvalFilterTemplate, MenuFilterTemplate, RuleFilterTemplate ],
    search      = EvalTableFilter(),
    show_toolbar = True,
    row_factory = CFileTuple )

class ThreadedDownload(Thread):
    """ Download the cfiles in an other thread """
    
    def __init__(self, cfilecontainer, **kwargs):
        Thread.__init__(self, **kwargs)
        self.cfilecontainer = cfilecontainer

    def run(self):
        logger.debug('Stat downloading files from ConnectomeDatabase.')
        
        for cfile in self.cfilecontainer.cfiles:
            if cfile.download:
                logger.debug('Try to download cfile with ID' + str(cfile.id))
                # open temporary file
                name = cfile.species.lower()
                name = name.replace(' ', '_')
                fname = str(name) + '_' + str(cfile.id) + '.cff'
                path = self.cfilecontainer.savepath
                logger.debug(os.path.join(path, fname))
                url = self.cfilecontainer._get_cfile_url(id = cfile.id)
                import urllib
                try:
                    urllib.urlretrieve(url, os.path.join(path, fname))
                except Exception, e:
                    logger.debug("Error downloading %s: %s" % (url, e))
                logger.debug('Successfully saved file: ' + fname)

# learn from http://code.enthought.com/projects/traits/docs/html/TUIUG/handler.html#id12
# and https://svn.enthought.com/enthought/wiki/UnderstandingMVCAndTraitsUI

class InterfaceHandler(Handler):

    def close(self, info, is_ok):
        if is_ok:
            # download the files            
            download = ThreadedDownload(cfilecontainer = info.object)
            download.start()

            logger.info('ConnectomeDB Download Interface completed successfully.')
            # then close the window
            info.ui.dispose()
        else:
            logger.info('ConnectomeDB Download Interface cancelled.')
            # directly close the window
            info.ui.dispose()
        

class CFileContainer(HasTraits):
    """ A CFile container class to connect to the database and get cfiles """
    
    cfiles = List(CFileTuple)
    savepath = Directory
    conn = Any
    
    def _establish_connection(self, myhost, myuser, mypasswd, mydb):
        """ Connect to database
        
        Parameters
        ----------
        myhost : string
            hostname or IP address
        myuser : string
            username with read access
        mypasswd : string
            password for user
        mydb : string
            name of the database
            
        """
        
        import pymysql
        #port: 3306

        #conn = pymysql.connect(host='localhost', port=3306, user='root', unix_socket='/tmp/mysql.sock', passwd='', db='cdb')
        # FIXME: this should be wrapped in a try-except, but
        # the documentation says nothing about the exceptions
        self.conn = pymysql.connect(host = myhost, user = myuser, \
                                        passwd = mypasswd , db = mydb)

        return True
            
    def _set_cfiles_list(self):
        """ Setting the cfiles list """
        import pymysql
        
        cfilelist = []

        cursor = self.conn.cursor()

        cursor.execute ("SELECT id,name,species,legalnotice,reference,description,nrofnetworks,url,generator,created,modified FROM cff_index")
    
        result_set = cursor.fetchall ()
        
        for row in result_set:
            
            if row[4] is None:
                refer = ''
            else:
                refer = row[4]
                
            if row[7] is None:
                urli = ''
            else:
                urli = row[7]
            
            if row[3] is None:
                legnot = ''
            else:
                legnot = row[3]
            
            # modify when database schema changes
            tmptuple = CFileTuple(
                id = int(row[0]),
                name = row[1],
                species = row[2],
                legalnotice = legnot,
                reference = refer,
                description = row[5],
                nrofnetworks = int(row[6]),
                url = urli,
                generator = row[8],
                created = row[9].isoformat(),
                modified = row[10].isoformat()
            )
            cfilelist.append(tmptuple)
      
        cursor.close ()
        
        self.cfiles = cfilelist
    
    def _close_connection(self):
        """ Closes the SQL connection """
        
        self.conn.close ()
    
    def _get_cfile_url(self, id):
        """ Get a CFF file url from the database
        
        Parameters
        ----------
        id : int
            Internal id from the database
        
        """
        
        cursor = self.conn.cursor ()
        cursor.execute ("SELECT cfile_url FROM cff_index WHERE id = " + str(id))
    
        result_set = cursor.fetchone()

        cursor.close ()
        
        return result_set[0]
        

    def default_traits_view(self):

        traits_view = View(
            Group(Item( 'cfiles',
                                show_label  = False,
                                editor      = table_editor 
                ),
                show_border = True,
            ),
            width     = 710,
            height    = 280,
            x         = 50,
            y         = 200,
            resizable = True,  
            buttons   = ['OK', 'Cancel'],
            handler   = InterfaceHandler(),
            kind      = 'nonmodal',
        )
        return traits_view
