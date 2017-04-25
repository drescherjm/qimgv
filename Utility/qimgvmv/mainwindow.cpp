#include "mainwindow.h"

MainWindow::MainWindow() :
	m_AryImageViewers{},
	m_AryControlsOverlay{},
    toolbox(NULL),
    currentViewer(0),
    currentDisplay(0),
    layout(NULL),
    desktopWidget(NULL)
{
    resize(1100, 700);
    setMinimumSize(QSize(400, 300));
    this->setMouseTracking(true);
    this->setAcceptDrops(true);
    init();
    setWindowTitle(QCoreApplication::applicationName() +
                   " " +
                   QCoreApplication::applicationVersion());
}

void MainWindow::init() {
    desktopWidget = QApplication::desktop();


	for (size_t i = 0; i < m_AryImageViewers.size(); ++i) {
		auto& pImageViewer = m_AryImageViewers[i];
		auto& pControl = m_AryControlsOverlay[i];

		pImageViewer = new ImageViewer(this);
		pImageViewer->hide();

		pControl = new ControlsOverlay(pImageViewer);
		pControl->hide();
	}

    QWidget *central = new QWidget();

    layout = new QGridLayout;
    central->setAttribute(Qt::WA_MouseTracking);
    QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    central->setSizePolicy(sizePolicy);
    layout->setContentsMargins(0, 0, 0, 0);
	layout->setHorizontalSpacing(3);
	layout->setVerticalSpacing(3);
    central->setLayout(layout);
    this->setCentralWidget(central);
    windowMoveTimer.setSingleShot(true);
    windowMoveTimer.setInterval(150);

    core = new Core();

    connect(settings, SIGNAL(settingsChanged()),
            this, SLOT(readSettings()));

    enableImageViewer();

	connect(core, SIGNAL(signalUnsetImage()), this, SLOT(imageLoaded()),Qt::QueuedConnection);

    connect(this, SIGNAL(signalNextImage()),
            core, SLOT(slotNextImage()));

    connect(this, SIGNAL(signalPrevImage()),
            core, SLOT(slotPrevImage()));

    connect(this, SIGNAL(fileOpened(QString)),
            core, SLOT(loadImage(QString)));

    connect(this, SIGNAL(signalFullscreenEnabled(bool)),
            this, SLOT(slotShowControls(bool)));

    connect(this, SIGNAL(signalFullscreenEnabled(bool)),
            this, SLOT(slotShowInfo(bool)));

    connect(core, SIGNAL(infoStringChanged(QString)),
            this, SLOT(setInfoString(QString)));

	for (auto pControl : m_AryControlsOverlay) {
		connect(pControl, SIGNAL(exitClicked()),
            this, SLOT(close()));
	}

    // Shortcuts

//     connect(actionManager, SIGNAL(nextImage()), core, SLOT(slotNextImage()));
//     connect(actionManager, SIGNAL(prevImage()), core, SLOT(slotPrevImage()));
    connect(actionManager, SIGNAL(fitAll()), this, SLOT(slotFitAll()));
    connect(actionManager, SIGNAL(fitWidth()), this, SLOT(slotFitWidth()));
    connect(actionManager, SIGNAL(fitNormal()), this, SLOT(slotFitNormal()));
    connect(actionManager, SIGNAL(toggleFitMode()), this, SLOT(switchFitMode()));
    connect(actionManager, SIGNAL(toggleFullscreen()), this, SLOT(triggerFullscreen()));


	for (auto pImageViewer : m_AryImageViewers) {
		connect(actionManager, SIGNAL(zoomIn()), pImageViewer, SLOT(slotZoomIn()));
		connect(actionManager, SIGNAL(zoomOut()), pImageViewer, SLOT(slotZoomOut()));
		connect(actionManager, SIGNAL(scrollUp()), pImageViewer, SLOT(scrollUp()));
		connect(actionManager, SIGNAL(scrollDown()), pImageViewer, SLOT(scrollDown()));
	}
    
	
	connect(actionManager, SIGNAL(resize()), this, SLOT(slotResizeDialog()));
    connect(actionManager, SIGNAL(rotateLeft()), this, SLOT(slotRotateLeft()));
    connect(actionManager, SIGNAL(rotateRight()), this, SLOT(slotRotateRight()));
    connect(actionManager, SIGNAL(openSettings()), this, SLOT(showSettings()));
    connect(actionManager, SIGNAL(crop()), this, SLOT(slotCrop()));
    connect(actionManager, SIGNAL(setWallpaper()), this, SLOT(slotSelectWallpaper()));
    connect(actionManager, SIGNAL(open()), this, SLOT(slotOpenDialog()));
    //connect(actionManager, SIGNAL(save()), this, SLOT(slotSaveDialog()));
    connect(actionManager, SIGNAL(exit()), this, SLOT(close()));
    connect(actionManager, SIGNAL(removeFile()), core, SLOT(removeFile()));

    //connect(this, SIGNAL(fileSaved(QString)), core, SLOT(saveImage(QString)));

    //connect(&windowMoveTimer, SIGNAL(timeout()), this, SLOT(updateCurrentDisplay()));

    readSettingsInitial();
    core->init();
}

