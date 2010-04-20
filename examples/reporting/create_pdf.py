#!/usr/bin/python

from reportlab.pdfgen import canvas
import tempfile
a=tempfile.NamedTemporaryFile(suffix='.jpg')
cfile.networks[0].rendermanager.scene3d.scene.save_jpg(a.name)
c = canvas.Canvas("hello.pdf")
c.drawInlineImage(a.name, 0,0)
c.showPage()
c.save()

## {{{ Recipe 576717 (r1): PDF a Directory of Images using Reportlab 
import os
from reportlab.pdfgen import canvas
from reportlab.lib.pagesizes import letter
from reportlab.lib.units import cm, mm, inch, pica

def pdfDirectory(imageDirectory, outputPDFName):
    dirim = str(imageDirectory)
    output = str(outputPDFName)
    width, height = letter
    height, width = letter
    c = canvas.Canvas(output, pagesize=letter)
    try:
        for root, dirs, files in os.walk(dirim):
            for name in files:
                lname = name.lower()
                if lname.endswith(".jpg") or lname.endswith(".gif") or lname.endswith(".png"):
                    filepath = os.path.join(root, name)
                    c.drawImage(filepath, inch, inch * 1)
                    c.showPage()
                    c.save()
        print "PDF of Image directory created"
    except:
        print "Failed creating PDF"
## End of recipe 576717 }}}

# pdfDirectory('.', 'test.pdf')
