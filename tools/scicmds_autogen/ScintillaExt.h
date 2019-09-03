/// x-studio365 spec ScintillaExt.h header
#pragma once
#include "Platform.h"

#define SCLEX_SEARCHRESULT     150

#define SCN_MARGINRELEASECLICK 5001
#define SCN_CHARADDING         5002
#define SCN_VSCROLLCHANGE      5003
#define SCN_UNKNOWN_MESSAGE    5099

#define SCI_ALPHARECTANGLE     2901
#define SCI_QUICK_PASTE        2902
#define SCI_GETMARGINFROMPOINT 2903

#define SC_SEARCHRESULT_LINEBUFFERMAXLENGTH 1024


struct SearchResultMarking {
    long _start;
    long _end;
};

struct SearchResultMarkings {
    long _length;
    SearchResultMarking *_markings;
};

namespace Scintilla {
    /** AlphaRectangle parameters
    *
    */
    struct AlphaRectangleParameters
    {
        int cornerSize;
        PRectangle rectangle;
        ColourDesired colorFill;
        int alphaFill;
        ColourDesired colorOutline;
        int alphaOutline;
    };
};
