//
//  TFTScreenEmulatorWindow.hpp
//  TFTDisplayEmulator - App
//
//  Created by Nicholas Newdigate on 01/04/2018.
//  Copyright © 2018 ROLI Ltd. All rights reserved.
//

#ifndef TFTScreenEmulatorWindow_hpp
#define TFTScreenEmulatorWindow_hpp

#include <stdio.h>
#include "../JuceLibraryCode/JuceHeader.h"

class TFTScreenEmulatorWindow : public DocumentWindow, Timer
{
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TFTScreenEmulatorWindow)
    juce::Image *_image;

public:
    TFTScreenEmulatorWindow (
                             const String& name,
                             Colour backgroundColour,
                             int buttonsNeeded,
                             juce::Image *image) : DocumentWindow (name, backgroundColour, buttonsNeeded), Timer()
    {
        _image = image;
        setOpaque (true);
        setSize (160, 160);
        startTimer(50);
    }
    
    void closeButtonPressed() override
    {
        delete this;
    }
    
    void paint (Graphics& g) override
    {
        g.fillAll (Colours::black);
        g.drawImage ( *_image,
                     0, 0, getWidth(), getHeight(),
                     0, 0, 160, 160, false);
    }
    
    void timerCallback() override
    
    {
        repaint();
        //std::cout << "time tick" << std::endl;
        
    }
    
};



#endif /* TFTScreenEmulatorWindow_hpp */
