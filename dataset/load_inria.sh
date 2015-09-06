wget -c ftp://ftp.inrialpes.fr/pub/lear/douze/data/INRIAPerson.tar
tar xvf INRIAPerson.tar

chmod +w INRIAPerson/Train/neg
chmod +w INRIAPerson/Test/neg

mkdir INRIAPerson/negative
mv INRIAPerson/Test/neg/* INRIAPerson/Train/neg/* INRIAPerson/negative

for f in INRIAPerson/negative/*.png
do
    convert $f $(echo $f | sed -e 's/png/jpg/g')
done