void MainWindow::enableImageViewer() {
    if(currentViewer != 1) {
      
		for (size_t i = 0; i < m_AryImageViewers.size();++i) {
	
			auto pImageViewer = m_AryImageViewers[i];
			auto pControl = m_AryControlsOverlay[i];

			pControl->setParent(pImageViewer);

			int nRow = i & 2;
			int nCol = i & 1;
			
			layout->addWidget(pImageViewer,nRow,nCol);
			
			pImageViewer->show();

			connect(core, SIGNAL(loadingTimeout()),
				pImageViewer, SLOT(showLoadingMessage()), Qt::UniqueConnection);

			connect(pImageViewer, SIGNAL(scalingRequested(QSize)),
				core, SLOT(rescaleForZoom(QSize)), Qt::UniqueConnection);

			connect(core, SIGNAL(scalingFinished(QPixmap *,int)),
				this, SLOT(scalingFinished(QPixmap *,int)), Qt::UniqueConnection);
			
			connect(core, SIGNAL(frameChanged(QPixmap *)),
				pImageViewer, SLOT(updateImage(QPixmap *)),
				static_cast<Qt::ConnectionType>(Qt::DirectConnection | Qt::UniqueConnection));
			// reload after image edits
			connect(core, SIGNAL(imageAltered(QPixmap *)),
				pImageViewer, SLOT(displayImage(QPixmap *)), Qt::UniqueConnection);

			connect(pImageViewer, SIGNAL(cropSelected(QRect)),
				core, SLOT(crop(QRect)), Qt::UniqueConnection);

			connect(pImageViewer, SIGNAL(wallpaperSelected(QRect)),
				core, SLOT(setWallpaper(QRect)), Qt::UniqueConnection);

			connect(core, SIGNAL(signalSetImage(QPixmap *, int)),
				this, SLOT(openImage(QPixmap *,int)), Qt::UniqueConnection);

			connect(this, SIGNAL(signalFitAll()),
				pImageViewer, SLOT(slotFitAll()), Qt::UniqueConnection);

			connect(this, SIGNAL(signalFitWidth()),
				pImageViewer, SLOT(slotFitWidth()), Qt::UniqueConnection);

			connect(this, SIGNAL(signalFitNormal()),
				pImageViewer, SLOT(slotFitNormal()), Qt::UniqueConnection);

			connect(this, SIGNAL(signalZoomIn()),
				pImageViewer, SLOT(slotZoomIn()), Qt::UniqueConnection);

			connect(this, SIGNAL(signalZoomOut()),
				pImageViewer, SLOT(slotZoomOut()), Qt::UniqueConnection);
		}
        
		updateOverlays();
        currentViewer = 1;
    }
}

void MainWindow::disableImageViewer() {

	for (auto pImageViewer : m_AryImageViewers) {
		
		layout->removeWidget(pImageViewer);

		disconnect(core, SIGNAL(loadingTimeout()),
			pImageViewer, SLOT(showLoadingMessage()));

		disconnect(pImageViewer, SIGNAL(scalingRequested(QSize)),
			core, SLOT(rescaleForZoom(QSize)));

		disconnect(core, SIGNAL(signalSetAnimation(QMovie*)),
			pImageViewer, SLOT(displayAnimation(QMovie*)));

		disconnect(core, SIGNAL(signalStopAnimation()),
			pImageViewer, SLOT(stopAnimation()));

		disconnect(pImageViewer, SIGNAL(cropSelected(QRect)),
			core, SLOT(crop(QRect)));

		disconnect(pImageViewer, SIGNAL(wallpaperSelected(QRect)),
			core, SLOT(setWallpaper(QRect)));

		disconnect(core, SIGNAL(signalSetImage(QPixmap *)),
			pImageViewer, SLOT(displayImage(QPixmap *)));

		disconnect(this, SIGNAL(signalZoomIn()),
			pImageViewer, SLOT(slotZoomIn()));

		disconnect(this, SIGNAL(signalZoomOut()),
			pImageViewer, SLOT(slotZoomOut()));

		currentViewer = 0;
		pImageViewer->hide();

	}
}

