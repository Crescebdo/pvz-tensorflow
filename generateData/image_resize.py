# Importing Image class from PIL module
from PIL import Image
import os
from random import randrange
from numpy import random

def resize(path, res):
    if res == 0:
        return
    
    # Opens a image in RGB mode
    im = Image.open(path)

    # Size of the image in pixels (size of original image)
    # (This is not mandatory)
    width, height = im.size

    # Determine target size
    ratio = (random.normal() / 4.0) + 1
    while (ratio < 1 and res == 1) or (ratio > 1 and res == 2) or (ratio <= 0.25):
        ratio = (random.normal() / 4.0) + 1

    # Resize
    newsize = (int(width*ratio), int(height*ratio))
    mode = randrange(3)
    if mode == 0:
        im1 = im.resize(newsize, Image.Resampling.NEAREST)
    elif mode == 1:
        im1 = im.resize(newsize, Image.Resampling.BILINEAR)
    else:
        im1 = im.resize(newsize, Image.Resampling.BICUBIC)
    
    im1.save(path)


# Get all subdirectories
directory = r"C:\Users\Chenz\Desktop\data2"
for dirpath,_,filenames in os.walk(directory):
        for f in filenames:
            resize(os.path.abspath(os.path.join(dirpath, f)), randrange(3))

