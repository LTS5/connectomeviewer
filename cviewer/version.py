try:
    import pkg_resources
    version = pkg_resources.require('ConnectomeViewer')[0].version
except:
    version = ''