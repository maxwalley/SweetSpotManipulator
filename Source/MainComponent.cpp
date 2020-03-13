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
    
    addAndMakeVisible(speaker0xCoOrdSlider);
    speaker0xCoOrdSlider.setRange(-4, 4);
    speaker0xCoOrdSlider.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
    speaker0xCoOrdSlider.setValue(0);
    speaker0xCoOrdSlider.addListener(this);
    addAndMakeVisible(speaker0xCoOrdSliderLabel);
    speaker0xCoOrdSliderLabel.setText("Speaker 1 X Position", dontSendNotification);
    
    addAndMakeVisible(speaker0yCoOrdSlider);
    speaker0yCoOrdSlider.setRange(0, 8);
    speaker0yCoOrdSlider.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
    speaker0yCoOrdSlider.setValue(0);
    speaker0yCoOrdSlider.addListener(this);
    addAndMakeVisible(speaker0yCoOrdSliderLabel);
    speaker0yCoOrdSliderLabel.setText("Speaker 1 Y Position", dontSendNotification);
    
    addAndMakeVisible(speaker1xCoOrdSlider);
    speaker1xCoOrdSlider.setRange(-4, 4);
    speaker1xCoOrdSlider.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
    speaker1xCoOrdSlider.setValue(0);
    speaker1xCoOrdSlider.addListener(this);
    addAndMakeVisible(speaker1xCoOrdSliderLabel);
    speaker1xCoOrdSliderLabel.setText("Speaker 2 X Position", dontSendNotification);
    
    addAndMakeVisible(speaker1yCoOrdSlider);
    speaker1yCoOrdSlider.setRange(0, 8);
    speaker1yCoOrdSlider.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
    speaker1yCoOrdSlider.setValue(0);
    speaker1yCoOrdSlider.addListener(this);
    addAndMakeVisible(speaker1yCoOrdSliderLabel);
    speaker1yCoOrdSliderLabel.setText("Speaker 2 Y Position", dontSendNotification);
    
    
    balance.setSpeakerCoOrdinates(0, speaker0xCoOrdSlider.getValue(), speaker0yCoOrdSlider.getValue());
    balance.setSpeakerCoOrdinates(1, speaker1xCoOrdSlider.getValue(), speaker1yCoOrdSlider.getValue());
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
            buffer[sample] = buffer[sample] * balance.getMultiplier(channel, userPosX, depthAtUserPos);
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
    masterSlider.setBounds(1090, 490, 50, 200);
    masterSliderLabel.setBounds(1090, 470, 50, 20);
    
    balance.setBounds(1000, 90, 200, 110);
    
    speaker0xCoOrdSliderLabel.setBounds(1000, 220, 200, 20);
    speaker0xCoOrdSlider.setBounds(1000, 250, 200, 20);
    
    speaker0yCoOrdSliderLabel.setBounds(1000, 280, 200, 20);
    speaker0yCoOrdSlider.setBounds(1000, 310, 200, 20);
    
    speaker1xCoOrdSliderLabel.setBounds(1000, 350, 200, 20);
    speaker1xCoOrdSlider.setBounds(1000, 380, 200, 20);
    
    speaker1yCoOrdSliderLabel.setBounds(1000, 410, 200, 20);
    speaker1yCoOrdSlider.setBounds(1000, 440, 200, 20);
    
    UserSelectedDeviceSettings.setBounds(0, 0, 400, 100);
    
    audioPlayer.setBounds(100, 500, 200, 150);
    
    CVWindowButton.setBounds(700, 500, 200, 30);
    closeCVWindow.setBounds(700, 550, 200, 30);
    
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
        cv::namedWindow("Depth", cv::WINDOW_AUTOSIZE);
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
    
    else if(slider == &speaker0xCoOrdSlider || slider == &speaker0yCoOrdSlider)
    {
        balance.setSpeakerCoOrdinates(0, speaker0xCoOrdSlider.getValue(), speaker0yCoOrdSlider.getValue());
    }
    
    else if(slider == &speaker1xCoOrdSlider || slider == &speaker1yCoOrdSlider)
    {
        balance.setSpeakerCoOrdinates(1, speaker1xCoOrdSlider.getValue(), speaker1yCoOrdSlider.getValue());
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
    
    cv::imshow("Depth", depthMat);
    
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