void MainWindow::open(QString path) {
   // core->loadImageBlocking(path);

	QFileInfo info(path);

	if (info.isDir()) {
		core->loadFolder(path + '/',true);
	}
	else {
		core->loadImage(path, true);
	}

	
}

void MainWindow::openImage(QPixmap *pixmap, int nIndex) {
    enableImageViewer();

	if (nIndex < nImageViewers) {
		m_AryImageViewers[nIndex]->displayImage(pixmap);
	}
}

void MainWindow::readSettingsInitial() {
    readSettings();
    currentDisplay = settings->lastDisplay();
    restoreWindowGeometry();
    if(settings->fullscreenMode() && !isFullScreen()) {
        this->triggerFullscreen();
    }
}

void MainWindow::readSettings() {
    fitMode = settings->imageFitMode();
    if(fitMode == 1) {
        slotFitWidth();
    } else if(fitMode == 2) {
        slotFitNormal();
    } else {
        slotFitAll();
    }
    emit resized(size());
}

void MainWindow::updateOverlays() {
    //m_AryControlsOverlay->updatePosition(this->centralWidget()->size());


	for (size_t i = 0; i < m_AryControlsOverlay.size();++i) {

		auto pControl = m_AryControlsOverlay[i];
		auto pImageViewer = m_AryImageViewers[i];

		pControl->updatePosition(pImageViewer->size());
	}

//    infoOverlay->updateWidth(this->centralWidget()->width());
}

void MainWindow::resizeEvent(QResizeEvent *event) {
    Q_UNUSED(event)

    updateOverlays();
}

//#############################################################
//######################### EVENTS ############################
//#############################################################

void MainWindow::mouseMoveEvent(QMouseEvent *event) {
    if(event->buttons() != Qt::RightButton && event->buttons() != Qt::LeftButton) {
        event->ignore();
    }
    lastMouseMovePos = event->pos();
}

void MainWindow::dragEnterEvent(QDragEnterEvent *e) {
    if(e->mimeData()->hasUrls()) {
        e->acceptProposedAction();
    }
}

void MainWindow::dropEvent(QDropEvent *event) {
    const QMimeData *mimeData = event->mimeData();
    // check for our needed mime type, here a file or a list of files
    if(mimeData->hasUrls()) {
        QStringList pathList;
        QList<QUrl> urlList = mimeData->urls();
        // extract the local paths of the files
        for(int i = 0; i < urlList.size() && i < 32; ++i) {
            pathList.append(urlList.at(i).toLocalFile());
        }
        // try to open first file in the list
        open(pathList.first());
    }
}

bool MainWindow::event(QEvent *event) {
    if(event->type() == QEvent::Move) {
        windowMoveTimer.start();
    }
    return (actionManager->processEvent(event)) ? true : QMainWindow::event(event);
}

void MainWindow::closeEvent(QCloseEvent *event) {
    this->hide();
    if(QThreadPool::globalInstance()->activeThreadCount()) {
        QThreadPool::globalInstance()->waitForDone();
    }
    if(!isFullScreen()) {
        saveWindowGeometry();
    }
    saveDisplay();
    QMainWindow::closeEvent(event);
}

void MainWindow::saveDisplay() {
    settings->setLastDisplay(desktopWidget->screenNumber(this));
}

void MainWindow::saveWindowGeometry() {
    #ifdef __linux__
        settings->setWindowGeometry(QRect(pos(), size()));
        //settings->setWindowGeometry(geometry());
    #else
         settings->setWindowGeometry(QRect(pos(), size()));
    #endif
}

void MainWindow::restoreWindowGeometry() {
    QRect geometry = settings->windowGeometry();
    this->resize(geometry.size());
    this->move(geometry.x(), geometry.y());
    updateCurrentDisplay();
}

void MainWindow::updateCurrentDisplay() {
    currentDisplay = desktopWidget->screenNumber(this);
}

