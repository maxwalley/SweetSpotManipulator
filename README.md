# SweetSpotManipulator
Dissertation project - An application that tracks a user and moves the sweet spot of an audio system to their location

This project was my first full application in JUCE and was created for my dissertation.

A user is tracked through the use of an Microsoft Kinect. Implementation for this was built through the OpenCV and libfreenect libraries. The application takes the colour image from the Kinect sensor. This image is run through a haar cascade maching learning detection algorithm set to detect humans. Once a detection occurs the detected location on the colour image is cross referenced with the same location on the Kinect's depth detection image. Trigonometry in relation to the Kinect's field of view is then used to calculate the detected locations distance from the loudspeakers the system has been configured for. The locations of these loudspeakers are input by the user through the applications GUI. With these calculations completed amplitude and delay manipulation is performed on each loudspeakers input signal to account for the user position.
