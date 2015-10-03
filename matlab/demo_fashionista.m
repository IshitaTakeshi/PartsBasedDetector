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
K = [4 4 4 4 4 4 4 4 4 4 4 4 4 4];


% Tree structure for parts: pa(i) is the parent of part i
% This structure must correspond to the part labels passed to annotateParts().
% See "Deriving part type from position" section in http://www.ics.uci.edu/~dramanan/papers/pose2011.pdf, page 4, for scholarly reference.


% (PARSE_data.m) and evaluation (PARSE_eval_pcp)
pa =      [0 1 2 3 1 5 6 1 8  9  1 11 12  1];
%children [1 2 3 4 5 6 7 8 9 10 11 12 13 14];
ensure_parent_is_smaller_than_child(pa);


% Spatial resolution of HOG cell, interms of pixel width and hieght
sbin = 8;


root = fileparts(mfilename('fullpath'));
addpath(fullfile(root, 'learning'));


% --------------------
% Define training and testing data
name = 'fashionista';
[pos test] = getPositiveData('../dataset/dataset/fashionista',
                             '.jpg', '.txt', 1.0);
neg        = getNegativeData('../dataset/dataset/INRIAPerson/negative', '.jpg');
pos        = pointtobox(pos,pa);


% --------------------
% training
model = trainmodel(name,pos,neg,K,pa,sbin);
save('fashionista.mat', '-mat-binary', 'model', 'pa', 'sbin', 'name');
