/*
 * Copyright (c) 2006 Sean C. Rhea (srhea@srhea.net)
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc., 51
 * Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef _GC_MainWindow_h
#define _GC_MainWindow_h 1
#include "GoldenCheetah.h"

#include <QDir>
#include <QSqlDatabase>
#include <QtGui>
#include "RideItem.h"
#include "TimeUtils.h"

#ifdef Q_OS_MAC
// What versions are supported by this SDK?
#include <AvailabilityMacros.h>
#endif

class HomeWindow;
class IntervalSummaryWindow;
class RideNavigator;
class GcToolBar;
class DiarySidebar;
class GcMultiCalendar;
class LTMSidebar;
class AnalysisSidebar;
class LionFullScreen;
class QTFullScreen;
class TrainSidebar;
class ChartSettings;
class QtMacSegmentedButton;
class QtMacButton;
class GcScopeBar;
class Library;
class BlankStateAnalysisPage;
class BlankStateHomePage;
class BlankStateDiaryPage;
class BlankStateTrainPage;
class GcSplitter;
class GcSplitterItem;
class QtSegmentControl;
class SaveSingleDialogWidget;
class SplitActivityWizard;
class IntervalItem;

class MainWindow;
class Athlete;
class Context;

extern QList<MainWindow *> mainwindows; // keep track of all the MainWindows we have open
extern QDesktopWidget *desktop;         // how many screens / res etc

class MainWindow : public QMainWindow
{
    Q_OBJECT
    G_OBJECT

    // transitional
    friend class ::Context;
    friend class ::SaveSingleDialogWidget;
    friend class ::SplitActivityWizard;

    public:

        MainWindow(const QDir &home);

        // transitionary
        Context *context;

        // filters changed
        void notifyFilter(QStringList f) { filters = f; emit filterChanged(f); }

        // *********************************************
        // MAINWINDOW STATE / GUI DATA
        // *********************************************

        // Top-level views
        HomeWindow *homeWindow;
        HomeWindow *diaryWindow;
        HomeWindow *trainWindow;
        HomeWindow *analWindow;
        HomeWindow *currentWindow;  // tracks the curerntly showing window

        BlankStateAnalysisPage *blankStateAnalysisPage;
        BlankStateHomePage *blankStateHomePage;
        BlankStateDiaryPage *blankStateDiaryPage;
        BlankStateTrainPage *blankStateTrainPage;

        bool showBlankAnal;
        bool showBlankTrain;
        bool showBlankHome;
        bool showBlankDiary;

        ChartSettings *chartSettings;

        // state data
        int session;

        // global filters
        bool isfiltered;
        QStringList filters;

        // splitter for sidebar and main view
        QSplitter *splitter;

        // sidebar and views
        QStackedWidget *toolBox; // contains all left sidebars
        QStackedWidget *views;   // contains all the views

        // sidebars
        AnalysisSidebar *analysisSidebar;
        TrainSidebar *trainSidebar; // train view
        LTMSidebar *ltmSidebar; // home view
        DiarySidebar *diarySidebar; // diary

#if (defined Q_OS_MAC) && (defined GC_HAVE_LION)
        LionFullScreen *fullScreen;
#endif
#ifndef Q_OS_MAC
        QTFullScreen *fullScreen;
#endif

    protected:

        virtual void resizeEvent(QResizeEvent*);
        virtual void moveEvent(QMoveEvent*);
        virtual void closeEvent(QCloseEvent*);
        virtual void dragEnterEvent(QDragEnterEvent *);
        virtual void dropEvent(QDropEvent *);

    signals:
        void filterChanged(QStringList&);

    public slots:
        void closeAll();
        void addDevice();

        void checkBlankState();
        void closeBlankTrain();
        void closeBlankAnal();
        void closeBlankDiary();
        void closeBlankHome();

        void downloadErgDB();
        void manageLibrary();
        void showWorkoutWizard();

        // transitionary
        void rideSelected(RideItem*ride);
        bool saveRideSingleDialog(RideItem *);
        void saveSilent(RideItem *);

    private slots:
        void splitterMoved(int, int);
        void newCyclist();
        void openCyclist();
        void downloadRide();
        void manualRide();
        void exportRide();
        void exportBatch();
        void exportMetrics();
        void uploadRideWithGPSAction();
        void uploadTtb();
        void manualProcess(QString);
#ifdef GC_HAVE_SOAP
        void uploadTP();
        void downloadTP();
#endif
#ifdef GC_HAVE_ICAL
        void uploadCalendar(); // upload ride to calendar
#endif
        void importFile();
        void importWorkout();
        void splitRide();
        void mergeRide();
        void deleteRide();
        void aboutDialog();
        void saveRide();                        // save current ride menu item
        void revertRide();
        bool saveRideExitDialog();              // save dirty rides on exit dialog
        void showOptions();
        void showTools();
        void showRhoEstimator();
        void toggleSidebar();
        void showSidebar(bool want);
        void showToolbar(bool want);
        void resetWindowLayout();
        void dateChanged(const QDate &);

        // working with measures, not rides
        void recordMeasure();
        void downloadMeasures();
        void exportMeasures();
        void importMeasures();
        void downloadMeasuresFromZeo();

        // get calendars
        void refreshCalendar();
        void importCalendar();
        void exportCalendar();

        void helpView();
        void logBug();

        void selectHome();
        void selectDiary();
        void selectAnalysis();
        void selectTrain();

        void setActivityMenu();
        void setWindowMenu();
        void selectWindow(QAction*);

        void setChartMenu();
        void setSubChartMenu();
        void addChart(QAction*);

        void toggleStyle();
        void setStyle();
        // special case for linux/win qtsegmentcontrol toggline
        void setStyleFromSegment(int);

#ifndef Q_OS_MAC
        void toggleFullScreen();
#else
        // Mac Native Support
        void searchTextChanged(QString);
#endif
        void actionClicked(int);

        void dateRangeChangedDiary(DateRange);
        void dateRangeChangedLTM(DateRange);

        // from global search
        void searchResults(QStringList);
        void searchClear();


    protected:

    private:

        // active when right clicked
        RideItem *activeRide; // currently active for context menu popup

#ifdef Q_OS_MAC
        // Mac Native Support
        QtMacButton *import, *compose, *sidebar;
        QtMacSegmentedButton *actbuttons, *styleSelector;
        QToolBar *head;
#else
        // Not on Mac so use other types
        QPushButton *import, *compose, *sidebar;
        QtSegmentControl *actbuttons, *styleSelector;
        GcToolBar *head;
        QPushButton *full;

        // the icons
        QIcon importIcon, composeIcon, intervalIcon, splitIcon,
              deleteIcon, sidebarIcon, tabbedIcon, tiledIcon;
#endif
        GcScopeBar *scopebar;

        // chart menus
        QMenu *chartMenu;
        QMenu *subChartMenu;

        // Application menu
        QMenu *windowMenu;

        // Application actions
        // only keeping those used outside of mainwindow constructor
        QAction *styleAction;
        QAction *showhideSidebar;
        QAction *rideWithGPSAction;
        QAction *ttbAction;

        // each view has its own controls
        QStackedWidget *masterControls,
                       *analysisControls,
                       *trainControls,
                       *diaryControls,
                       *homeControls;

        // Miscellany
        QSignalMapper *groupByMapper;
        QSignalMapper *toolMapper;

};

#endif // _GC_MainWindow_h
