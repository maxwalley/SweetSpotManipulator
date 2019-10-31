/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent() : AudioAppComponent(UserSelectedDevice), UserSelectedDeviceSettings(UserSelectedDevice, 0, 0, 0, 6, false, false, false, false), chooser("Open a File", File(), "*.wav", true, true), OpenFileButton("Open File"), PlayPauseButton("Play/Pause"), rewindButton("Rewind"), kinPic(Image::PixelFormat::RGB, 640, 480, true), rgbPic(Image::PixelFormat::RGB, 640, 480, true), audioBlockCount(0), channel1Multiplier(0), channel2Multiplier(0), channel1State(up), channel2State(up), channel1SampleCount(0), channel2SampleCount(0), kinUpButton("Tilt Up"), kinDownButton("Tilt Down")
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (1280, 800);
    
    /*addAndMakeVisible(Channels);
    addAndMakeVisible(OpenFileButton);
    addAndMakeVisible(PlayPauseButton);
    addAndMakeVisible(rewindButton);*/
    
    addAndMakeVisible(masterSlider);
    masterSlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
    masterSlider.setRange(0, 1);
    masterSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, false, 50, 20);
    addAndMakeVisible(masterSliderLabel);
    masterSliderLabel.setText("Master", dontSendNotification);
    
    addAndMakeVisible(Lights);
    
    //addAndMakeVisible(panningLaw);
    addAndMakeVisible(balance);
        
    addAndMakeVisible(UserSelectedDeviceSettings);
    UserSelectedDevice.initialise(0, 6, nullptr, false);

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
    
    PlayPauseButton.setClickingTogglesState(true);
    rewindButton.setClickingTogglesState(true);
    PlayPauseButton.setToggleState(false, dontSendNotification);
    rewindButton.setToggleState(false, dontSendNotification);
    
    FormatManager.registerBasicFormats();
    
    OpenFileButton.onClick = [this] {buttonClickedEvent();};
    
    setDepthPixels();
    setRGBPixels();
    
    
    //kinectErrorCodeTriggered = true;
    
    int initErrorCode = kin.kinInit();
    
    /*if(initErrorCode != 0)
    {
        DBG("Kinect init failed with error code: " << initErrorCode);
        kinectErrorCodeTriggered = false;
    }
    else if (initErrorCode == 0)
    {
        DBG("Kinect init completed");
        kinectErrorCodeTriggered = true;
    }
    
    if(kinectErrorCodeTriggered == true)
    {
        int tiltErrorCode = kin.kinTilt();
    
        if(tiltErrorCode != 0)
        {
            DBG("Kinect tilt failed with error code: " << tiltErrorCode);
            kinectErrorCodeTriggered = false;
        }
        else if (tiltErrorCode == 0)
        {
            DBG("Kinect tilt completed");
        }
    }*/
    
    kinectErrorCodeTriggered = false;
    
    addAndMakeVisible(kinUpButton);
    addAndMakeVisible(kinDownButton);
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    AlertWindow::showOkCancelBox(AlertWindow::AlertIconType::WarningIcon, "Close App?", "Are you sure you want to close this app?", "Continue", "Cancel", nullptr, nullptr);
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    // This function will be called when the audio device is started, or when
    // its settings (i.e. sample rate, block size, etc) are changed.
    printf("Sample Rate is: %f\n", sampleRate);
    printf("Block Size is %d\n", samplesPerBlockExpected);
    // You can use this function to initialise any resources you might need,
    // but be careful - it will be called on the audio thread, not the GUI thread.
}

void MainComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    /*if(kinectErrorCodeTriggered == true)
    {
        int kinProcessingErrorCode = kin.RunVidandDepth();
        if(kinProcessingErrorCode != 0)
        {
            DBG("Kinect processing failed with error code: " << kinProcessingErrorCode);
            kinectErrorCodeTriggered = false;
        }
    }
    
    if(kinectErrorCodeTriggered == true)
    {
        int kinLEDErrorCode = kin.checkLed(Lights.getSelectedLed());
        if(kinLEDErrorCode != 0)
        {
            DBG("Kinect led check failed with error code: " << kinLEDErrorCode);
            kinectErrorCodeTriggered = false;
        }
    }*/
    
    //printf("Audio Block Count = %d\n", audioBlockCount);
    
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
    if(audioBlockCount >= 50)
    {
        audioBlockCount = 0;
        //setDepthPixels();
        //setRGBPixels();
        //paintImage();
    }
    else
    {
        audioBlockCount++;
    }
}

void MainComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.
    if(kinectErrorCodeTriggered == true)
    {
        int kinEndErrorCode = kin.End();
        if(kinEndErrorCode != 0)
        {
            DBG("Kinect closing sequence failed with error code: " << kinEndErrorCode);
            kinectErrorCodeTriggered = false;
        }
    }
}

//==============================================================================
void MainComponent::paint (Graphics& g)
{
    g.setOpacity(1.0);
    g.fillAll(Colours::orangered);
    
    PlayPauseButton.setColour(TextButton::ColourIds::buttonOnColourId, Colours::royalblue);
    rewindButton.setColour(TextButton::ColourIds::buttonOnColourId, Colours::royalblue);
    
    g.drawImage(kinPic, 100, 200, 320, 240, 0, 0, 640, 480, false);
    g.drawImage(rgbPic, 500, 200, 320, 240, 0, 0, 640, 480, false);
}

void MainComponent::resized()
{
    /*Channels.setBounds(0, 180, 800, 340);
    
    OpenFileButton.setBounds(650, 20, 80, 20);
    PlayPauseButton.setBounds(530, 20, 80, 20);
    rewindButton.setBounds(530, 50, 80, 20);*/
    
    Lights.setBounds(1000, 20, 200, 55);
    
    masterSlider.setBounds(1090, 470, 50, 200);
    masterSliderLabel.setBounds(1090, 440, 50, 25);
    
    //panningLaw.setBounds(1000, 150, 200, 100);
    balance.setBounds(1000, 90, 200, 350);
    
    //kinUpButton.setBounds(1000, 300, 100, 30);
    //kinDownButton.setBounds(1000, 350, 100, 30);
    
    UserSelectedDeviceSettings.setBounds(0, 0, 400, 100);
}

void MainComponent::buttonClickedEvent()
{
    if(chooser.browseForFileToOpen() == true)
    {
        playFile = chooser.getResult();
    }
    
    AudioFormatReader* reader = FormatManager.createReaderFor(playFile);
    AudioFormatReaderSource source(reader, true);
    
    std::cout<<reader->getFormatName()<<std::endl;
}

void MainComponent::buttonClicked(Button* button)
{
    if(button == &kinUpButton)
    {
        if(kinectErrorCodeTriggered == true)
        {
            int kinTiltUpErrorCode = kin.kinTiltUp();
            
            if(kinTiltUpErrorCode != 0)
            {
                DBG("Kinect tilt up failed with error code: " << kinTiltUpErrorCode);
                kinectErrorCodeTriggered = false;
            }
        }
    }
    
    else if(button == &kinDownButton)
    {
        if(kinectErrorCodeTriggered == true)
        {
            int kinTiltDownErrorCode = kin.kinTiltDown();
            
            if(kinTiltDownErrorCode != 0)
            {
                DBG("Kinect tilt down failed with error code: " << kinTiltDownErrorCode);
                kinectErrorCodeTriggered = false;
            }
        }
    }
}

void MainComponent::paintImage()
{
    const MessageManagerLock paintLock;
    repaint(100, 200, 720, 240);
}

void MainComponent::setDepthPixels()
{
    //uint8_t shortArray[480][640];
    
    for(int i = 0; i < 640; i++)
    {
        for(int c = 0; c < 480; c++)
        {
            //shortArray[c][i] = kin.depthArray[c][i]/8.02745;
            //printf("Depth data = %hu\n", shortArray[c][i]);
            kinPic.setPixelAt(i, c, Colour(kin.depthArray[i][c], kin.depthArray[i][c], kin.depthArray[i][c]));
        }
    }
}

void MainComponent::setRGBPixels()
{
    for(int i = 0; i < 640; i++)
    {
        for(int c = 0; c < 480; c++)
        {
            rgbPic.setPixelAt(i, c, Colour(kin.redArray[i][c], kin.greenArray[i][c], kin.blueArray[i][c]));
        }
    }
}

float MainComponent::workOutValue(float multiplier, int channel)
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
    
    return multiplier * masterSlider.getValue() * balance.workOutMultiplier(channel);
    
    return 0;
}
