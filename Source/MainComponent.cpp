/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent() : AudioAppComponent(UserSelectedDevice), UserSelectedDeviceSettings(UserSelectedDevice, 0, 0, 0, 6, false, false, false, false), CVWindowButton("Open CV View"), closeCVWindow("Close CV"), userPosX(329)
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
    
    kinect.kinInit();
    
    addAndMakeVisible(speakerLineDistanceSlider);
    speakerLineDistanceSlider.setRange(0.5, 10);
    speakerLineDistanceSlider.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
    speakerLineDistanceSlider.setValue(2);
    speakerLineDistanceSlider.addListener(this);
    balance.setSpeakerLineDistance(speakerLineDistanceSlider.getValue());
    addAndMakeVisible(speakerLineDisLabel);
    speakerLineDisLabel.setText("Speaker Distance", dontSendNotification);
    
    addAndMakeVisible(leftChannelGainSlider);
    leftChannelGainSlider.setRange(0, 2);
    leftChannelGainSlider.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
    leftChannelGainSlider.setValue(0);
    addAndMakeVisible(leftChannelGainLabel);
    leftChannelGainLabel.setText("Left Channel Gain", dontSendNotification);
    
    
    addAndMakeVisible(rightChannelGainSlider);
    rightChannelGainSlider.setRange(0, 2);
    rightChannelGainSlider.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
    rightChannelGainSlider.setValue(0);
    addAndMakeVisible(rightChannelGainLabel);
    rightChannelGainLabel.setText("Right Channel Gain", dontSendNotification);
    
    Timer::startTimer(100);
    cv::namedWindow("Cascade", cv::WINDOW_AUTOSIZE);
    cv::namedWindow("Depth", cv::WINDOW_AUTOSIZE);
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    AlertWindow::showOkCancelBox(AlertWindow::AlertIconType::WarningIcon, "Close App?", "Are you sure you want to close this app?", "Continue", "Cancel", nullptr, nullptr);
    
    kinect.End();
    
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
    g.fillAll(Colours::grey);
}

void MainComponent::resized()
{
    masterSlider.setBounds(1090, 470, 50, 200);
    masterSliderLabel.setBounds(1090, 440, 50, 25);
    
    balance.setBounds(1000, 90, 200, 350);

    UserSelectedDeviceSettings.setBounds(0, 0, 400, 100);
    
    audioPlayer.setBounds(100, 500, 200, 150);
    
    CVWindowButton.setBounds(700, 500, 200, 30);
    closeCVWindow.setBounds(700, 550, 200, 30);

    speakerLineDistanceSlider.setBounds(600, 200, 200, 30);
    speakerLineDisLabel.setBounds(600, 180, 100, 20);
    
    leftChannelGainSlider.setBounds(100, 300, 600, 30);
    leftChannelGainLabel.setBounds(100, 280, 100, 20);
    rightChannelGainSlider.setBounds(100, 350, 600, 30);
    rightChannelGainLabel.setBounds(100, 330, 100, 20);
}

void MainComponent::buttonClicked(Button* button)
{
    if (button == &CVWindowButton)
    {
        Timer::startTimer(40);
        cv::namedWindow("Cascade", cv::WINDOW_AUTOSIZE);
        //cv::namedWindow("Depth", cv::WINDOW_AUTOSIZE);
    }
    
    else if (button == &closeCVWindow)
    {
        Timer::stopTimer();
        cv::destroyAllWindows();
    }
}

void MainComponent::sliderValueChanged(Slider* slider)
{
    if(slider == &masterSlider)
    {
        masterSliderValue = masterSlider.getValue();
    }
    
    else if(slider == &speakerLineDistanceSlider)
    {
        balance.setSpeakerLineDistance(speakerLineDistanceSlider.getValue());
    }
}

void MainComponent::timerCallback()
{
    MessageManagerLock cvLock;
    //Shows image with small y axis
    cv::Mat depthMat(480, 1280, CV_8UC1, &kinect.depthArray);
    cv::Mat colourMat(320, 640, CV_8UC3, &kinect.colourArray);
    
    cv::Mat imageWithCascade;
    imageWithCascade = haarCascade.performCascade(colourMat);
    cv::imshow("Cascade", imageWithCascade);
    
    //cv::imshow("Depth", depthMat);
    
    workOutDepthAtPosition();
    
    repaintSliders();
}

void MainComponent::workOutDepthAtPosition()
{
    userPosX = haarCascade.getPersonPointX();
    
    depthAtUserPos = kinect.depthArray[haarCascade.getPersonPointY()][userPosX];
}

void MainComponent::repaintSliders()
{
    leftChannelGainSlider.setValue(balance.getLeftGain());
    rightChannelGainSlider.setValue(balance.getRightGain());
    
    leftChannelGainSlider.repaint();
    rightChannelGainSlider.repaint();
}
