% Copyright (C) 2015 Ishita Takeshi
%
% This program is free software: you can redistribute it and/or modify
% it under the terms of the GNU General Public License as published by
% the Free Software Foundation, either version 3 of the License, or
% (at your option) any later version.
%
% This program is distributed in the hope that it will be useful,
% but WITHOUT ANY WARRANTY; without even the implied warranty of
% MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
% GNU General Public License for more details.
%
% You should have received a copy of the GNU General Public License
% along with this program.  If not, see <http://www.gnu.org/licenses/>.


% Preparing animate-reshape dataset
%
% Images will be cropped and stored into
% <the path to this file>/animate-reshape/cropped with their annotations.
%
% The left upper corner of the cropped image is regarded as the origin of
% coordinates.
%
%Generated images and their annotations can be seen by the command below:
%$python3 plot_overlaid.py <path_to_image> <path_to_annotations>


pkg load image;


%change the order of annotations since the trainer doesn't allow the default
function annotations = change_order(annotations)
  annotations = annotations(
    :, [13 3 2 1 4 5 6 9 8 7 10 11 12 14]
  );
end


function download(url, directory)
  if(nargin < 2)
    directory = '.'
  end
  system(sprintf('wget %s -P %s', url, directory));
end


function prepare_positive(positive_dir)
  if not(exist(fullfile(pwd, 'lsp_dataset.zip')))
    download('http://www.comp.leeds.ac.uk/mat4saj/lsp_dataset.zip');
  end

  unzip('lsp_dataset.zip');

  load(fullfile('lsp_dataset', 'joints.mat'));

  movefile(fullfile('lsp_dataset', 'images'), positive_dir);

  for i = 1:length(joints(:, :, :))
    annotations = joints(1:2, :, i);
    annotations = change_order(annotations);

    path = fullfile(positive_dir, sprintf('im%04d.txt', i));
    csvwrite(path, annotations');
  end
end


function prepare_negative(negative_dir)
  if not(exist(fullfile(pwd, 'people.zip')))
    download('http://www.ics.uci.edu/~dramanan/papers/parse/people.zip');
  end

  unzip('people.zip');

  if not(exist(negative_dir))
    movefile('people_all', negative_dir);
  end
end


dataset_root = 'lsp_dataset';

if not(exist(dataset_root))
  mkdir(dataset_root);
end

positive_dir = fullfile(dataset_root, 'positive');
negative_dir = fullfile(dataset_root, 'negative');

prepare_positive(positive_dir);
prepare_negative(negative_dir);
