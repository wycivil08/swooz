/*******************************************************************************
**                                                                            **
**  SWoOz is a software platform written in C++ used for behavioral           **
**  experiments based on interactions between people and robots               **
**  or 3D avatars.                                                            **
**                                                                            **
**  This program is free software: you can redistribute it and/or modify      **
**  it under the terms of the GNU Lesser General Public License as published  **
**  by the Free Software Foundation, either version 3 of the License, or      **
**  (at your option) any later version.                                       **
**                                                                            **
**  This program is distributed in the hope that it will be useful,           **
**  but WITHOUT ANY WARRANTY; without even the implied warranty of            **
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             **
**  GNU Lesser General Public License for more details.                       **
**                                                                            **
**  You should have received a copy of the GNU Lesser General Public License  **
**  along with Foobar.  If not, see <http://www.gnu.org/licenses/>.           **
**                                                                            **
** *****************************************************************************
**          Authors: Guillaume Gibert, Florian Lance                          **
**  Website/Contact: http://swooz.free.fr/                                    **
**       Repository: https://github.com/GuillaumeGibert/swooz                 **
********************************************************************************/

/**
 * \file SWViewerInterface.h
 * \brief Defines SWViewerInterface
 * \author Florian Lance
 * \date 02/06/14
 */

#ifndef _SWVIEWERINTERFACE_
#define _SWVIEWERINTERFACE_

#include <QMainWindow>
#include <QThread>

//#include "SWViewerWorker.h"

#include "SWUI_Viewer.h"

#include "interface/QtWidgets/SWGLMultiObjectWidget.h"

#include "animation/SWAnimation.h"



namespace Ui {
    class SWUI_Viewer;
}

class SWViewerWorker;

/**
 * \class SWViewerInterface
 * \brief ...
 * \author Florian Lance
 * \date 02/06/14
 */
class SWViewerInterface : public QMainWindow
{
    Q_OBJECT

    public :


        // ############################################# CONSTRUCTORS / DESTRUCTORS

        /**
         * \brief Constructor of SWViewerInterface
         */
        SWViewerInterface(QApplication *parent);

        /**
         * \brief Destructor of SWViewerInterface
         */
        ~SWViewerInterface();


        // ############################################# METHODS

    public slots:

        /**
         * @brief openOnlineDocumentation
         */
        void openOnlineDocumentation();

        /**
         * @brief openAboutWindow
         */
        void openAboutWindow();

        /**
         * @brief closeEvent
         * @param event
         */
        void closeEvent(QCloseEvent *event);

        /**
         * @brief loadCloud
         */
        void loadCloud();

        /**
         * @brief loadMesh
         */
        void loadMesh();

        /**
         * @brief deleteCloud
         */
        void deleteCloud();

        /**
         * @brief deleteMesh
         */
        void deleteMesh();

        /**
         * @brief updateParameters
         * @param i32Inused
         */
        void updateParameters(int i32Inused);

        /**
         * @brief updateParameters
         * @param dInused
         */
        void updateParameters(double dInused);

        /**
         * @brief updateParameters
         * @param sInused
         */
        void updateParameters(QString sInused);

        /**
         * @brief updateParameters
         */
        void updateParameters();

        void updateCloudInterfaceParameters(QListWidgetItem*);

        void updateMeshInterfaceParameters(QListWidgetItem*);

        void updateCloudInterfaceParameters();

        void updateMeshInterfaceParameters();

        void updateInterfaceParameters();

        void setTexture();

        /**
         * @brief setCameraToCurrentItem
         */
        void setCameraToCurrentItem();

        /**
         * @brief enableFullScreen
         */
        void enableGLFullScreen();

        /**
         * @brief disableFullScreen
         */
        void disableGLFullScreen();

        /**
         * @brief loadMod
         */
        void loadModFile();

        /**
         * @brief loadSeq
         */
        void loadSeqFile();

        /**
         * @brief loadMeshCorrFile
         */
        void loadMeshCorrFile();


        /**
         * @brief updateAnimationPathFileDisplay
         * @param modFilePath
         * @param seqFilePath
         * @param corrFilePath
         */
        void updateAnimationPathFileDisplay(QString modFilePath, QString seqFilePath, QString corrFilePath);

        /**
         * @brief SWViewerInterface::updateCloudAnimationPath
         */
        void updateCloudAnimationPath(QListWidgetItem *);

