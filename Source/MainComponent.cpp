/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent() : AudioAppComponent(UserSelectedDevice), UserSelectedDeviceSettings(UserSelectedDevice, 0, 0, 0, 6, false, false, false, false), channel1Multiplier(0), channel2Multiplier(0), channel1State(up), channel1SampleCount(0), channel2State(up), channel2SampleCount(0), kinUpButton("Tilt Up"), kinDownButton("Tilt Down"), CVWindowButton("Open CV View"), closeCVWindow("Close CV"), kinectTestButton("Kin Test"), userPosOnXAxis(329), userPosOnYAxis(239)
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
    //Checks that the user has selected square wave and that the masterSlider is not at 0
    if(audioOutSelector.getSelectedId() == 1 && masterSlider.getValue() != 0)
    {
        //Iterates through the channels
        for(int channel = 0; channel < bufferToFill.buffer->getNumChannels(); channel++)
        {
            //Creates Pointer to the first sample of the selected channel
            float* buffer = bufferToFill.buffer->getWritePointer(channel, bufferToFill.startSample);
        
            //Iterates through the samples
            for(int sample = 0; sample < bufferToFill.numSamples; ++sample)
            {
                if(channel == 0)
                {
                    if(channel1State == up)
                    {
                        channel1Multiplier = 0.25;
                        if(channel1SampleCount == 200)
                        {
                            channel1SampleCount = 0;
                            channel1State = down;
                        }
                    }
                    else if(channel1State == down)
                    {
                        channel1Multiplier = -0.25;
                        if(channel1SampleCount == 200)
                        {
                            channel1SampleCount = 0;
                            channel1State = up;
                        }
                    }
                    channel1SampleCount += 1;
                    buffer[sample] = workOutValue(channel1Multiplier, channel);
                }
            
                else if(channel == 1)
                {
                    if(channel2State == up)
                    {
                        channel2Multiplier = 0.25;
                        if(channel2SampleCount == 200)
                        {
                            channel2SampleCount = 0;
                            channel2State = down;
                        }
                    }
                    else if(channel2State == down)
                    {
                        channel2Multiplier = -0.25;
                        if(channel2SampleCount == 200)
                        {
                            channel2SampleCount = 0;
                            channel2State = up;
                        }
                    }
                    channel2SampleCount += 1;
                    buffer[sample] = workOutValue(channel2Multiplier, channel);
                }
            }
        }
    }
    //Checks user has selected Audio Player
    else if(audioOutSelector.getSelectedId() == 2)
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
                //Applies balance processing to that data
                buffer[sample] = workOutValue(buffer[sample], channel);
            }
        }
    }
    
    //Work out at a later date
    /*for (int i = getNumInputChannels(); i < getNumOutputChannels(); ++i)
    input.clear (i, 0, numSamples);*/
    
    //Delay
    //Iterates through the channels
    
    for(int channel = 0; channel < bufferToFill.buffer->getNumChannels(); channel++)
    {
        
        delay.performDelay(*bufferToFill.buffer, balance.getListenerDistance(channel, userPosOnXAxis, depthAtUserPos), currentSampleRate, channel);
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

/*StringArray MainComponent::getMenuBarNames()
{
    const char* const names[] = { "File", "Edit", 0 };
    return StringArray (names);
}

PopupMenu getMenuForIndex(int topLevelMenuIndex, const String &menuName)
{
    PopupMenu menu;
    
    if (topLevelMenuIndex == 0)
    {
        menu.addItem(1, "Audio Prefrences", true, false);
    }
    
    return menu;
}

void MainComponent::menuItemSelected(int menuItemID, int topLevelMenuIndex)
{
    if(topLevelMenuIndex == 0)
    {
        if (menuItemID == 1)
        {
            DBG("AUDIO PREFS CLICKED");
        }
    }
}*/

void MainComponent::buttonClicked(Button* button)
{
    /*if(button == &kinUpButton)
    {
        if(kinectErrorCodeTriggered == false)
        {
            int kinTiltUpErrorCode = kin.kinTiltUp();
            
            if(kinTiltUpErrorCode != 0)
            {
                DBG("Kinect tilt up failed with error code: " << kinTiltUpErrorCode);
                kinectErrorCodeTriggered = true;
            }
        }
    }
    
    else if(button == &kinDownButton)
    {
        if(kinectErrorCodeTriggered == false)
        {
            int kinTiltDownErrorCode = kin.kinTiltDown();
            
            if(kinTiltDownErrorCode != 0)
            {
                DBG("Kinect tilt down failed with error code: " << kinTiltDownErrorCode);
                kinectErrorCodeTriggered = true;
            }
        }
    }*/
    
    if (button == &CVWindowButton)
    {
        Timer::startTimer(1000);
        //cv::namedWindow("keypoints", cv::WINDOW_AUTOSIZE);
        //cv::namedWindow("Contours", cv::WINDOW_AUTOSIZE);
        cv::namedWindow("Canny", cv::WINDOW_AUTOSIZE);
        cv::namedWindow("Cascade", cv::WINDOW_AUTOSIZE);
        cv::namedWindow("Colour", cv::WINDOW_AUTOSIZE);
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

float MainComponent::workOutValue(float input, int channel)
{
    /*if(channel == 0)
    {
        
        if(panningLaw.getPanningLaw() == 0)
        {
            return multiplier * masterSlider.getValue() * (sin(0.5 * M_PI * panningLaw.getPanSliderVal()));
        }
        
        else if(panningLaw.getPanningLaw() == 1)
        {
            return multiplier * masterSlider.getValue() * panningLaw.getPanSliderVal();
        }
    }
    else if(channel == 1)
    {
        if(panningLaw.getPanningLaw() == 0)
        {
            return multiplier * masterSlider.getValue() * (1 - (sin(0.5 * M_PI * panningLaw.getPanSliderVal())));
        }
        
        else if(panningLaw.getPanningLaw() == 1)
        {
            return multiplier * masterSlider.getValue() * (1 - panningLaw.getPanSliderVal());
        }
    }*/
    
    return input * masterSlider.getValue() * balance.workOutMultiplier(channel, userPosOnXAxis, depthAtUserPos);
    
    return 0;
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
        
    //cv::imshow("Colour", colourMat);
    
    cv::Mat imageWithCascade;
    imageWithCascade = haarCascade.performCascade(colourMat);
    cv::imshow("Cascade", imageWithCascade);
    
    workOutDepthAtPosition();
    
    //cv::Canny(test, cannyOutput, minThresSlider.getValue(), maxThresSlider.getValue());
    
    //cv::imshow("Image", test);
    
    //cv::imshow("Canny", cannyOutput);
    
    //workOutPosition(test);
    
    
    
}

void MainComponent::workOutPosition(cv::Mat input)
{
    bool locationFound = false;
    
    //Reading from top to bottom
    for(int y = 50; y < 480 && locationFound == false; y++)
    {
        //Reading from left to right
        for(int x = 50; x < 1279 && locationFound == false; x++)
        {
            
            if(cannyOutput.at<uint8_t>(y, x) == 255)
            {
                DBG("LOCATION FOUND!!!! at " << y << ", " << x);
                userPosOnXAxis = floor(x/2);
                
                locationFound = true;
            }
        }
    }
}

void MainComponent::workOutDepthAtPosition()
{
    
    depthAtUserPos = kinectImage.kinect.depthArray[haarCascade.getPersonPointY()][getPersonPointX()];
}
