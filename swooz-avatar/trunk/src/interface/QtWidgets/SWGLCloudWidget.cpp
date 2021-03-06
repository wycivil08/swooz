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
 * \file SWGLCloudWidget.cpp
 * \brief Defines SWGLCloudWidget
 * \author Florian Lance
 * \date 11/07/13
 */

#include "SWGLCloudWidget.h"

#include "moc_SWGLCloudWidget.cpp"

#include "interface/SWGLUtility.h"

using namespace swExcept;


SWGLCloudWidget::SWGLCloudWidget(QGLContext *context, QWidget* parent) :
    SWGLWidget(context, parent), m_bInitCamWithCloudPosition(true), m_bNewCloud(false), m_pCloud(NULL)
{
    m_fDefaultOpacity   = 1.f;
    m_fDepthRect        = -1.f;
}

SWGLCloudWidget::~SWGLCloudWidget()
{}

void SWGLCloudWidget::initializeGL()
{
    // set perspective
        m_rZNear = 0.01;
        m_rZFar  = 100.0;
        m_rFOV   = 40.0;

    // set background
        qglClearColor(QColor(49, 53, 70));

    // init shaders
        initShaders("../data/shaders/cloudAvatar.vert", "../data/shaders/cloudAvatar.frag", m_oShader,    false);

    // set options
    //  depth buffer
        glEnable(GL_DEPTH_TEST);
    //  transparency
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //  line smoothing
        glEnable(GL_LINE_SMOOTH);
    //  back face culling
        glDisable(GL_CULL_FACE);

    // init QGL buffers
        initCloudBuffers();

    // start timer
        m_oTimer->start(1000/30, this);
}

void SWGLCloudWidget::paintGL()
{
    // clear color and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // calculate model view transformation
    QMatrix4x4 l_oViewMatrix;
    l_oViewMatrix.setToIdentity();

    // set camera vue
    l_oViewMatrix.lookAt( m_pCamera->eyePosition(), m_pCamera->lookAt(), m_pCamera->up());

    // comput MVP matrix
    QMatrix4x4 l_oModelMatrix;
    l_oModelMatrix.setToIdentity();
    m_oMVPMatrix = l_oModelMatrix  * m_oProjectionMatrix * l_oViewMatrix;

    // draw geometry
    if(m_pCloud)
    {
        if(m_pCloud->size() > 0)
        {
            drawCloud(m_oShader, m_glFSizePoint, m_oMVPMatrix);
            swCloud::SWCloudBBox l_oBBox = m_pCloud->bBox();
            drawDepthRect(m_oShader, m_oMVPMatrix, l_oBBox.m_fMinZ + m_fDepthRect);
        }
    }
}

void SWGLCloudWidget::setCloud(swCloud::SWCloud *oCloud, bool bResetCamera)
{
    if(oCloud)
    {
        if(oCloud->size() == 0)
        {
            return;
        }
    }
    else
    {
        return;
    }

    if(m_pCamera->cameraMode() == SWQtCamera::TRACKBALL_CAMERA && bResetCamera)
    {
        std::vector<float> l_v3FMean = oCloud->meanPoint();
        QVector3D l_oEye,l_oLookAt;
        l_oLookAt.setX(l_v3FMean[0]);
        l_oLookAt.setY(l_v3FMean[1]);
        l_oLookAt.setZ(l_v3FMean[2]);
        l_oEye = l_oLookAt;
        l_oEye.setZ(l_oEye.z() - 0.25f);

        setCamera(l_oEye,l_oLookAt);
        setCameraInitial(l_oEye,l_oLookAt, QVector3D(0,1,0));
    }
    else if(m_pCamera->cameraMode() == SWQtCamera::FPS_CAMERA && bResetCamera)
    {
        swCloud::SWCloudBBox l_oBBox = oCloud->bBox();
        QVector3D l_oEye,l_oLookAt;
        l_oEye.setX((l_oBBox.m_fMaxX + l_oBBox.m_fMinX)/2);
        l_oEye.setY((l_oBBox.m_fMaxY + l_oBBox.m_fMinY)/2);
        l_oEye.setZ((l_oBBox.m_fMaxZ + l_oBBox.m_fMinZ)/2);
        l_oLookAt = l_oEye;
        l_oEye.setZ(l_oEye.z() + 0.25f);
        l_oLookAt.setZ(l_oLookAt.z() + 1.f);

        setCamera(l_oEye,l_oLookAt);
        setCameraInitial(l_oEye,l_oLookAt, QVector3D(0,1,0));
    }

    deleteAndNullify(m_pCloud);
    m_pCloud = new swCloud::SWCloud(*oCloud);
    deleteAndNullify(oCloud);
    m_bNewCloud = true;

    updateGL();
}

void SWGLCloudWidget::initCloudBuffers()
{
    initIndexBuffer(m_indexBufferCloud);
    initVertexBuffer(m_vertexBufferCloud);
    initVertexBuffer(m_colorBufferCloud);

    m_indexBufferCloud.setUsagePattern(QGLBuffer::StaticDraw);
    m_vertexBufferCloud.setUsagePattern(QGLBuffer::StaticDraw);
    m_colorBufferCloud.setUsagePattern(QGLBuffer::StaticDraw);
}


