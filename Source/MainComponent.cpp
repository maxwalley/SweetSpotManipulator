/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent() : AudioAppComponent(UserSelectedDevice), UserSelectedDeviceSettings(UserSelectedDevice, 0, 0, 0, 6, false, false, false, false), kinPic(Image::PixelFormat::RGB, 640, 480, true), rgbPic(Image::PixelFormat::RGB, 640, 480, true), audioBlockCount(0), channel1Multiplier(0), channel2Multiplier(0), channel1State(up), channel2State(up), channel1SampleCount(0), channel2SampleCount(0), kinUpButton("Tilt Up"), kinDownButton("Tilt Down"), CVWindowButton("Open CV View")
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (1280, 800);
    
    //addAndMakeVisible(Channels);
     
    addAndMakeVisible(masterSlider);
    masterSlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
    masterSlider.setRange(0, 1);
    masterSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, false, 50, 20);
    masterSlider.addListener(this);
    addAndMakeVisible(masterSliderLabel);
    masterSliderLabel.setText("Master", dontSendNotification);
    
    addAndMakeVisible(Lights);
    
    addAndMakeVisible(CVWindowButton);
    CVWindowButton.addListener(this);
    
    //addAndMakeVisible(panningLaw);
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
    
    setDepthPixels();
    setRGBPixels();
    
    StringArray availableCameras = CameraDevice::getAvailableDevices();
    DBG("Number of Available Cameras is: " << availableCameras.size());
    
    /*int initErrorCode = kin.kinInit();
    
    if(initErrorCode != 0)
    {
        DBG("Kinect init failed with error code: " << initErrorCode);
        kinectErrorCodeTriggered = true;
    }
    else if (initErrorCode == 0)
    {
        DBG("Kinect init completed");
        kinectErrorCodeTriggered = false;
    }*/
    
    addAndMakeVisible(kinUpButton);
    addAndMakeVisible(kinDownButton);
    
    addAndMakeVisible(audioOutSelector);
    audioOutSelector.addItem("Square Wave", 1);
    audioOutSelector.addItem("Audio Player", 2);
    audioOutSelector.setSelectedId(1);
    audioOutSelector.setEditableText(false);
    
    addAndMakeVisible(kinectImage);
    kinectImage.kinectInit();
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    AlertWindow::showOkCancelBox(AlertWindow::AlertIconType::WarningIcon, "Close App?", "Are you sure you want to close this app?", "Continue", "Cancel", nullptr, nullptr);
    kinectImage.kinectEnd();
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    // This function will be called when the audio device is started, or when
    // its settings (i.e. sample rate, block size, etc) are changed.
    printf("Sample Rate is: %f\n", sampleRate);
    printf("Block Size is %d\n", samplesPerBlockExpected);
    
    //kinRefreshRate = floor(sampleRate/samplesPerBlockExpected);
    
    audioPlayer.prepareToPlay(samplesPerBlockExpected, sampleRate);
    // You can use this function to initialise any resources you might need,
    // but be careful - it will be called on the audio thread, not the GUI thread.
}

void MainComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    /*if(kinectErrorCodeTriggered == true)
    {
        int kinLEDErrorCode = kin.checkLed(Lights.getSelectedLed());
        if(kinLEDErrorCode != 0)
        {
            DBG("Kinect led check failed with error code: " << kinLEDErrorCode);
            kinectErrorCodeTriggered = false;
        }
    }
    */
    //printf("Audio Block Count = %d\n", audioBlockCount);
    
    if(audioOutSelector.getSelectedId() == 1)
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
    
    else if(audioOutSelector.getSelectedId() == 2)
    {
        audioPlayer.getNextAudioBlock(bufferToFill);
    }
    
    /*if(audioBlockCount >= kinRefreshRate)
    {
        if(kinectErrorCodeTriggered == false)
        {
            int kinProcessingErrorCode = kin.RunVidandDepth();
            if(kinProcessingErrorCode != 0)
            {
                DBG("Kinect processing failed with error code: " << kinProcessingErrorCode);
                kinectErrorCodeTriggered = true;
            }
        }
        else
        {
            DBG("kinect ERROR");
        }
        
        audioBlockCount = 0;
        setDepthPixels();
        //setRGBPixels();
        //paintImage();
        pic.repaintImage();
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
    /*if(kinectErrorCodeTriggered == false)
    {
        int kinEndErrorCode = kin.End();
        if(kinEndErrorCode != 0)
        {
            DBG("Kinect closing sequence failed with error code: " << kinEndErrorCode);
            kinectErrorCodeTriggered = true;
        }
    }*/
    audioPlayer.releaseResources();
}

//==============================================================================
void MainComponent::paint (Graphics& g)
{
    g.setOpacity(1.0);
    g.fillAll(Colours::orangered);
    
    //g.drawImage(kinPic, 100, 200, 320, 240, 0, 0, 640, 480, false);
    g.drawImage(rgbPic, 500, 200, 320, 240, 0, 0, 640, 480, false);
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
    
    kinectImage.setBounds(100, 200, 320, 240);
}


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
        displayDepthImageCV();
    }
}

void MainComponent::paintImage()
{
    const MessageManagerLock paintLock;
    repaint(100, 200, 320, 240);
    //DBG("Repainting");
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
            //kinPic.setPixelAt(i, c, Colour(kin.depthArray[i][c], kin.depthArray[i][c], kin.depthArray[i][c]));
            //pic.setImage(i, c, kin.depthArray[i][c]);
        }
    }
}

void MainComponent::setRGBPixels()
{
    for(int i = 0; i < 640; i++)
    {
        for(int c = 0; c < 480; c++)
        {
            //rgbPic.setPixelAt(i, c, Colour(kin.redArray[i][c], kin.greenArray[i][c], kin.blueArray[i][c]));
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

void MainComponent::sliderValueChanged(Slider* slider)
{
    audioPlayer.setGain(slider->getValue());
}

void MainComponent::displayDepthImageCV()
{
    //cv::Mat image = kin.getDepthImageCV();
    
    cv::namedWindow("Cv Image", cv::WINDOW_AUTOSIZE);
    
    //cv::imshow("Cv Image", image);
    cv::waitKey(500);
}
