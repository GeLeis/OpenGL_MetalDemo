//
//  main.cpp
//  OpenGLESDemo
//
//  Created by gelei on 2019/10/23.
//  Copyright © 2019 gelei. All rights reserved.
//

#include <stdio.h>
#include "GLTools.h"
//矩阵堆栈
#include "GLMatrixStack.h"
#include <GLUT/GLUT.h>
//投影矩阵
#include "GLFrustum.h"
//GLFrame
#include "GLFrame.h"
#include "GLBatch.h"
#include "GLGeometryTransform.h"
#include <math3d.h>

//固定着色器
GLShaderManager shaderManager;
//模型矩阵
GLMatrixStack modelViewMatrix;
//投影矩阵
GLMatrixStack projectionMatrix;
GLFrame cameraFrame;
GLFrame objectFrame;
//投影矩阵
GLFrustum viewFrustum;
//容器类
GLBatch pointBatch;
GLBatch lineBatch;
GLBatch lineStripBatch;
GLBatch lineLoopBatch;
GLBatch triangleBatch;
GLBatch triangleStringBatch;
GLBatch triangleFanBatch;
//几何变换的管道
GLGeometryTransform transformPipeline;
M3DMatrix44f shadowMatrix;

GLfloat vGreen[] = {0.f,1.f,0.f,1.f};
GLfloat vBlack[] = {0.f,0.f,0.f,1.f};

//正方形边长
GLfloat blockSize = 0.1;
//顶点
GLfloat sVVerts[] = {
    -blockSize,-blockSize,0.f,
    blockSize,-blockSize,0.f,
    blockSize,blockSize,0.f,
    -blockSize,blockSize,0.f
};

//矩阵坐标
GLfloat ypos = 0.f;
GLfloat xpos = 0.f;

// 跟踪效果步骤
int nStep = 0;

