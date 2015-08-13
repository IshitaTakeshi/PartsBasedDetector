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


pkg load image;


function ensure_parent_is_smaller_than_child(pa)
  for i = 1:length(pa)
    child = i;
    parent = pa(child);
    assert(parent < child);
  end
end


% --------------------
% specify model parameters
% number of mixtures for 6 parts
% See "Structure" section in http://www.ics.uci.edu/~dramanan/papers/pose2011.pdf, page 5, for scholarly reference.
K = [4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4];


% Tree structure for parts: pa(i) is the parent of part i
% This structure must correspond to the part labels passed to annotateParts().
% See "Deriving part type from position" section in http://www.ics.uci.edu/~dramanan/papers/pose2011.pdf, page 4, for scholarly reference.


% (PARSE_data.m) and evaluation (PARSE_eval_pcp)
pa =      [0 1 2 3 4 5 3 7 8  3 10 11 12 10 14 15];
%children [1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16];
ensure_parent_is_smaller_than_child(pa);


% Spatial resolution of HOG cell, interms of pixel width and hieght
sbin = 8;


root = fileparts(mfilename('fullpath'));
addpath(fullfile(root, 'learning'));


% --------------------
% Define training and testing data
name = 'demo_model';
[pos test] = getPositiveData('../dataset/animate-reshape-small/positive',
                             '.png', '.txt', 0.7);
neg        = getNegativeData('../dataset/animate-reshape-small/negative',
                             '.jpg');
pos        = pointtobox(pos,pa);


% --------------------
% training
model = trainmodel(name,pos,neg,K,pa,sbin);
save('animate_reshape.mat', '-mat-binary', 'model', 'pa', 'sbin', 'name');
