/*
 * garbage_thick_edge.cxx
 * Daniel Nelson - 9/1/0
 *
 * Copyright (C) 2000  Daniel Nelson
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * Daniel Nelson - aluminumangel.org
 * 174 W. 18th Ave.
 * Columbus, OH  43210
 *
 * Generates a display list for the edge of thick garbage.
 */

#include <GL/glut.h>

#include "glext.h"

using namespace std;

#include "Game.h"
#include "Displayer.h"

GLuint Displayer::garbage_thick_edge_list;

void Displayer::generateGarbageThickEdgeList (   )
{
  garbage_thick_edge_list = glGenLists(1);

  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_NORMAL_ARRAY);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);

  glNewList(garbage_thick_edge_list, GL_COMPILE);

    GLfloat vertices_0[3 * 12] = {
      1.0f, -1.0f, -1.0,
      -1.0f, -1.0f, -1.0,
      1.0f, 0.8, -1.0,
      -1.0f, 0.8, -1.0,
      1.0f, 1.0, -0.8,
      -1.0f, 1.0, -0.8,
      1.0f, 1.0, 0.8,
      -1.0f, 1.0, 0.8,
      1.0f, 0.8, 1.0,
      -1.0f, 0.8, 1.0,
      1.0f, -1.0f, 1.0,
      -1.0f, -1.0f, 1.0
    };

    GLfloat normals_0[3 * 12] = {
      0.0f, 0.0f, -1.0f,
      0.0f, 0.0f, -1.0f,
      0.0f, 0.0f, -1.0f,
      0.0f, 0.0f, -1.0f,
      0.0f, 1.0f, 0.0f,
      0.0f, 1.0f, 0.0f,
      0.0f, 1.0f, 0.0f,
      0.0f, 1.0f, 0.0f,
      0.0f, 0.0f, 1.0f,
      0.0f, 0.0f, 1.0f,
      0.0f, 0.0f, 1.0f,
      0.0f, 0.0f, 1.0f
    };


    GLfloat tex_coords_0[3 * 12];
    for (int n = 3 * 12; n--; )
      tex_coords_0[n] = vertices_0[n] * DC_GARBAGE_LIGHTMAP_COORD_CONVERTER;

    glVertexPointer(3, GL_FLOAT, 0, vertices_0);
    glNormalPointer(GL_FLOAT, 0, normals_0);
    glTexCoordPointer(3, GL_FLOAT, 0, tex_coords_0);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 12);

  glEndList();

  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_NORMAL_ARRAY);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}