//窗口大小改变时接受新的宽、高,和第一次创建
void ChangeISize(int w, int h) {
    glViewport(0, 0, w, h);
    //创建投影矩阵 ,35角度,(w)/h纵横比,1.f近平面,500,远平面
    viewFrustum.SetPerspective(35.0, float(w)/h, 1.f, 500.f);
    projectionMatrix.LoadMatrix(viewFrustum.GetProjectionMatrix());
    
    //模型视图矩阵
    modelViewMatrix.LoadIdentity();
}
//为程序作一次性的设置
void SetupRC() {
    
    //定义一些点，类似佛罗里达州的形状。
    GLfloat vCoast[24][3] = {
        {2.80, 1.20, 0.0 }, {2.0,  1.20, 0.0 },
        {2.0,  1.08, 0.0 },  {2.0,  1.08, 0.0 },
        {0.0,  0.80, 0.0 },  {-.32, 0.40, 0.0 },
        {-.48, 0.2, 0.0 },   {-.40, 0.0, 0.0 },
        {-.60, -.40, 0.0 },  {-.80, -.80, 0.0 },
        {-.80, -1.4, 0.0 },  {-.40, -1.60, 0.0 },
        {0.0, -1.20, 0.0 },  { .2, -.80, 0.0 },
        {.48, -.40, 0.0 },   {.52, -.20, 0.0 },
        {.48,  .20, 0.0 },   {.80,  .40, 0.0 },
        {1.20, .80, 0.0 },   {1.60, .60, 0.0 },
        {2.0, .60, 0.0 },    {2.2, .80, 0.0 },
        {2.40, 1.0, 0.0 },   {2.80, 1.0, 0.0 }};
    
    //通过三角形创建金字塔
    GLfloat vPyramid[12][3] = {
        -2.0f, 0.0f, -2.0f,
        2.0f, 0.0f, -2.0f,
        0.0f, 4.0f, 0.0f,
        
        2.0f, 0.0f, -2.0f,
        2.0f, 0.0f, 2.0f,
        0.0f, 4.0f, 0.0f,
        
        2.0f, 0.0f, 2.0f,
        -2.0f, 0.0f, 2.0f,
        0.0f, 4.0f, 0.0f,
        
        -2.0f, 0.0f, 2.0f,
        -2.0f, 0.0f, -2.0f,
        0.0f, 4.0f, 0.0f
    };
    
    //设置背影颜色,放置在颜色缓存区里
    glClearColor(0.7f, 0.7f, 0.7f, 1.f);
    //初始化着色管理器
    shaderManager.InitializeStockShaders();
    //开启深度测试
    glEnable(GL_DEPTH_TEST);
    
    //设置变换管线以使用两个矩阵堆栈
    transformPipeline.SetMatrixStacks(modelViewMatrix, projectionMatrix);
    
    cameraFrame.MoveForward(-15.0);
    
    //设置三角形顶点,其中数组vVert包含所有3个顶点的x,y笛卡尔坐标对
    //坐标系从-1 - 1
    GLfloat vVerts[] = {
        -0.5f,0.f,0.f,
        0.5f,0.f,0.f,
        0.f,0.5f,0.f,
    };
    
    //画图形
    //点方式
    //a表示要使用的模式,b表示顶点
    pointBatch.Begin(GL_POINTS, 24);
    pointBatch.CopyVertexData3f(vCoast);
    pointBatch.End();
    
    //线方式
    lineBatch.Begin(GL_LINES, 24);
    lineBatch.CopyVertexData3f(vCoast);
    lineBatch.End();
    
    //线段
    lineStripBatch.Begin(GL_LINE_STRIP, 24);
    lineStripBatch.CopyVertexData3f(vCoast);
    lineStripBatch.End();
    
    //线环
    lineLoopBatch.Begin(GL_LINE_LOOP, 24);
    lineLoopBatch.CopyVertexData3f(vCoast);
    lineLoopBatch.End();
    
    //金字塔
    triangleBatch.Begin(GL_TRIANGLES, 12);
//    triangleBatch.Begin(GL_TRIANGLES, 3);
    triangleBatch.CopyVertexData3f(vPyramid);
    triangleBatch.End();
    
    //三角形扇子
    GLfloat vPoints[100][3];
    int nVerts = 0;
    //半径
    GLfloat r = 3.0f;
    vPoints[nVerts][0] = 0.f;
    vPoints[nVerts][1] = 0.f;
    vPoints[nVerts][2] = 0.f;
    for (GLfloat angel = 0; angel < M3D_2PI; angel+=M3D_2PI/6.0) {
        nVerts++;
        vPoints[nVerts][0] = float(cos(angel) * r);
        vPoints[nVerts][1] = float(sin(angel) * r);
        vPoints[nVerts][2] = -0.5;
    }
    //闭合
    nVerts++;
    vPoints[nVerts][0] = r;
    vPoints[nVerts][1] = r;
    vPoints[nVerts][2] = r;
    triangleBatch.Begin(GL_TRIANGLE_FAN, 8);
    triangleBatch.CopyVertexData3f(vPoints);
    triangleBatch.End();
}

