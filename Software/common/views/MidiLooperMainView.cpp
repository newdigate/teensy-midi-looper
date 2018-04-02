//
// Created by Nicholas Newdigate on 17/03/2018.
//

#include "MidiLooperMainView.h"

void MidiLooperMainView::update(unsigned long millis) {
    _loopSequencer->tick(millis);

    if (_lastUpdate == 0 || millis - _lastUpdate > 10) {
        _recordingIndicator.update(millis);
        _songPositionIndicator.setSongPosition(_loopSequencer->getSongPosition());
        _songTimeIndicator.update(millis);
        _lastUpdate = millis;
    } else
    if (_lastPianoDisplayUpdate - millis > 50) {
        if (_topPianoDisplay.displayNeedsUpdating())
            _topPianoDisplay.drawPiano();

        if (_bottomPianoDisplay.displayNeedsUpdating())
            _bottomPianoDisplay.drawPiano();

        _lastPianoDisplayUpdate = millis;
    }
}
