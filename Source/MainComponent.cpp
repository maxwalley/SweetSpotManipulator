/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent() : AudioAppComponent(UserSelectedDevice), UserSelectedDeviceSettings(UserSelectedDevice, 0, 0, 0, 6, false, false, false, false), userPosX(329)
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (1000, 600);
    
    addAndMakeVisible(masterSlider);
    masterSlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
    masterSlider.setRange(0, 1);
    masterSlider.setValue(0);
    masterSliderValue = masterSlider.getValue();
    masterSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, false, 50, 20);
    masterSlider.addListener(this);
    addAndMakeVisible(masterSliderLabel);
    masterSliderLabel.setText("Master", dontSendNotification);
    
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
    leftChannelGainSlider.setRange(0, 4);
    leftChannelGainSlider.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
    leftChannelGainSlider.setValue(0);
    addAndMakeVisible(leftChannelGainLabel);
    leftChannelGainLabel.setText("Left Channel Gain", dontSendNotification);
    
    
    addAndMakeVisible(rightChannelGainSlider);
    rightChannelGainSlider.setRange(0, 4);
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
    
    addAndMakeVisible(speaker0zCoOrdSlider);
    speaker0zCoOrdSlider.setRange(0, 8);
    speaker0zCoOrdSlider.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
    speaker0zCoOrdSlider.setValue(0);
    speaker0zCoOrdSlider.addListener(this);
    addAndMakeVisible(speaker0zCoOrdSliderLabel);
    speaker0zCoOrdSliderLabel.setText("Speaker 1 Z Position", dontSendNotification);
    
    addAndMakeVisible(speaker1xCoOrdSlider);
    speaker1xCoOrdSlider.setRange(-4, 4);
    speaker1xCoOrdSlider.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
    speaker1xCoOrdSlider.setValue(0);
    speaker1xCoOrdSlider.addListener(this);
    addAndMakeVisible(speaker1xCoOrdSliderLabel);
    speaker1xCoOrdSliderLabel.setText("Speaker 2 X Position", dontSendNotification);
    
    addAndMakeVisible(speaker1zCoOrdSlider);
    speaker1zCoOrdSlider.setRange(0, 8);
    speaker1zCoOrdSlider.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
    speaker1zCoOrdSlider.setValue(0);
    speaker1zCoOrdSlider.addListener(this);
    addAndMakeVisible(speaker1zCoOrdSliderLabel);
    speaker1zCoOrdSliderLabel.setText("Speaker 2 Z Position", dontSendNotification);
    
    
    balance.setSpeakerCoOrdinates(0, speaker0xCoOrdSlider.getValue(), speaker0zCoOrdSlider.getValue());
    balance.setSpeakerCoOrdinates(1, speaker1xCoOrdSlider.getValue(), speaker1zCoOrdSlider.getValue());
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
            buffer[sample] = buffer[sample] * masterSliderValue * 5;
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
    masterSlider.setBounds(350, 370, 50, 200);
    masterSliderLabel.setBounds(350, 350, 50, 20);
    
    balance.setBounds(500, 30, 200, 40);
    
    speaker0xCoOrdSliderLabel.setBounds(750, 170, 200, 20);
    speaker0xCoOrdSlider.setBounds(750, 200, 200, 20);
    
    speaker0zCoOrdSliderLabel.setBounds(750, 230, 200, 20);
    speaker0zCoOrdSlider.setBounds(750, 260, 200, 20);
    
    speaker1xCoOrdSliderLabel.setBounds(750, 320, 200, 20);
    speaker1xCoOrdSlider.setBounds(750, 350, 200, 20);
    
    speaker1zCoOrdSliderLabel.setBounds(750, 380, 200, 20);
    speaker1zCoOrdSlider.setBounds(750, 410, 200, 20);
    
    UserSelectedDeviceSettings.setBounds(0, 0, 400, 100);
    
    audioPlayer.setBounds(100, 420, 200, 150);
    
    leftChannelGainSlider.setBounds(50, 220, 600, 30);
    leftChannelGainLabel.setBounds(50, 200, 100, 20);
    rightChannelGainSlider.setBounds(50, 270, 600, 30);
    rightChannelGainLabel.setBounds(50, 250, 100, 20);
}

void MainComponent::sliderValueChanged(Slider* slider)
{
    if(slider == &masterSlider)
    {
        masterSliderValue = masterSlider.getValue();
    }
    
    else if(slider == &speaker0xCoOrdSlider || slider == &speaker0zCoOrdSlider)
    {
        balance.setSpeakerCoOrdinates(0, speaker0xCoOrdSlider.getValue(), speaker0zCoOrdSlider.getValue());
    }
    
    else if(slider == &speaker1xCoOrdSlider || slider == &speaker1zCoOrdSlider)
    {
        balance.setSpeakerCoOrdinates(1, speaker1xCoOrdSlider.getValue(), speaker1zCoOrdSlider.getValue());
    }
}

void MainComponent::timerCallback()
{
    MessageManagerLock cvLock;
    //Shows image with small y axis
    cv::Mat colourMat(320, 640, CV_8UC3, &kinect.colourArray);
    
    cv::Mat imageWithCascade;
    imageWithCascade = haarCascade.performCascade(colourMat);
    cv::imshow("Cascade", imageWithCascade);
    
    /*for(int y = 0; y < 320; y++)
    {
        kinect.depthArray[y][haarCascade.getPersonPointX()] = 0;
    }
    
    for(int x = 0; x < 640; x++)
    {
        kinect.depthArray[haarCascade.getPersonPointY()][x] = 0;
    }*/
    //kinect.depthArray[haarCascade.getPersonPointY()][haarCascade.getPersonPointX()] = 0;
    
    cv::Mat depthMat(480, 1280, CV_8UC1, &kinect.depthArray);
    
    workOutDepthAtPosition();
    
    cv::imshow("Depth", depthMat);
    
    repaintSliders();
}

void MainComponent::workOutDepthAtPosition()
{
    userPosX = haarCascade.getPersonPointX();
    //DBG(userPosX);
    
    depthAtUserPos = kinect.depthArray[haarCascade.getPersonPointY()][userPosX];
    //DBG(haarCascade.getPersonPointY());
}

void MainComponent::repaintSliders()
{
    leftChannelGainSlider.setValue(balance.getLeftGain());
    rightChannelGainSlider.setValue(balance.getRightGain());
    
    leftChannelGainSlider.repaint();
    rightChannelGainSlider.repaint();
}
