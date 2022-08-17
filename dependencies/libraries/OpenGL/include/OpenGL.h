#ifndef OPENGL_H
#define OPENGL_H


#ifdef __WIN32
#include <GL/gl.h>
#include <gl/wglext.h>
#elif defined(__APPLE__)
// Probably shouldn't do this
#define GL_SILENCE_DEPRECATION

#include <gl3.h>
#include <gl3ext.h>

#else
#include <GL/gl.h>
#include <GL/glext.h>
#endif

#endif /* OPENGL_H */