void SWGLCloudWidget::drawDepthRect(QGLShaderProgram &oShader, QMatrix4x4 &mvpMatrix, cfloat fDepth)
{
    // bind shader
    oShader.bind();
        checkGlError();

    // set mode
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // release buffers
    QGLBuffer::release(QGLBuffer::VertexBuffer);
    QGLBuffer::release(QGLBuffer::IndexBuffer);

    // allocate buffers
    int l_i32SizeIndex      = sizeof(GLuint);
    int l_i32SizeVertex     = sizeof(float) * 3;
    uint32 *l_aDepthRectI   = new uint32[6];
    float  *l_aDepthRectV   = new float[3*4];

    // defines points    
    l_aDepthRectV[2] = l_aDepthRectV[5] = l_aDepthRectV[8] = l_aDepthRectV[11] = fDepth;
    l_aDepthRectV[0] = m_oCloudBBox.m_fMinX; l_aDepthRectV[3] = m_oCloudBBox.m_fMinX; l_aDepthRectV[6] = m_oCloudBBox.m_fMaxX; l_aDepthRectV[9]  = m_oCloudBBox.m_fMaxX;
    l_aDepthRectV[1] = m_oCloudBBox.m_fMaxY; l_aDepthRectV[4] = m_oCloudBBox.m_fMinY; l_aDepthRectV[7] = m_oCloudBBox.m_fMinY; l_aDepthRectV[10] = m_oCloudBBox.m_fMaxY;
    // define index
    l_aDepthRectI[0] = 0;
    l_aDepthRectI[1] = 1;
    l_aDepthRectI[2] = 2;
    l_aDepthRectI[3] = 2;
    l_aDepthRectI[4] = 3;
    l_aDepthRectI[5] = 0;

    QGLBuffer l_indexBufferDepthRect,l_vertexBufferDepthRect;
    initIndexBuffer(l_indexBufferDepthRect);
    initVertexBuffer(l_vertexBufferDepthRect);

    allocateBuffer(l_indexBufferDepthRect,  l_aDepthRectI, 6 * l_i32SizeIndex);
    allocateBuffer(l_vertexBufferDepthRect, l_aDepthRectV, 4 * l_i32SizeVertex);

    deleteAndNullifyArray(l_aDepthRectI);
    deleteAndNullifyArray(l_aDepthRectV);

    // set uniform values parameters
    oShader.setUniformValue("displayMode", 1);
    oShader.setUniformValue("uniColor", 22, 39, 51);
    oShader.setUniformValue("mvpMatrix", mvpMatrix);
    oShader.setUniformValue("opacity", 0.8f);

    // draw primitives
        drawBuffer(l_indexBufferDepthRect, l_vertexBufferDepthRect, oShader, GL_TRIANGLES);

    oShader.release();
        checkGlError();
}

void SWGLCloudWidget::setDepthRect(const double dDepth)
{
    m_fDepthRect = static_cast<float>(dDepth);
}


void SWGLCloudWidget::drawCloud(QGLShaderProgram &oShader, cfloat fSizePoint, QMatrix4x4 &mvpMatrix)
{   
    // bind shader
    oShader.bind();
        checkGlError();

    // set mode
    glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);

    // release buffers
    QGLBuffer::release(QGLBuffer::VertexBuffer);
    QGLBuffer::release(QGLBuffer::IndexBuffer);

    // set size of the points
    glPointSize(fSizePoint);

    // set uniform values parameters
    oShader.setUniformValue("displayMode", 0);
    oShader.setUniformValue("mvpMatrix", mvpMatrix);
    oShader.setUniformValue("opacity", m_fDefaultOpacity);

    if(m_bNewCloud)
    {
        // allocate buffers
        int l_i32SizeIndex      = sizeof(GLuint);
        int l_i32SizeVertex     = sizeof(float) * 3;
        uint32 *l_aCloudI       = m_pCloud->indexBuffer();
        float  *l_aCloudV       = m_pCloud->vertexBuffer();
        float  *l_aCloudC       = m_pCloud->colorBuffer();

        allocateBuffer(m_indexBufferCloud,  l_aCloudI, m_pCloud->size() * l_i32SizeIndex);
        allocateBuffer(m_vertexBufferCloud, l_aCloudV, m_pCloud->size() * l_i32SizeVertex);
        allocateBuffer(m_colorBufferCloud  ,l_aCloudC, m_pCloud->size() * l_i32SizeVertex);

        deleteAndNullifyArray(l_aCloudI);
        deleteAndNullifyArray(l_aCloudV);
        deleteAndNullifyArray(l_aCloudC);

        m_bNewCloud = false;
    }


    // draw primitives
        drawBufferWithColor(m_indexBufferCloud, m_vertexBufferCloud, m_colorBufferCloud, oShader, GL_POINTS);

    oShader.release();
        checkGlError();
}
