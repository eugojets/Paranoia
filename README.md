# Paranoia
Have you ever thought about having your computer do something when it sees that your friends or people you don't know on it? Paranoia gives you a chance to do just that. Paranoia is an application that can recognize and detect faces and perform some action based on the detected/recognized faces. 

Currently, it is able to detect/recognize faces, save images of unauthorized users, greet people with phrases, and dim or brighten the screen.

It can be configured to do more things, and the API to do so is simple, so the actions it can take upon seeing a person is up to your imagination.

## Getting Started
Paranoia is currently only developed for Windows.

### Prerequisites
These are the 3rd party libraries that you will need
```
boost
opencv
opencv_contrib
rapidjson
spdlog
```
Make sure to use opencv 3.0 or later, and to build opencv with opencv_contrib.

### Running it
Paranoia takes a config.json file that specifies everything it needs in order to run. Here is an example config.json:

```javascript
{
   "haarFile":"", //path to a haarfile (used by opencv)
   "nestedHaarFile": "", // path to a nested haarfile (used by opencv)
   "intrudersFolder":"", // Name of the folder where you want to store pictures of people using your computer
   "authorizedUsers":["person1", "person2"], // an array of strings to specify people who are authorized to use your computer, these names should match the names in the knownFaces array
   "maxQueueSize":5, // the number of images that is stored in memory for each unauthorized person
   "delay":30, // the delay in seconds befor the next image is tored in the buffer
   "deviceId":0, // id of the camera
   "knownFaces":[
      {
         "name":"person1", // name to identify this person
         "trainingImages":"", // path to the folder containing training images for this person.
         "speeches":[
            "Hello, World",
         ] // an array of phrases that the Host will randomly pick from if the FaceRecognizer predicts this person to be on the screen
      },
   ] // an array of known faces that the FaceRecognizer will learn
}
```

### Training Images
The training images that are used to teach the FaceRecognizer should all be the same dimensions.

### FaceRecognizer
Simple callback that passes in a vector of recognized people on every frame.

### FaceDetector
2 callbacks:
```
OnFaceDetected 
OnNoFaceDetected
```
These callbacks are also avaialble on every frame
