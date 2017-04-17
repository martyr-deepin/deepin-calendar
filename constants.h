#ifndef CONSTANTS_H
#define CONSTANTS_H

namespace DDECalendar {
    static const int HeaderItemWidth = 80;
    static const int HeaderItemHeight = 40;

    static const int CellWidth = 106;
    static const int CellHeight = 70;

    static const int CellHighlightWidth = 80;
    static const int CellHighlightHeight = 70;
}

enum Week {
    Monday    = 6,
    Tuesday   = 5,
    Wednesday = 4,
    Thursday  = 3,
    Friday    = 2,
    Saturday  = 1,
    Sunday    = 0,
};

#endif // CONSTANTS_H
