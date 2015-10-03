wget -c ftp://ftp.inrialpes.fr/pub/lear/douze/data/INRIAPerson.tar \
     -P dataset
tar xvf dataset/INRIAPerson.tar -C dataset

chmod +w dataset/INRIAPerson/Train/neg
chmod +w dataset/INRIAPerson/Test/neg

mkdir dataset/INRIAPerson/negative
mv dataset/INRIAPerson/Test/neg/* dataset/INRIAPerson/Train/neg/* \
   dataset/INRIAPerson/negative

for f in dataset/INRIAPerson/negative/*.png
do
    convert $f $(echo $f | sed -e 's/png/jpg/g')
done
