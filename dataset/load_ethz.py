from glob import glob
import os
from os.path import exists, basename, join, abspath, dirname
import csv
from subprocess import call
import shutil

import numpy as np
from scipy.misc import imread, imsave

from util import download, untar, unzip, plot_overlaid


dataset_root = join(dirname(abspath(__file__)), 'ETHZ')

if not(exists(dataset_root)):
    os.makedirs(dataset_root)


class ETHZDatasetPreparer(object):
    def __init__(self):
        url = \
            'http://groups.inf.ed.ac.uk/calvin/ethz_pascal_stickmen/'\
            'downloads/ETHZ_PASCAL_Stickmen_v1.11.tgz'
        data_filename = basename(url)
        self.data_filepath = join(dataset_root, data_filename)
        data_directory = join(dataset_root, 'ETHZ_PASCAL_Stickmen_v1.11')
        self.loaded = False

        if not(exists(self.data_filepath)):
            download(url, self.data_filepath)

        if not(exists(data_directory)):
            untar(self.data_filepath, dataset_root)

            shutil.move(join(data_directory, 'images'),
                        join(dataset_root, 'images'))

            shutil.move(join(data_directory, 'data'),
                        join(dataset_root, 'annotations'))

    def load(self):
        filenames = []

        # the coordinates of body parts in an image
        # should be represented like below
        # [x1 y1]
        # [x2 y2]
        # ...
        # [x6 y6]

        coordinates = []
        images = []
        image_path_template = join(dataset_root, 'images', '{:>06}')

        t = join(dataset_root, 'annotations', 'pascal_sticks.txt')
        for row in csv.reader(open(t, 'r'), delimiter=' '):
            if(len(row) == 1):
                n_rows = 0
                points = []
                image_path = image_path_template.format(row[0])
                images.append(imread(image_path))
                continue

            points += list(map(float, row[1:5]))

            n_rows += 1
            if(n_rows == 6):
                points = np.array(points)
                coordinates.append(points.reshape(12, 2))

        self.coordinates = coordinates
        self.images = images
        self.loaded = True

    def export(self, dataset_directory):
        if not(self.loaded):
            raise AssertionError('Data not loaded.')

        if not(exists(dataset_directory)):
            os.makedirs(dataset_directory)

        for i in range(len(self.images)):
            filename = '{:>06}.jpg'.format(i)
            path = join(dataset_directory, filename)
            imsave(path, self.images[i])

            filename = '{:>06}.txt'.format(i)
            path = join(dataset_directory, filename)
            writer = csv.writer(open(path, 'w'))
            writer.writerows(self.coordinates[i].tolist())

    def plot_overlaid(self, index):
        if(index >= len(self.images)):
            m = "Object contains only {} images. "\
                "Index too large.".format(n_images)
            raise ValueError(m)

        plot_overlaid(self.images[index], self.coordinates[index])


def prepare_negative_dataset(dataset_directory):
    negative_dataset_url = \
        'http://www.ics.uci.edu/~dramanan/papers/parse/people.zip'
    data_filepath = join(dataset_root, basename(negative_dataset_url))
    if not(exists(data_filepath)):
        download(negative_dataset_url, path=data_filepath)
    unzip(data_filepath, dataset_root)

    if not(exists(dataset_directory)):
        shutil.move(join(dataset_root, 'people_all'), dataset_directory)


loader = ETHZDatasetPreparer()
loader.load()

#make sure points in a file indicate correct joint coordinates
loader.plot_overlaid(10)

dataset_directory = join(dataset_root, 'positive')
loader.export(dataset_directory=dataset_directory)

dataset_directory = join(dataset_root, 'negative')
prepare_negative_dataset(dataset_directory=dataset_directory)
