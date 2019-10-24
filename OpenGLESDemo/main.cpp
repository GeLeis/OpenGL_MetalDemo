//
//  main.cpp
//  OpenGLESDemo
//
//  Created by gelei on 2019/10/23.
//  Copyright © 2019 gelei. All rights reserved.
//

#include <stdio.h>
#include "GLTools.h"
#include <GLUT/GLUT.h>

GLBatch triangleBatch;
//固定着色器
GLShaderManager shaderManager;
//正方形边长
GLfloat blockSize = 0.1;
//顶点
GLfloat sVVerts[] = {
    -blockSize,-blockSize,0.f,
    blockSize,-blockSize,0.f,
    blockSize,blockSize,0.f,
    -blockSize,blockSize,0.f
};
//窗口大小改变时接受新的宽、高,和第一次创建
void ChangeISize(int w, int h) {
    glViewport(0, 0, w, h);
}
//为程序作一次性的设置
void SetupRC() {
    //设置背影颜色,放置在颜色缓存区里
    glClearColor(0.f, 0.f, 1.f, 1.f);
    //初始化着色管理器
    shaderManager.InitializeStockShaders();
    //设置三角形顶点,其中数组vVert包含所有3个顶点的x,y笛卡尔坐标对
    //坐标系从-1 - 1
    GLfloat vVerts[] = {
        -0.5f,0.f,0.f,
        0.5f,0.f,0.f,
        0.f,0.5f,0.f,
    };
    
    //批次处理,用不同的模型来绘图 ,3是几个顶点
    triangleBatch.Begin(GL_TRIANGLE_FAN, 4);
//    triangleBatch.Begin(GL_TRIANGLES, 3);
    triangleBatch.CopyVertexData3f(sVVerts);
    triangleBatch.End();
}

void SpecialKeys(int key, int x,int y) {
    GLfloat setpSize = 0.005f;
    GLfloat blockX = sVVerts[0];
    GLfloat blockY = sVVerts[10];
    if (key == GLUT_KEY_UP) {
        blockY += setpSize;
    }
    if (key == GLUT_KEY_DOWN) {
        blockY -= setpSize;
    }
    
    if (key == GLUT_KEY_RIGHT) {
        blockX += setpSize;
    }
    
    if (key == GLUT_KEY_LEFT) {
        blockX -= setpSize;
    }
    //A点
    sVVerts[0] = blockX;
    sVVerts[1] = blockY - blockSize * 2;
    
    sVVerts[3] = blockX + blockSize * 2;
    sVVerts[4] = blockY - blockSize * 2;
    
    sVVerts[6] = blockX + blockSize * 2;
    sVVerts[7] = blockY;
    
    sVVerts[9] = blockX;
    sVVerts[10] = blockY;
    
    triangleBatch.CopyVertexData3f(sVVerts);
    //触发renderScene
    glutPostRedisplay();
}

//开始渲染
void RenderScene(void) {
    //清除一个或一组特定的缓冲区
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    //设置一组浮点数来表示红色
    GLfloat vRed[] = {1.f,0.f,0.f,1.f};
    //使用固定着色器,GLT_SHADER_IDENTITY,单元着色器
    shaderManager.UseStockShader(GLT_SHADER_IDENTITY,vRed);
    //提交着色器,绘制
    triangleBatch.Draw();
    //将在后台缓冲区进行渲染,在结束时交换到前台
    glutSwapBuffers();
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