void SpecialKeys(int key, int x,int y) {
//    GLfloat setpSize = 0.025f;
////    GLfloat blockX = sVVerts[0];
////    GLfloat blockY = sVVerts[10];
//    if (key == GLUT_KEY_UP) {
//        ypos += setpSize;
//    }
//    if (key == GLUT_KEY_DOWN) {
//        ypos -= setpSize;
//    }
//
//    if (key == GLUT_KEY_RIGHT) {
//        xpos += setpSize;
//    }
//
//    if (key == GLUT_KEY_LEFT) {
//        xpos -= setpSize;
//    }
//    //边界检测,这里只检测最左侧
//    if (xpos < -1.f + blockSize) {
//        xpos = -1.f + blockSize;
//    }
//
//    //A点
//    sVVerts[0] = blockX;
//    sVVerts[1] = blockY - blockSize * 2;
//
//    sVVerts[3] = blockX + blockSize * 2;
//    sVVerts[4] = blockY - blockSize * 2;
//
//    sVVerts[6] = blockX + blockSize * 2;
//    sVVerts[7] = blockY;
//
//    sVVerts[9] = blockX;
//    sVVerts[10] = blockY;
//
//    triangleBatch.CopyVertexData3f(sVVerts);
//    //触发renderScene
    
    //移动物体
    if (key == GLUT_KEY_UP) {
        objectFrame.RotateWorld(m3dDegToRad(-5.f), 1.f, 0, 0);
    } else if (key == GLUT_KEY_DOWN) {
        objectFrame.RotateWorld(m3dDegToRad(5.f), 1.f, 0, 0);
    } else if (key == GLUT_KEY_LEFT) {
        objectFrame.RotateWorld(m3dDegToRad(-5.f), 0.f, 1.f, 0);
    } else if (key == GLUT_KEY_RIGHT) {
        objectFrame.RotateWorld(m3dDegToRad(5.f), 0.f, 1.f, 0);
    }
    glutPostRedisplay();
}

void DrawWireFramedBatch(GLBatch* pBatch) {
    /*------------画绿色部分----------------*/
    /* GLShaderManager 中的Uniform 值——平面着色器
     参数1：平面着色器
     参数2：运行为几何图形变换指定一个 4 * 4变换矩阵
          --transformPipeline 变换管线（指定了2个矩阵堆栈）
     参数3：颜色值
    */
    shaderManager.UseStockShader(GLT_SHADER_FLAT, transformPipeline.GetModelViewProjectionMatrix(), vGreen);
    pBatch->Draw();
    
    
    
    /*-----------边框部分-------------------*/
    /*
        glEnable(GLenum mode); 用于启用各种功能。功能由参数决定
        参数列表：http://blog.csdn.net/augusdi/article/details/23747081
        注意：glEnable() 不能写在glBegin() 和 glEnd()中间
        GL_POLYGON_OFFSET_LINE  根据函数glPolygonOffset的设置，启用线的深度偏移
        GL_LINE_SMOOTH          执行后，过虑线点的锯齿
        GL_BLEND                启用颜色混合。例如实现半透明效果
        GL_DEPTH_TEST           启用深度测试 根据坐标的远近自动隐藏被遮住的图形（材料
     
     
        glDisable(GLenum mode); 用于关闭指定的功能 功能由参数决定
     
     */
    
    //画黑色边框
    glPolygonOffset(-1.0f, -1.0f);// 偏移深度，在同一位置要绘制填充和边线，会产生z冲突，所以要偏移
    glEnable(GL_POLYGON_OFFSET_LINE);
    
    // 画反锯齿，让黑边好看些
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    //绘制线框几何黑色版 三种模式，实心，边框，点，可以作用在正面，背面，或者两面
    //通过调用glPolygonMode将多边形正面或者背面设为线框模式，实现线框渲染
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //设置线条宽度
    glLineWidth(2.5f);
    
    /* GLShaderManager 中的Uniform 值——平面着色器
     参数1：平面着色器
     参数2：运行为几何图形变换指定一个 4 * 4变换矩阵
         --transformPipeline.GetModelViewProjectionMatrix() 获取的
          GetMatrix函数就可以获得矩阵堆栈顶部的值
     参数3：颜色值（黑色）
     */
    
    shaderManager.UseStockShader(GLT_SHADER_FLAT, transformPipeline.GetModelViewProjectionMatrix(), vBlack);
    pBatch->Draw();

    // 复原原本的设置
    ////通过调用glPolygonMode将多边形正面或者背面设为全部填充模式
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDisable(GL_POLYGON_OFFSET_LINE);
    glLineWidth(1.0f);
    glDisable(GL_BLEND);
    glDisable(GL_LINE_SMOOTH);
    
    
}


