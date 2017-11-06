#ifndef GLWIDGET_H
#define GLWIDGET_H
#include "GL/glew.h"
#include <QGLWidget>
#include <QTimer>
#include <QTime>
#include "hairCommon.h"

class ObjMesh;
class HairObject;
class Simulation;
class ShaderProgram;
class HairInterface;
class Texture;
class Framebuffer;
class SceneEditor;
class Tessellator;

class GLWidget : public QGLWidget
{
    Q_OBJECT
    friend class SceneWidget;
    friend class HairInterface;
    friend class SceneEditor;

public:
    GLWidget(QGLFormat format, HairInterface *hairInterface, QWidget *parent = 0);
    ~GLWidget();

    void resetSimulation(bool hardReset = false);
    void applySceneEditor(Texture *_hairGrowthTexture, Texture *_hairGroomingTexture);

    void pause();
    void unpause();
    bool isPaused();
    void forceUpdate(); // Redraws the scene if paused.

    bool useShadows = true;
    bool useSupersampling = true;
    bool useFrictionSim = true;
    bool useTransparency = true;

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

    void initSimulation();
    void initCamera();

protected slots:
    /** Repaints the canvas. Called 60 times per second by m_timer. */
    void updateCanvas();

private:
    void _drawHair(ShaderProgram *program, glm::mat4 model, glm::mat4 view, glm::mat4 projection, bool bindProgram = true);
    void _drawMesh(ShaderProgram *program, glm::mat4 model, glm::mat4 view, glm::mat4 projection);

    void _drawHairFromFeedback(ShaderProgram *program, glm::mat4 model, glm::mat4 view, glm::mat4 projection);

    void _resizeDepthPeelFramebuffers();

    //bool m_paused = false;   // pause the simulation for USC dataset
    bool m_paused = true;      //
    bool m_pausedLastFrame = true;

    HairInterface *m_hairInterface;
    SceneEditor *m_sceneEditor;

    ObjMesh *m_highResMesh, *m_lowResMesh;
    HairObject *m_hairObject;
    Simulation *m_testSimulation;

    Tessellator *m_tessellator;

    Texture *m_noiseTexture;

    std::vector<ShaderProgram*> m_programs;
    ShaderProgram *m_hairProgram,
                  *m_meshProgram,
                  *m_hairOpacityProgram,
                  *m_whiteMeshProgram,
                  *m_whiteHairProgram,
                  *m_hairDepthPeelProgram,
                  *m_meshDepthPeelProgram,

                  // TRANSFORM FEEDBACK
                  *m_TFwhiteHairProgram,
                  *m_TFhairDepthPeelProgram,
                  *m_TFhairOpacityProgram,
                  *m_TFhairProgram;

    std::vector<Framebuffer*> m_framebuffers;
    Framebuffer *m_hairShadowFramebuffer,
                *m_meshShadowFramebuffer,
                *m_opacityMapFramebuffer,
                *m_finalFramebuffer,
                *m_depthPeel0Framebuffer,
                *m_depthPeel1Framebuffer;

    // Camera parameters
    glm::mat4 m_projection, m_view;
    float m_zoom, m_angleX, m_angleY;
    QPoint m_prevMousePos;
    QPoint m_prevXformPos;
    QPoint m_prevRotPos;

    // Light parameters
    glm::vec3 m_lightPosition;
    glm::mat4 m_eyeToLight;

    float m_hairDensity;
    float m_maxHairLength;

    QTime m_clock;
    QTimer m_timer; /** Timer calls tick() 60 times per second. */
    int m_increment; /** Incremented on every call to paintGL. */
    float m_targetFPS;

    int msec;

    Texture *resetFromSceneEditorGrowthTexture, *resetFromSceneEditorGroomingTexture;
};

#endif // GLWIDGET_H
