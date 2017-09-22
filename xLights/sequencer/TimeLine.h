#ifndef TIMELINE_H
#define TIMELINE_H

#include "wx/wx.h"
#include "wx/window.h"
#define HORIZONTAL_PADDING      10
#define PIXELS_PER_MAJOR_HASH   100

#define ZOOM_OUT        0
#define ZOOM_IN         1

#define SCROLL_RIGHT    0
#define SCROLL_LEFT     1

enum EFFECT_SCREEN_MODE{
    SCREEN_L_R_OFF,
    SCREEN_L_ON,
    SCREEN_R_ON,
    SCREEN_L_R_ON,
    SCREEN_L_R_ACROSS,
};


wxDECLARE_EVENT(EVT_TIME_LINE_CHANGED, wxCommandEvent);

class TimeLine : public wxWindow
{
    public:
        TimeLine(wxPanel* parent, wxWindowID id, const wxPoint &pos=wxDefaultPosition, const wxSize &size=wxDefaultSize,
                       long style=0, const wxString &name=wxPanelNameStr);
        virtual ~TimeLine();
        void Initialize();

        void SetStartTimeMS(int time);
        float GetStartTimeMS() const;

        bool SetPlayMarkerMS(int ms);
        int GetPlayMarker() const;

        void SetZoomMarkerMS(int ms);

        void SetSelectedPositionStart(int pos, bool reset_end = true);
        void SetSelectedPositionStartMS(int time);
        void SetSelectedPositionEnd(int pos);
        void SetSelectedPositionEndMS(int time);
        int GetSelectedPositionStart() const { return mSelectedPlayMarkerStart; }
        int GetSelectedPositionEnd() const { return mSelectedPlayMarkerEnd; }
        void LatchSelectedPositions();
        void SetMousePositionMS(int ms);
        int GetMousePosition() const { return mMousePosition; }
        void SavePosition();
        void RestorePosition();

        void ResetMarkers(int ms);
        void RecalcEndTime();

        void SetStartPixelOffset(int startPixel);
        int GetStartPixelOffset();

        void SetTimeFrequency(int frequency);
        int GetTimeFrequency();

        void SetTimeLength(int ms);
        int GetTimeLength() const;

        void SetZoomLevel(int level);
        int GetZoomLevel();

        int GetZoomLevelValue() const;
        int GetMaxZoomLevel();
        void ZoomIn();
        void ZoomOut();

        int TimePerMajorTickInMS();
        int PixelsPerMajorTick() const { return PIXELS_PER_MAJOR_HASH; };
        void GetViewableTimeRange(int &StartTime, int &EndTime);

        void GetPositionsFromTimeRange(int startTimeMS,int endTimeMS,EFFECT_SCREEN_MODE &screenMode,int &x1, int &x2, int& x3, int& x4);
        int GetPositionFromTimeMS(int timeMS);

        void SetSequenceEnd(int ms);

        int GetTimeMSfromPosition(int position);
        int GetAbsoluteTimeMSfromPosition(int position);
        int GetRawTimeMSfromPosition(int position);

        static int RoundToMultipleOfPeriod(int number,double period);

        struct TIME_LINE_CHANGED_ARGS
        {
            int zoomLevel;
            int startPixel;
        };

        void PlayStarted();       // signal the start of play so timeline can adjust marks
        void PlayStopped();       // signal play stop so timeline can adjust marks
        int GetNewStartTimeMS() const;  // return the time where to begin playing
        int GetNewEndTimeMS() const;    // return the time where to end playing
        int GetCurrentPlayMarkerMS() const { return mCurrentPlayMarker;};
        void GoToTag(int tag);

        void MoveToLeft(int numberOfPixels);
        void MoveToRight(int numberOfPixels);
        void RaiseChangeTimeline();
        void CheckNeedToScrollToPlayStart(bool paused = false);
        const static int ZoomLevelValues[];

    private:
        DECLARE_EVENT_TABLE()
        void mouseLeftDown( wxMouseEvent& event);
        void mouseRightDown(wxMouseEvent& event);
        void mouseLeftUp( wxMouseEvent& event);
        void mouseMoved( wxMouseEvent& event);
        void OnPopup(wxCommandEvent& event);
        void OnLostMouseCapture(wxMouseCaptureLostEvent& event);
        void triggerPlay();
        void DrawTag(wxDC& dc, int tag, int position, int y_bottom);
        bool mIsInitialized;
        int mStartTimeMS;
        int mEndTimeMS;
        int mEndPos;
        int _savedPosition;
        int _tagPositions[10];
        int _rightClickPosition;

        int mStartPixelOffset;
        int mFrequency;
        int mZoomLevel;
        int mMaxZoomLevel;
        int mTimeLength;
        int mCurrentPlayMarkerStart;
        int mCurrentPlayMarkerEnd;
        int mSelectedPlayMarkerStart;
        int mSelectedPlayMarkerEnd;
        int mCurrentPlayMarker;
        int mCurrentPlayMarkerStartMS;
        int mCurrentPlayMarkerEndMS;
        int mSelectedPlayMarkerStartMS;
        int mSelectedPlayMarkerEndMS;
        int mCurrentPlayMarkerMS;
        int mSequenceEndMarker;
        int mSequenceEndMarkerMS;
        int mZoomMarkerMS;
        int mMousePositionMS;
        int mMousePosition;
        bool m_dragging;
        bool timeline_initiated_play;

        void render( wxPaintEvent& event );
        void render( wxDC& dc );
        float GetFirstTimeLabelFromPixelOffset(int offset);
        int GetPixelOffsetFromStartTime();
        int GetMaxViewableTimeMS();
        int GetTotalViewableTimeMS();
        int GetPositionFromSelection(int position);
        void DrawTriangleMarkerFacingLeft(wxDC& dc, int& play_start_mark, const int& tri_size, int& height);
        void DrawTriangleMarkerFacingRight(wxDC& dc, int& play_start_mark, const int& tri_size, int& height);
        void DrawRectangle(wxDC& dc, int x1, int y1, int x2, int y2);
        void RecalcMarkerPositions();
        wxPanel* mParent;
};

class TimelineChangeArguments
{
    public:
        TimelineChangeArguments(int zoomLevel, int startPixelOffset,int currentTime);
        virtual ~TimelineChangeArguments();
        int ZoomLevel;
        int StartPixelOffset;
        int CurrentTimeMS;

};

#endif // TIMELINE_H
