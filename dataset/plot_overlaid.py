# Image viewer with overlay points

# Copyright (C) 2015 Ishita Takeshi

# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.


import sys
import csv

from scipy.misc import imread
import numpy as np

from util import plot_overlaid


if(len(sys.argv) <= 2):
    print("Usage: "
          "$python3 plot_overlaid.py <path_to_image> <path_to_annotations>")
    exit(0)


path_to_image = sys.argv[1]
path_to_annotations = sys.argv[2]

image = imread(path_to_image)

reader = csv.reader(open(path_to_annotations))
annotations = [list(map(float, row)) for row in reader]
annotations = np.array(annotations)

plot_overlaid(image, annotations)