        /**
         * @brief updateMeshAnimationPath
         */
        void updateMeshAnimationPath(QListWidgetItem *);


    signals:

        /**
         * @brief stopLoop
         */
        void stopLoop();

        /**
         * @brief addAnimation
         */
        void addAnimation(bool);

        /**
         * @brief deleteAnimation
         */
        void deleteAnimation(bool, int);


        /**
         * @brief setModFilePath
         */
        void setModFilePath(bool, int, QString);

        /**
         * @brief setSeqFilePath
         */
        void setSeqFilePath(bool, int, QString);

        /**
         * @brief setCorrFilePath
         */
        void setCorrFilePath(bool, int, QString);

        /**
         * @brief cloudCurrentRowChanged
         */
        void cloudCurrentRowChanged(int);

        /**
         * @brief meshCurrentRowChanged
         */
        void meshCurrentRowChanged(int);


    private :

        QString m_absolutePath;

        bool m_bIsCloudLastSelection;
        bool m_bDesactiveUpdateParameters;


        bool m_bGLFullScreen;



        QWidget *m_pGLContainer;

        // widgets & ui
        Ui::SWUI_Viewer* m_uiViewer;   /**< qt main window */

        SWGLMultiObjectWidget *m_pGLMultiObject; /**< ... */

        // threads & workers
        SWViewerWorker  *m_pWViewer;    /**< viewer worker */
        QThread         m_TViewer;    /**< viewer thread */

};



/**
 * @brief The SWViewerWorker class
 */
class SWViewerWorker : public QObject
{
    Q_OBJECT

    public :

        /**
         * \brief constructor of SWViewerWorker
         */
        SWViewerWorker();

        /**
         * \brief destructor of SWViewerWorker
         */
//        ~SWViewerWorker();


    public slots:


        /**
         * @brief startLoop
         */
        void startLoop();

        /**
         * @brief stopLoop
         */
        void stopLoop();

        /**
         * @brief updateCloudAnimationPath
         */
        void updateCloudAnimationPath(int indexCloud);

        /**
         * @brief updateMeshAnimationPath
         */
        void updateMeshAnimationPath(int indexMesh);

        /**
         * @brief setModFile
         * @param isCloud
         * @param indexObject
         * @param pathFile
         */
        void setModFile(bool isCloud, int indexObject, QString pathFile);

        /**
         * @brief setSeqFile
         * @param isCloud
         * @param indexObject
         * @param pathFile
         */
        void setSeqFile(bool isCloud, int indexObject, QString pathFile);

        /**
         * @brief setCorrFilePath
         * @param isCloud
         * @param indexObject
         * @param pathFile
         */
        void setCorrFilePath(bool isCloud, int indexObject, QString pathFile);

        /**
         * @brief addAnimation
         * @param isCloud
         */
        void addAnimation(bool isCloud);

        /**
         * @brief deleteAnimation
         * @param isCLoud
         * @param indexObject
         */
        void deleteAnimation(bool isCLoud, int indexObject);

    signals:

        /**
         * @brief sendAnimationPathFile
         */
        void sendAnimationPathFile(QString, QString, QString);

        /**
         * @brief startAnimation
         */
        void startAnimation(bool, int);

        /**
         * @brief sendOffsetAnimation
         */
//        void sendOffsetAnimation(bool, int, QVector<float>, QVector<float>, QVector<float>, QVector<float>, int);
        void sendOffsetAnimation(SWAnimationSendDataPtr);


        /**
         * @brief drawSceneSignal
         */
        void drawSceneSignal();

    private :


        int m_i32LoopPeriod;    /**< loop period */

        bool m_bDoLoop;

        QReadWriteLock m_oLoopMutex;    /**< mutex for the main worker loop */

        QList<swAnimation::SWAnimation> m_vCloudsAnimation; /**< ... */
        QList<swAnimation::SWAnimation> m_vMeshesAnimation;  /**< ... */

        QList<QString> m_cloudModFileName;
        QList<QString> m_cloudSeqFileName;
        QList<QString> m_cloudCorrFileName;

        QList<QString> m_meshModFileName;
        QList<QString> m_meshSeqFileName;        
        QList<QString> m_meshCorrFileName;

        QReadWriteLock m_oMutex;                /**< ... */
};


#endif // SWVIEWERINTERFACE_H
