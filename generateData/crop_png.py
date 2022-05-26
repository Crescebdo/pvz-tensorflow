# Importing Image class from PIL module
from PIL import Image
import os
from random import randrange
from numpy import random

outpath = r"C:\Users\Chenz\Desktop\tsf\out"
saved = [75]*20
types = [-1,
         0,
         1,
         3,
         4,
         5,
         6,
         7,
         8,
         10,
         13,
         17,
         18,
         21,
         22,
         28,
         29,
         31,
         34,
         37,
         ]


def get_idx_from_code(code):
    for i in range(20):
        if types[i] == code:
            return i
    return -1


def read_txt(filepath):
    with open(filepath) as f:
        arr = []
        lines = f.readlines()
        for line in lines:
            row = []
            for token in line.split(","):
                row.append(int(token.strip()))
            arr.append(row)
        print(arr)
        return arr


def resize(im, width, height):
    res = randrange(3)
    if res == 0:
        return im
    ratio = (random.normal() / 4.0) + 1
    while (ratio < 1 and res == 1) or (ratio > 1 and res == 2) or (ratio <= 0.25):
        ratio = (random.normal() / 4.0) + 1
    newsize = (int(width*ratio), int(height*ratio))
    mode = randrange(3)
    if mode == 0:
        im1 = im.resize(newsize, Image.Resampling.NEAREST)
    elif mode == 1:
        im1 = im.resize(newsize, Image.Resampling.BILINEAR)
    else:
        im1 = im.resize(newsize, Image.Resampling.BICUBIC)
    return im1


def convert(filepath):
    if "txt" in filepath:
        return
    newpath = filepath[:filepath.rfind(".")+1]
    print(newpath + "txt")
    code = read_txt(newpath + "txt")
    newpath = outpath + "\\"

    # Opens a image in RGB mode
    im = Image.open(filepath)

    width, height = 80, 100
    rowN, colN = 5, 5
    for r in range(rowN):
        for c in range(colN):
            box = (c * width, r * height, c * width+width, r * height+height)
          #  print(box)
            crop = im.crop(box)
            idx = get_idx_from_code(code[r][c])
            p = newpath + str(code[r][c])
           # print(p)
            if not os.path.exists(p):
                os.mkdir(p)
            p += "\\" + str(saved[idx]) + ".bmp"
          #  print(p)
            crop = resize(crop, width, height)
            crop.save(p, "bmp")
            saved[idx] += 1


# Get all subdirectories
directory = r"C:\Users\Chenz\Desktop\tsf\3"
for dirpath, _, filenames in os.walk(directory):
    for f in filenames:
        convert(os.path.abspath(os.path.join(dirpath, f)))
