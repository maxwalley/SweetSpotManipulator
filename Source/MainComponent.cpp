/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent() : AudioAppComponent(UserSelectedDevice), UserSelectedDeviceSettings(UserSelectedDevice, 0, 0, 0, 6, false, false, false, false), chooser("Open a File", File(), "*.wav", true, true), OpenFileButton("Open File"), PlayPauseButton("Play/Pause"), rewindButton("Rewind"), kinPic(Image::PixelFormat::RGB, 640, 480, true)
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (1300, 700);
    
    /*addAndMakeVisible(Channels);
    addAndMakeVisible(Lights);
    addAndMakeVisible(OpenFileButton);
    addAndMakeVisible(PlayPauseButton);
    addAndMakeVisible(rewindButton);*/
    
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
    
    LFMultiplier = 0;
    RFMultiplier = 0;
    
    LFMultiState = up;
    RFMultiState = up;
    
    LFsamplecount = 0;
    RFsamplecount = 0;
    
    FormatManager.registerBasicFormats();
    
    OpenFileButton.onClick = [this] {buttonClickedEvent();};
    
    audioBlockCount = 0;
    
    rebuildArray();
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
    kin.InitandMove();
    // For more details, see the help for AudioProcessor::prepareToPlay()
}

void MainComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    kin.RunVidandDepth();
    kin.checkLed(Lights.selectedLed);
    
    printf("Audio Block Count = %d\n", audioBlockCount);
    
    for(int channel = 0; channel < bufferToFill.buffer->getNumChannels(); channel++)
    {
        float* buffer = bufferToFill.buffer->getWritePointer(channel, bufferToFill.startSample);
        
        for(int sample = 0; sample < bufferToFill.numSamples; ++sample)
        {
            if(channel == 0 && Channels.LeftFront.Mute.getToggleState() == false && Channels.Master.Mute.getToggleState() == false)
            {
                LFsamplecount += 1;
                
                if(LFMultiState == up)
                {
                    LFMultiplier = 0.25;
                    if(LFsamplecount == 200)
                    {
                        LFMultiState = down;
                        LFsamplecount = 0;
                    }
                }
                else if(LFMultiState == down)
                {
                    LFMultiplier = -0.25;
                    if(LFsamplecount == 200)
                    {
                        LFMultiState = up;
                        LFsamplecount = 0;
                    }
                }
                buffer[sample] = LFMultiplier * (Channels.LeftFront.CHSlider.getValue() * Channels.Master.CHSlider.getValue());
            }
            
            if(channel == 1 && Channels.RightFront.Mute.getToggleState() == false && Channels.Master.Mute.getToggleState() == false)
            {
                RFsamplecount += 1;
            
                if(RFMultiState == up)
                {
                    RFMultiplier = 0.25;
                    if(RFsamplecount == 200)
                    {
                        RFMultiState = down;
                        RFsamplecount = 0;
                    }
                }
                else if(RFMultiState == down)
                {
                    RFMultiplier = -0.25;
                    if(RFsamplecount == 200)
                    {
                        RFMultiState = up;
                        RFsamplecount = 0;
                    }
                }
                buffer[sample] = RFMultiplier * (Channels.RightFront.CHSlider.getValue() * Channels.Master.CHSlider.getValue());
            }
        }
    }
    if(audioBlockCount >= 10)
    {
        audioBlockCount = 0;
        rebuildArray();
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
    AlertWindow::showOkCancelBox(AlertWindow::AlertIconType::WarningIcon, "Close App?", "Are you sure you want to close this app?", "Continue", "Cancel", nullptr, nullptr);
    
    kin.End();
    // For more details, see the help for AudioProcessor::releaseResources()
}

//==============================================================================
void MainComponent::paint (Graphics& g)
{
    g.setOpacity(1.0);
    g.fillAll(Colours::orangered);
    
    PlayPauseButton.setColour(TextButton::ColourIds::buttonOnColourId, Colours::royalblue);
    rewindButton.setColour(TextButton::ColourIds::buttonOnColourId, Colours::royalblue);
    
    g.drawImage(kinPic, 200, 200, 640, 480, 0, 0, 640, 480, false);
}

void MainComponent::resized()
{
    Channels.setBounds(0, 180, 800, 340);
    
    Lights.setBounds(500, 80, 250, 60);
    
    OpenFileButton.setBounds(650, 20, 80, 20);
    PlayPauseButton.setBounds(530, 20, 80, 20);
    rewindButton.setBounds(530, 50, 80, 20);
    
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
    repaint(200, 200, 640, 480);
}

void MainComponent::rebuildArray()
{
    uint8_t shortArray[640][480];
    
    for(int i = 0; i < 480; i++)
    {
        for(int c = 0; c < 640; c++)
        {
            shortArray[c][i] = kin.depthArray[c][i]/8.02745;
            std::cout << "Depth data = " << shortArray[c][i] << std::endl;
            kinPic.setPixelAt(c, i, Colour(shortArray[c][i], 0, 0));
        }
    }
    paintImage();
}
