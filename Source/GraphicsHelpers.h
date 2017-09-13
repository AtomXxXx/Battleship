#pragma once
#include <GL/freeglut.h>
#include <GL/gl.h>


class GraphicsHelpers
{
public:

    static const int ASPECT_WIDTH = 16;
    static const int ASPECT_HEIGHT = 9;

public:

    static int WorldToPixels(float lengthX)
    {
        GLint viewport[4]; // x, y, width, height
        glGetIntegerv(GL_VIEWPORT, viewport);

        float aspectRatio = (float)ASPECT_WIDTH/ASPECT_HEIGHT;
        int widthInPixels = viewport[3] * (1/aspectRatio) * lengthX;
        return widthInPixels;
    }

    static void DrawString(float posX, float posY, float scaleX, float scaleY, float lineWidth, int r, int g, int b, const char* str, void* font = GLUT_STROKE_ROMAN)
    {
        glColor3f(r, g, b);
        //glRasterPos2f(posX, posY);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glTranslatef(posX, posY, 0);
        glRotatef(0.0, 0.0, 0.0, 1.0);
        glScalef(scaleX, scaleY, 0.0);

        glLineWidth(WorldToPixels(lineWidth));

        glutStrokeString(font, (const unsigned char*)str);
    }

    static void DrawLine(float posX, float posY, float length, float thickness, float angle, float color[3])
    {
        int widthInPixels = WorldToPixels(thickness);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        glTranslatef(posX, posY, 0.0);
        glRotatef(angle, 0.0, 0.0, 1.0);
        glScalef(length ,1.0, 1.0);

        glColor3fv(color);
        glLineWidth(widthInPixels);
        glBegin(GL_LINES);
            glVertex2f(0.0, 0.0);
            glVertex2f(1.0, 0.0);
        glEnd();

    }

    static void DrawLineStrip(float posX, float posY, float angle, float* verts, int numVerts, float thickness, float color[3])
    {
        GLint viewport[4]; // x, y, width, height
        glGetIntegerv(GL_VIEWPORT, viewport);

        // Convert the line thickness from meters to pixels
        float aspectRatio = (float)ASPECT_WIDTH/ASPECT_HEIGHT;
        int widthInPixels = viewport[3] * (1/aspectRatio) * thickness;

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        glTranslatef(posX, posY, 0.0);
        glRotatef(angle, 0.0, 0.0, 1.0);
        glScalef(1.0, 1.0, 1.0);

        glColor3fv(color);
        glLineWidth(widthInPixels);
        glBegin(GL_LINE_STRIP);
            for(int i = 0; i < numVerts; i++)
                glVertex2f(verts[0 + i * 2], verts[1 + i * 2]);
        glEnd();
    }

    static void DrawTexturedRectangle(float vertices[4][2], float uv[4][2], GLuint textureID, float posX, float posY, float scaleX, float scaleY, float angle,
                               float r, float g, float b, float alpha = 1.0)
    {
        //float color[3] = {0.0, 0.8, 0.0};
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        glTranslatef(posX, posY, 0.0);
        glRotatef(angle, 0.0, 0.0, 1.0);
        glScalef(scaleX, scaleY, 1.0);

        glColor4f(r, g, b, alpha);

        glEnable(GL_TEXTURE_2D);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        //glColor3f(0.5, 1.0, 1.0);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glBegin(GL_POLYGON);
            glTexCoord2fv(uv[0]);
            glVertex2fv(vertices[0]);
            glTexCoord2fv(uv[1]);
            glVertex2fv(vertices[1]);
            glTexCoord2fv(uv[2]);
            glVertex2fv(vertices[2]);
            glTexCoord2fv(uv[3]);
            glVertex2fv(vertices[3]);
        glEnd();

        glColor4f(1.0, 1.0, 1.0, 1.0);

        glDisable(GL_TEXTURE_2D);
        glDisable(GL_BLEND);
        //glBindTexture(GL_TEXTURE_2D, NULL);
    }

    static void DrawTexturedRectangle(GLuint textureID, float posX, float posY, float scaleX, float scaleY, float angle, float r, float g, float b, float alpha = 1.0)
    {
        float vertices[4][2] = {{-0.5, 0.5}, {0.5, 0.5}, {0.5, -0.5}, {-0.5, -0.5}};
        float uv[4][2] = {{0.0, 0.0}, {1.0, 0.0}, {1.0, 1.0}, {0.0, 1.0}};
        DrawTexturedRectangle(vertices, uv, textureID, posX, posY, scaleX, scaleY, angle, r, g, b, alpha);
    }

    static void DrawTexturedRectangle(GLuint textureID, float posX, float posY, float scaleX, float scaleY, float angle, float alpha = 1.0)
    {
        DrawTexturedRectangle(textureID, posX, posY, scaleX, scaleY, angle, 1.0, 1.0, 1.0, alpha);
    }

    static void DrawTexturedRectangle(float vertices[4][2], float uv[4][2], GLuint textureID, float posX, float posY, float angle, float r, float g, float b, float alpha = 1.0)
    {
        DrawTexturedRectangle(vertices, uv, textureID, posX, posY, 1.0, 1.0, angle, r, g, b, alpha);
    }

    static void DrawTexturedRectangle(float vertices[4][2], float uv[4][2], GLuint textureID, float posX, float posY, float angle, float alpha = 1.0)
    {
        DrawTexturedRectangle(vertices, uv, textureID, posX, posY, 1.0, 1.0, angle, 1.0, 1.0, 1.0, alpha);
    }


    static void DrawRectangle(float posX, float posY, float scaleX, float scaleY, float angle, float color[3])
    {
        GLfloat vertices[4][2] = {{-0.5, -0.5}, {-0.5, 0.5}, {0.5, 0.5}, {0.5, -0.5}};

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        glTranslatef(posX, posY, 0.0);
        glRotatef(angle, 0.0, 0.0, 1.0);
        glScalef(scaleX, scaleY, 1.0);

        glColor3f(color[0], color[1], color[2]);
        glBegin(GL_POLYGON);
            glVertex2fv(vertices[0]);
            glVertex2fv(vertices[1]);
            glVertex2fv(vertices[2]);
            glVertex2fv(vertices[3]);
        glEnd();
    }


    static void ResizeScreen(int w, int h)
    {
        float aspectRatio = (float)ASPECT_WIDTH / ASPECT_HEIGHT;
        float screenAR = (float)w / h;
        int viewportHeight = (1/aspectRatio) * w;
        int viewportWidth = aspectRatio * h;

        if(screenAR < aspectRatio)
            //Reduce height .. black horizontal bars
            glViewport(0, (h - viewportHeight) / 2.0, w, viewportHeight);
        else
            glViewport((w - viewportWidth) / 2.0, 0, viewportWidth, h);
    }

    static bool PixelsToWorld(int x, int y, float* xWorld, float* yWorld)
    {
        GLint viewport[4]; // x, y, width, height
        glGetIntegerv(GL_VIEWPORT, viewport);

        if(x < viewport[0] || x > viewport[0] + viewport[2]
           || y < viewport[1] || y > viewport[1] + viewport[3])
            return false; // not inside the viewport

        int midX = viewport[2] / 2.0 + viewport[0];
        int midY = viewport[3] / 2.0 + viewport[1];

        //x, y from the middle of the viewport
        x = -(midX - x);
        y = midY - y;

        *xWorld = ASPECT_WIDTH * x / (float)viewport[2];
        *yWorld = ASPECT_HEIGHT * y / (float)viewport[3];

        return true;
    }
};