MainWindow::~MainWindow() {
}

//#############################################################
//######################## OTHERS #############################
//#############################################################

void MainWindow::setInfoString(QString text) {
//    infoOverlay->setText(text);
    setWindowTitle(text);
}

void MainWindow::slotShowControls(bool x) {
//     if(x && (panelPosition == BOTTOM || !settings->panelEnabled()) )
//         m_AryControlsOverlay->show();
//     else
//         m_AryControlsOverlay->hide();
}

void MainWindow::slotShowInfo(bool x) {
//    x ? infoOverlay->show() : infoOverlay->hide();
}

//#############################################################
//###################### ACTION SLOTS #########################
//#############################################################

void MainWindow::switchFitMode() {
    if(fitMode == 0) {
        this->slotFitNormal();
    } else {
        this->slotFitAll();
    }
}

void MainWindow::slotFitAll() {
    fitMode = 0;
    emit signalFitAll();
}

void MainWindow::slotFitWidth() {
    fitMode = 1;
    emit signalFitWidth();
}

void MainWindow::slotFitNormal() {
    fitMode = 2;
    emit signalFitNormal();
}

void MainWindow::triggerFullscreen() {
    if(!this->isFullScreen()) {
        //do not save immediately on application start
        if(!this->isHidden())
            saveWindowGeometry();
        //move to target screen
        if(desktopWidget->screenCount() > currentDisplay &&
           currentDisplay != desktopWidget->screenNumber(this))
        {
            this->move(desktopWidget->screenGeometry(currentDisplay).x(),
                       desktopWidget->screenGeometry(currentDisplay).y());
        }
        this->showFullScreen();
        emit signalFullscreenEnabled(true);

    } else {
        this->showNormal();
        restoreWindowGeometry();
        this->activateWindow();
        this->raise();
        emit signalFullscreenEnabled(false);
    }
}

void MainWindow::slotMinimize() {
    this->setWindowState(Qt::WindowMinimized);
}

void MainWindow::slotResize(QSize newSize) {
}

void MainWindow::slotResizeDialog() {
    ResizeDialog resizeDialog(this);
    connect(&resizeDialog, SIGNAL(sizeSelected(QSize)), this, SLOT(slotResize(QSize)));
    resizeDialog.exec();
 }

void MainWindow::slotRotateLeft() {
    core->rotateImage(-90);
}

void MainWindow::slotRotateRight() {
    core->rotateImage(90);
}

void MainWindow::slotCrop() {
	this->slotFitAll();

	for (auto pImageViewer : m_AryImageViewers) {
		pImageViewer->crop();
	}
}

void MainWindow::slotSelectWallpaper() {
    this->slotFitAll();

	for (auto pImageViewer : m_AryImageViewers) {
		pImageViewer->selectWallpaper();
	}
}

void MainWindow::slotSaveDialog() {
    const QString imagesFilter = settings->supportedFormatsString();
    QString fileName = core->getCurrentFilePath();
    fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                                            fileName,
                                            imagesFilter);
    emit fileSaved(fileName);
}

void MainWindow::slotOpenDialog() {
    QFileDialog dialog;
    QStringList imageFilter;
    imageFilter.append(settings->supportedFormatsString());
    imageFilter.append("All Files (*)");
    QString lastDir = settings->lastDirectory();
    dialog.setDirectory(lastDir);
    dialog.setNameFilters(imageFilter);
    dialog.setWindowTitle("Open image");
    //dialog.setParent(this);
    dialog.setWindowModality(Qt::ApplicationModal);
    connect(&dialog, SIGNAL(fileSelected(QString)), this, SLOT(open(QString)));//this, SIGNAL(fileOpened(QString))); // FIX: use MainWindow::open(QString) instead
    dialog.exec();
}

void MainWindow::showSettings() {
    SettingsDialog settingsDialog;
    settingsDialog.exec();
}

/////////////////////////////////////////////////////////////////////////////////////////

void MainWindow::scalingFinished(QPixmap* pImage, int nIndex)
{
	if (nIndex < nImageViewers) {
		emit m_AryImageViewers[nIndex]->updateImage(pImage);
	}
}

/////////////////////////////////////////////////////////////////////////////////////////

void MainWindow::imageLoaded()
{
	emit signalNextImage();
}

/////////////////////////////////////////////////////////////////////////////////////////
