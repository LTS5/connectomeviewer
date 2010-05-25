""" Encapsulates validator functions for connectome files """

def validate_metaxml(metaxml_string):
    """ Validate the meta.xml given as string using the
    connectome.xsd schema definition """
    
    from lxml import etree
    file1 = open('connectome.xsd', 'r')
    xmlschema_doc = etree.parse(file1)
    file1.close()
    
    xmlschema = etree.XMLSchema(xmlschema_doc)
    doc = etree.parse(metaxml_string)
    # validate and in case, throw an exception
    xmlschema.assert_(doc)

    return True