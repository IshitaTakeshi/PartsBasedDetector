import matplotlib
matplotlib.use('Qt4Agg')
matplotlib.rcParams['backend.qt4'] = 'PySide'
import matplotlib.pyplot as plt


def plot_overlaid(image, coordinates):
    plt.imshow(image)
    plt.scatter(*(coordinates.T))
    for i, c in enumerate(coordinates):
        plt.annotate((i+1, c.tolist()), c, color='white')
    plt.show()


def download(url, path):
    from urllib import request
    response = request.urlopen(url)
    content = response.read()
    open(path, 'wb').write(content)


def untar(data_filepath, directory_extract_to):
    import tarfile
    tar = tarfile.open(data_filepath, 'r')
    tar.extractall(path=directory_extract_to)
    tar.close()


def unzip(data_filepath, directory_extract_to):
    from zipfile import ZipFile
    zip_ = ZipFile(data_filepath, 'r')
    zip_.extractall(path=directory_extract_to)
    zip_.close()