//开始渲染
void RenderScene(void) {
    //清除一个或一组特定的缓冲区
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    //压栈
    modelViewMatrix.PushMatrix();
    M3DMatrix44f mCamera;
    
    cameraFrame.GetCameraMatrix(mCamera);
    //矩阵相乘
    modelViewMatrix.MultMatrix(mCamera);
    M3DMatrix44f mObjectFrame;
    //只要使用GetMatrix就可以获取矩阵堆栈顶部的值,这个函数可以进行2次重载,用来使用GLShaderManager的使用,或者是获取顶部矩阵的顶点副本数据
    objectFrame.GetMatrix(mObjectFrame);
    modelViewMatrix.MultMatrix(mObjectFrame);
    shaderManager.UseStockShader(GLT_SHADER_FLAT,transformPipeline.GetModelViewProjectionMatrix(),vBlack);
    
    
    switch (nStep) {
        case 0:
            glPointSize(4);
            pointBatch.Draw();
            glPointSize(1.f);
            break;
        case 1:
            //设置线的宽度
            glLineWidth(2.0f);
            lineBatch.Draw();
            glLineWidth(1.0f);
            break;
        case 2:
            glLineWidth(2.0f);
            lineStripBatch.Draw();
            glLineWidth(1.0f);
            break;
        case 3:
            glLineWidth(2.0f);
            lineLoopBatch.Draw();
            glLineWidth(1.0f);
            break;
        case 4:
            DrawWireFramedBatch(&triangleBatch);
            break;
        case 5:
            DrawWireFramedBatch(&triangleStringBatch);
            break;
        case 6:
            DrawWireFramedBatch(&triangleFanBatch);
            break;
            
        default:
            break;
    }
    //将在后台缓冲区进行渲染,在结束时交换到前台
    glutSwapBuffers();
}
//根据空格次数。切换不同的“窗口名称”
void KeyPressFunc(unsigned char key, int x, int y)
{
    if(key == 32)
    {
        nStep++;
        
        if(nStep > 6)
            nStep = 0;
    }
    
    switch(nStep)
    {
        case 0:
            glutSetWindowTitle("GL_POINTS");
            break;
        case 1:
            glutSetWindowTitle("GL_LINES");
            break;
        case 2:
            glutSetWindowTitle("GL_LINE_STRIP");
            break;
        case 3:
            glutSetWindowTitle("GL_LINE_LOOP");
            break;
        case 4:
            glutSetWindowTitle("GL_TRIANGLES");
            break;
        case 5:
            glutSetWindowTitle("GL_TRIANGLE_STRIP");
            break;
        case 6:
            glutSetWindowTitle("GL_TRIANGLE_FAN");
            break;
    }
    
    glutPostRedisplay();
}


int main(int argc,char* argv[]) {
    //固定代码
    //设置当前的工作目录,针对Mac os
    gltSetWorkingDirectory(argv[0]);
    //初始化glut库
    glutInit(&argc, argv);
    //需要用到颜色
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH|GLUT_STENCIL);
    //初始化窗口大小
    glutInitWindowSize(800,600);
    //窗口名称
    glutCreateWindow("Triangle");
    
    //注册回调函数
    //窗口大小改变的时候回调
    glutReshapeFunc(ChangeISize);
    //渲染函数
    glutDisplayFunc(RenderScene);
    //特殊键位
    glutSpecialFunc(SpecialKeys);
    glutKeyboardFunc(KeyPressFunc);
    //鼠标方法
//    glutMouseFunc(<#void (*func)(int, int, int, int)#>)
    //驱动程序的初始化中没有出现任何问题。
    GLenum err = glewInit();
    
    if(GLEW_OK != err) {
        
        fprintf(stderr,"glew error:%s\n",glewGetErrorString(err));
        
        return 1;
        
    }
    
    //调用SetupRC,设置我们的渲染环境
    SetupRC();
    
    glutMainLoop();
    
    return 0;
}
