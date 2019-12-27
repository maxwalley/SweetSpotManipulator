/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent() : AudioAppComponent(UserSelectedDevice), UserSelectedDeviceSettings(UserSelectedDevice, 0, 0, 0, 6, false, false, false, false), channel1Multiplier(0), channel2Multiplier(0), channel1State(up), channel1SampleCount(0), channel2State(up), channel2SampleCount(0), kinUpButton("Tilt Up"), kinDownButton("Tilt Down"), CVWindowButton("Open CV View"), closeCVWindow("Close CV"), kinectTestButton("Kin Test"), userPosX(329)
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (1280, 800);
    
    addAndMakeVisible(masterSlider);
    masterSlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
    masterSlider.setRange(0, 1);
    masterSlider.setValue(0);
    masterSliderValue = masterSlider.getValue();
    masterSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, false, 50, 20);
    masterSlider.addListener(this);
    addAndMakeVisible(masterSliderLabel);
    masterSliderLabel.setText("Master", dontSendNotification);
    
    addAndMakeVisible(Lights);
    
    addAndMakeVisible(CVWindowButton);
    CVWindowButton.addListener(this);
    
    addAndMakeVisible(closeCVWindow);
    closeCVWindow.addListener(this);
    
    addAndMakeVisible(balance);
        
    addAndMakeVisible(UserSelectedDeviceSettings);
    UserSelectedDevice.initialise(0, 6, nullptr, false);
    
    addAndMakeVisible(audioPlayer);

    // Some platforms require permissions to open input channels so request that here
    if (RuntimePermissions::isRequired (RuntimePermissions::recordAudio)
        && ! RuntimePermissions::isGranted (RuntimePermissions::recordAudio))
    {
        RuntimePermissions::request (RuntimePermissions::recordAudio,
                                     [&] (bool granted) { if (granted)  setAudioChannels (0, 6); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels (0, 6);
    }
    
    addAndMakeVisible(audioOutSelector);
    audioOutSelector.addItem("Square Wave", 1);
    audioOutSelector.addItem("Audio Player", 2);
    audioOutSelector.setSelectedId(1);
    audioOutSelector.setEditableText(false);
    
    addAndMakeVisible(kinectImage);
    kinectImage.kinectInit();
    
    addAndMakeVisible(minThresSlider);
    minThresSlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
    minThresSlider.setRange(0, 1000);
    minThresSlider.setValue(100);
    addAndMakeVisible(maxThresSlider);
    maxThresSlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
    maxThresSlider.setRange(0, 1000);
    maxThresSlider.setValue(200);
    
    addAndMakeVisible(kinectTestButton);
    kinectTestButton.addListener(this);
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    AlertWindow::showOkCancelBox(AlertWindow::AlertIconType::WarningIcon, "Close App?", "Are you sure you want to close this app?", "Continue", "Cancel", nullptr, nullptr);
    
    kinectImage.kinectEnd();
    
    shutdownAudio();
    
    cv::destroyAllWindows();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    printf("Sample Rate is: %f\n", sampleRate);
    printf("Block Size is %d\n", samplesPerBlockExpected);
    
    currentSampleRate = sampleRate;
    
    audioPlayer.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void MainComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    //Gets audio player data
    audioPlayer.getNextAudioBlock(bufferToFill);
    
    //Iterates through the channels
    for(int channel = 0; channel < bufferToFill.buffer->getNumChannels(); channel++)
    {
        //Creates Pointer to the first sample of the selected channel
        float* buffer = bufferToFill.buffer->getWritePointer(channel, bufferToFill.startSample);
    
        //Iterates through the samples
        for(int sample = 0; sample < bufferToFill.numSamples; ++sample)
        {
            //Applies balance mutiplier
            buffer[sample] = buffer[sample] * balance.getListenerDistance(channel, userPosX, depthAtUserPos);
        }
    }
    
    //Work out at a later date
    /*for (int i = getNumInputChannels(); i < getNumOutputChannels(); ++i)
    input.clear (i, 0, numSamples);*/
    
    //Delay
    //Iterates through the channels
    
    for(int channel = 0; channel < bufferToFill.buffer->getNumChannels(); channel++)
    {
        
        delay.performDelay(*bufferToFill.buffer, balance.getListenerDistance(channel, userPosX, depthAtUserPos), currentSampleRate, channel);
    }
    
    //Master Fader
    //Iterates through the channels
    for(int channel = 0; channel < bufferToFill.buffer->getNumChannels(); channel++)
    {
        //Creates Pointer to the first sample of the selected channel
        float* buffer = bufferToFill.buffer->getWritePointer(channel, bufferToFill.startSample);
    
        //Iterates through the samples
        for(int sample = 0; sample < bufferToFill.numSamples; ++sample)
        {
            //Sets the master gain
            buffer[sample] = buffer[sample] * masterSliderValue;
        }
    }
}

void MainComponent::releaseResources()
{
    audioPlayer.releaseResources();
}

//==============================================================================
void MainComponent::paint (Graphics& g)
{
    g.setOpacity(1.0);
    g.fillAll(Colours::orangered);
}

void MainComponent::resized()
{
    //Channels.setBounds(0, 180, 800, 340);
    
    Lights.setBounds(1000, 20, 200, 55);
    
    masterSlider.setBounds(1090, 470, 50, 200);
    masterSliderLabel.setBounds(1090, 440, 50, 25);
    
    //panningLaw.setBounds(1000, 150, 200, 100);
    balance.setBounds(1000, 90, 200, 350);
    
    kinUpButton.setBounds(1000, 300, 100, 30);
    kinDownButton.setBounds(1000, 350, 100, 30);
    
    UserSelectedDeviceSettings.setBounds(0, 0, 400, 100);
    
    audioPlayer.setBounds(100, 500, 200, 150);
    
    audioOutSelector.setBounds(400, 500, 200, 30);
    
    CVWindowButton.setBounds(700, 500, 200, 30);
    closeCVWindow.setBounds(700, 550, 200, 30);
    minThresSlider.setBounds(500, 100, 30, 200);
    maxThresSlider.setBounds(550, 100, 30, 200);
    
    kinectTestButton.setBounds(600, 100, 100, 30);
    
    kinectImage.setBounds(100, 200, 320, 240);
}

void MainComponent::buttonClicked(Button* button)
{
    if (button == &CVWindowButton)
    {
        Timer::startTimer(1000);
        cv::namedWindow("Cascade", cv::WINDOW_AUTOSIZE);
        cv::namedWindow("Depth", cv::WINDOW_AUTOSIZE);
    }
    
    else if (button == &closeCVWindow)
    {
        Timer::stopTimer();
        cv::destroyAllWindows();
    }
    
    else if (button == &kinectTestButton)
    {
        DBG("Kin Test - Depth Array left:" << kinectImage.kinect.depthArray[240][0]);
        DBG("Kin Test - Depth Array center:" << kinectImage.kinect.depthArray[240][320]);
        DBG("Kin Test - Depth Array right:" << kinectImage.kinect.depthArray[240][639]);
        
        DBG("Kin Test - Depth Mat left:" << depthMatLeft);
        DBG("Kin Test - Depth Mat center:" << depthMatCenter);
        DBG("Kin Test - Depth Mat right:" << depthMatRight);
    }
}

void MainComponent::sliderValueChanged(Slider* slider)
{
    if(slider == &masterSlider)
    {
        masterSliderValue = masterSlider.getValue();
    }
}

void MainComponent::timerCallback()
{
    MessageManagerLock cvLock;
    //Shows image with small y axis
    cv::Mat depthMat(480, 1280, CV_8UC1, &kinectImage.kinect.depthArray);
    cv::Mat colourMat(320, 640, CV_8UC3, &kinectImage.kinect.colourArray);
    cv::Mat colourMatGrey;
    
    depthMatLeft = depthMat.at<uint8_t>(240, 0);
    depthMatCenter = depthMat.at<uint8_t>(240, 640);
    depthMatRight = depthMat.at<uint8_t>(240, 1279);
    
    cv::Mat imageWithCascade;
    imageWithCascade = haarCascade.performCascade(colourMat);
    cv::imshow("Cascade", imageWithCascade);
    
    cv::circle(depthMat, cv::Point(640, 240), 20, cv::Scalar(0, 0, 255), 4);
    
    cv::imshow("Depth", depthMat);
    
    workOutDepthAtPosition();
}

void MainComponent::workOutDepthAtPosition()
{
    userPosX = haarCascade.getPersonPointX();
    
    depthAtUserPos = kinectImage.kinect.depthArray[haarCascade.getPersonPointY()][userPosX];
}
