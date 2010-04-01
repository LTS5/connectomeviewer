#!/usr/bin/env python

# just displaying a slice of brain tissue from brainmaps.org using matplotlib.

import matplotlib.pyplot as plt
import Image
im = Image.open("1251.jpg")
im2 = plt.imshow(im)
plt.show()
