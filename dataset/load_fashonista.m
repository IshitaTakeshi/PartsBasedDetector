%change the order of annotations since the trainer doesn't allow the default
function annotations = change_order(annotations)
  annotations = annotations(
    [13, 9, 8, 7, 10, 11, 12, 3, 2, 1, 4, 5, 6, 14], :
  );
end

url = ['http://vision.is.tohoku.ac.jp/'...
       '~kyamagu/research/paperdoll/fashionista-v0.2.1.tgz'];

system(sprintf('wget -c %s', url));
system('tar xvf fashionista-v0.2.1.tgz');

load('fashionista_v0.2.mat');

data_dir = 'fashionista';
mkdir(data_dir);

for i = 1:length(truths)
  truth = truths(i);

  filename = sprintf('image%d.jpg', i);
  path = fullfile(data_dir, filename);
  command = sprintf('wget -c %s -O %s', truth.url, path);
  system(command);
  s = dir(path);
  if s.bytes == 0
    % empty file
    delete(path);
    continue;
  end

  point = change_order(truth.pose.point);
  filename = sprintf('annotation%d.txt', i);
  path = fullfile(data_dir, filename);
  csvwrite(path, point);

  %prediction = predictions(i);
  %point = change_order(prediction.pose.point);
  %filename = sprintf('prediction%d.txt', i);
  %path = fullfile(data_dir, filename);
  %csvwrite(path, point);
end
