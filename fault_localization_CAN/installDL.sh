#!/bin/sh

export subject_dir=nanoxml_v5
export version=3
echo copying to coverage_info
cp -r ${subject_dir}/result/v${version}/componentinfo.txt Coverage_Info
cp -r ${subject_dir}/result/v${version}/covMatrix.txt Coverage_Info
cp -r ${subject_dir}/result/v${version}/error.txt Coverage_Info


echo excuting python 1-2.py
python CAN.py dev

echo moving DL_result.txt to result
mv Coverage_Info/DL_result.txt ${subject_dir}/result/v${version}

echo compiling DL.c
gcc -o ${subject_dir}/DL ${subject_dir}/analysePro/DL.c

cd ${subject_dir}
echo excuting DL.c
./DL result/v${version}/componentinfo.txt result/v${version}/DL_result.txt
rm -f DL
echo moving DeepLearning.txt to result
mv DeepLearning.txt result/v${version}/DL_result
echo excuting sliceDL.c
gcc -o sliceDL analysePro/sliceDL.c
./sliceDL result/v${version}/componentinfo.txt result/v${version}/DL_result.txt sliceResult/v${version}/sliceResult.txt
rm -f sliceDL
echo moving SliceDeepLearning.txt to result
mv SliceDeepLearning.txt result/v${version}/DL_result
cd ..

echo get final result
cp -r analysePro/translate translate
chmod u+x translate
./translate ${subject_dir}/result/v${version}/DL_result/DeepLearning.txt ${subject_dir}/result/v${version}/DL_result/SliceDeepLearning.txt
rm -f translate

rm -f Coverage_Info/componentinfo.txt 
rm -f Coverage_Info/covMatrix.txt 
rm -f Coverage_Info/error.txt 

