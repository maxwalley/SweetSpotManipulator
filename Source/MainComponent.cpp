/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent() : AudioAppComponent(UserSelectedDevice), UserSelectedDeviceSettings(UserSelectedDevice, 0, 0, 0, 6, false, false, false, false), chooser("Open a File", File(), "*.wav", true, true), OpenFileButton("Open File"), PlayPauseButton("Play/Pause"), rewindButton("Rewind"), kinPic(Image::PixelFormat::RGB, 640, 480, true), rgbPic(Image::PixelFormat::RGB, 640, 480, true)
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (1300, 700);
    
    /*addAndMakeVisible(Channels);
    addAndMakeVisible(Lights);
    addAndMakeVisible(OpenFileButton);
    addAndMakeVisible(PlayPauseButton);
    addAndMakeVisible(rewindButton);*/
    
    addAndMakeVisible(masterSlider);
    masterSlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
    masterSlider.setRange(0, 1);
    
    addAndMakeVisible(panningLaw);
    
    addAndMakeVisible(panSlider);
    panSlider.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
    panSlider.setRange(0, 1);
    
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
    
    /*LFMultiplier = 0;
    RFMultiplier = 0;
    
    LFMultiState = up;
    RFMultiState = up;
    
    LFsamplecount = 0;
    RFsamplecount = 0;*/
    
    FormatManager.registerBasicFormats();
    
    OpenFileButton.onClick = [this] {buttonClickedEvent();};
    
    audioBlockCount = 0;
    
    setDepthPixels();
    setRGBPixels();
    
    channel1Multiplier = 0;
    channel2Multiplier = 0;
    
    channel1State = up;
    channel2State = up;
    
    channel1SampleCount = 0;
    channel2SampleCount = 0;
    
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
        DBG("Kinect no error");
    }
    else if(kinectErrorCodeTriggered == false)
    {
        DBG("Kinect error");
    }*/
    
    /*if(kinectErrorCodeTriggered == true)
    {
        int kinProcessingErrorCode = kin.RunVidandDepth();
        if(kinProcessingErrorCode != 0)
        {
            DBG("Kinect processing failed with error code: " << kinProcessingErrorCode);
            kinectErrorCodeTriggered = false;
        }
    }*/
    
    if(kinectErrorCodeTriggered == true)
    {
        int kinLEDErrorCode = kin.checkLed(Lights.getSelectedLed());
        if(kinLEDErrorCode != 0)
        {
            DBG("Kinect led check failed with error code: " << kinLEDErrorCode);
            kinectErrorCodeTriggered = false;
        }
    }
    
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
    /*if(audioBlockCount >= 50)
    {
        audioBlockCount = 0;
        setDepthPixels();
        //setRGBPixels();
        paintImage();
    }
    else
    {
        audioBlockCount++;
    }*/
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
    
    Lights.setBounds(500, 80, 250, 60);
    
    OpenFileButton.setBounds(650, 20, 80, 20);
    PlayPauseButton.setBounds(530, 20, 80, 20);
    rewindButton.setBounds(530, 50, 80, 20);*/
    
    masterSlider.setBounds(900, 300, 30, 200);
    panningLaw.setBounds(800, 600, 200, 30);
    panSlider.setBounds(800, 660, 190, 30);
    
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
    if(channel == 0)
    {
        if(panningLaw.getPanningLaw() == 0)
        {
            return multiplier * masterSlider.getValue() * panSlider.getValue();
        }
    }
    else if(channel == 1)
    {
        if(panningLaw.getPanningLaw() == 0)
        {
            return multiplier * masterSlider.getValue() * (1 - panSlider.getValue());
        }
    }
    
    return 0;
}
