# ADIP-project
A beautiful school project based on objects recognition using OpenCV API.

# How to train a custom model

Inside the training folder, there are 4 folders which correspond to the 4 items we have to find. To train one of these items, the following commands are needed:

```bash
$ cd training
$ cd <item>
$ opencv_createsamples -info positives.txt -num <number_of_samples> -vec training.vec -show -w 60 -h 60
$ mv training.vec ../
$ cd ..
$ opencv_traincascade -data data -vec training.vec -bg negatives.txt -numPos <number_of_samples> -numNeg 47 -numStages 10 -featureType HAAR -minhitrate 0.999 -maxfalsealarm 0.5 -w 60 -h 60
$ cd data
$ mv cascade.xml <item>.xml
```
\<item> corresponds to the name of the item you want to train.

\<number_of_samples> corresponds to the number of samples used inside positives.txt file (not the number of .jpg but the number of objects detected inside those .jpg).

To use the new trained model, you have to move \<item>.xml inside the models folder which is at the root of the project.
