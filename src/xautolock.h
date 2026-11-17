//===========================================================================
//
// This file is part of the KDE project
//
// Copyright (c) 1999 Martin R. Jones <mjones@kde.org>
//

#ifndef __XAUTOLOCK_H__
#define __XAUTOLOCK_H__

#include <time.h>
#include <qobject.h>
#include <qptrqueue.h>
#include <X11/Xlib.h>

//===========================================================================
//
// A window and its creation time.
//
class WatchedWindow
{
public:
    WatchedWindow(Window w)
        { mWindow = w; mTime = time(0); }

    time_t created() { return mTime; }
    Window window() { return mWindow; }

protected:
    Window mWindow;
    time_t mTime;
};

//===========================================================================
//
// Detect user inactivity.
// Named XAutoLock after the program that it is based on.
//
class XAutoLock : public QObject
{
    Q_OBJECT
public:
    XAutoLock();
    ~XAutoLock();

    //-----------------------------------------------------------------------
    //
    // The time in seconds of continuous inactivity.
    //
    void setTimeout(int t);

    //-----------------------------------------------------------------------
    //
    // Start watching Activity
    //
    void start();

    //-----------------------------------------------------------------------
    //
    // Stop watching Activity
    //
    void stop();

public slots:
    void windowCreated(Window w);
    void keyPressed();

signals:
    void timeout();
	void activity();

protected:
    void resetTrigger();
    void processWatched(time_t delay);
    void selectEvents(Window window, bool substructure_only);
    void queryPointer();
    virtual void timerEvent(QTimerEvent *ev);

protected:
    int     mTimerId;
    int     mTimeout;
    int     mTrigger;
    bool    mActive;
	bool	mIdle;
    time_t  mLastTimeout;
    QPtrQueue<WatchedWindow>  mWindows;
};

#endif

