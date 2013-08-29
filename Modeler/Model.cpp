/*
 *  Model.cpp
 *
 *  Created by Nelson El-Hage on Wed Nov 27 2002.
 *
 */

#include "Model.h"
#include "GLUI2D.h"
#include "camera.h"
#include "App_Log.h"
#include <fstream>
#include <wx/wx.h>
#include "wxModeler.h"

using std::ofstream;
using std::ifstream;
using std::endl;

const GLfloat LightAmbient[] = {0.5,0.5,0.5,1.0};
const GLfloat LightDiffuse[] = {.75,.75,.75,1.0};
const GLfloat LightPos[]	 = {0.0,0.0,1.0,0.0};
const GLfloat CubeColor[] 	 = {0.5,0.0,0.0,1.0};
const GLfloat SelCubeColor[] = {0.0,.75,.75,1.0};
const GLfloat SelFaceColor[] = {0.0,.75,0.0,1.0};

Model::Model()			//construct a single cube with edge length 2 at the origin
{
}

void Model::Reset()
{
	//wxMessageBox("init()");
	int i;

	Points.erase(Points.begin(),Points.end());
		
	Points.push_back(Vector3D<>(-0.5,-0.5,0.5));
	Points.push_back(Vector3D<>(0.5,-0.5,0.5));
	Points.push_back(Vector3D<>(0.5,0.5,0.5));
	Points.push_back(Vector3D<>(-0.5,0.5,0.5));

	Points.push_back(Vector3D<>(-0.5,-0.5,-0.5));
	Points.push_back(Vector3D<>(0.5,-0.5,-0.5));
	Points.push_back(Vector3D<>(0.5,0.5,-0.5));
	Points.push_back(Vector3D<>(-0.5,0.5,-0.5));

	Cube c;

	for(i=0;i<8;i++)
		c.Verts[i] = i;

	Cubes.erase(Cubes.begin(),Cubes.end());
	
	Cubes.push_back(c);

	SelectedCube = 0;
	SelectedFace = 0;
	SelectedPoint = 0;
	Mode = SEL_CUBE;
	DMode = DRW_LINE;
	DrgMode = DRG_NONE;
	HandleLength = .5;

	glLightfv(GL_LIGHT0,GL_AMBIENT,LightAmbient);
	glLightfv(GL_LIGHT0,GL_DIFFUSE,LightDiffuse);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	
}

Model::Cube::Cube()
{
	int i;
	for(i=0;i<8;i++) Verts[i] = 0;
	for(i=0;i<6;i++) Adjacent[i] = -1;
}


//This draws all the faces of the cubes as quads and loads the appropriate names
void Model::DrawPicking()
{
	glInitNames();
	glPushName(0);

	unsigned int i,j;
	
	if(Mode == SEL_CUBE || Mode == SEL_FACE)
	{
		for(i=0;i<Cubes.size();i++)
		{
			if(Mode == SEL_CUBE)
				glLoadName(i+1);
			else
				glLoadName(0);
			const Cube & c = Cubes[i];
			for(j=0;j<6;j++)
			{
				if(Mode == SEL_FACE && i == SelectedCube)
					glLoadName(j+1);
				glBegin(GL_QUADS);
				Points[c.Verts[Faces[j][0]]].SendOGLVertex();
				Points[c.Verts[Faces[j][1]]].SendOGLVertex();
				Points[c.Verts[Faces[j][2]]].SendOGLVertex();
				Points[c.Verts[Faces[j][3]]].SendOGLVertex();
				glEnd();
			}
		}
	}
	else if(Mode == SEL_POINT)
	{
		const Cube & c = Cubes[SelectedCube];
		for(i=0;i<4;i++)
		{
			glLoadName(i+1);
			const Vector3D<> & v = Points[c.Verts[Faces[SelectedFace][i]]];
			glRasterPos3dv(v.V);
		}
		//Log.Write("Drawing move handles.");
		glLoadName(DRG_X);
		glRasterPos3dv((Points[SelectedPoint]+Vector3D<>(HandleLength,0.0,0.0)).V);
		glLoadName(DRG_Y);
		glRasterPos3dv((Points[SelectedPoint]+Vector3D<>(0.0,HandleLength,0.0)).V);
		glLoadName(DRG_Z);
		glRasterPos3dv((Points[SelectedPoint]+Vector3D<>(0.0,0.0,HandleLength)).V);
	}
}

void Model::Draw()
{
	//wxMessageBox("Draw");
	unsigned int i,j;
	//glColor3f(0.5,0.0,0.0);
	glMaterialfv(GL_FRONT,GL_AMBIENT,CubeColor);
	for(i=0;i<Cubes.size();i++)
	{
		if(i == SelectedCube) continue;
		const Cube & c = Cubes[i];
		for(j=0;j<6;j++)
		{
			glBegin((DMode==DRW_LINE)?GL_LINE_LOOP:GL_QUADS);
			//calculate the vector perpendicular to the selected face
			Vector3D <> v0 = Points[c.Verts[Faces[j][0]]];
			Vector3D <> v1 = Points[c.Verts[Faces[j][1]]];
			Vector3D <> v2 = Points[c.Verts[Faces[j][2]]];
			Vector3D <> norm = (v2-v1).cross(v0-v1).Norm();
			norm.SendOGLNormal();
			
			Points[c.Verts[Faces[j][0]]].SendOGLVertex();
			Points[c.Verts[Faces[j][1]]].SendOGLVertex();
			Points[c.Verts[Faces[j][2]]].SendOGLVertex();
			Points[c.Verts[Faces[j][3]]].SendOGLVertex();
			glEnd();
		}
	}
	//wxMessageBox("Drew Cubes");

	const Cube & c = Cubes[SelectedCube];
	
	for(i=0;i<6;i++)
	{
		if(i != SelectedFace)
		{
			if(DMode == DRW_LINE)
				glBegin(GL_LINE_LOOP);
			else
				glBegin(GL_QUADS);
			//glColor3f(1.0,1.0,1.0);
			glMaterialfv(GL_FRONT,GL_AMBIENT,SelCubeColor);
		}
		else
		{
			glBegin(GL_QUADS);
			//glColor3f(0.0,0.25,0.0);
			glMaterialfv(GL_FRONT,GL_AMBIENT,SelFaceColor);
		}

		Vector3D <> v0 = Points[c.Verts[Faces[i][0]]];
		Vector3D <> v1 = Points[c.Verts[Faces[i][1]]];
		Vector3D <> v2 = Points[c.Verts[Faces[i][2]]];
		Vector3D <> norm = (v2-v1).cross(v0-v1).Norm();
		norm.SendOGLNormal();
		
		Points[c.Verts[Faces[i][0]]].SendOGLVertex();
		Points[c.Verts[Faces[i][1]]].SendOGLVertex();
		Points[c.Verts[Faces[i][2]]].SendOGLVertex();
		Points[c.Verts[Faces[i][3]]].SendOGLVertex();
		glEnd();
	}

	glDisable(GL_LIGHTING);
	glPointSize(5.0);
	glBegin(GL_POINTS);
		glColor3f(1.0,0.0,1.0);
		Points[SelectedPoint].SendOGLVertex();
	glEnd();
	if(Mode==SEL_POINT)
	{
		glBegin(GL_LINES);
			glColor3f(1.0,0.0,0.0);
			Points[SelectedPoint].SendOGLVertex();
			(Points[SelectedPoint]+Vector3D<>(HandleLength,0.0,0.0)).SendOGLVertex();
			glColor3f(0.0,1.0,0.0);
			Points[SelectedPoint].SendOGLVertex();
			(Points[SelectedPoint]+Vector3D<>(0.0,HandleLength,0.0)).SendOGLVertex();
			glColor3f(0.0,0.0,1.0);
			Points[SelectedPoint].SendOGLVertex();
			(Points[SelectedPoint]+Vector3D<>(0.0,0.0,HandleLength)).SendOGLVertex();
		glEnd();
		glPointSize(10.0);
		glBegin(GL_POINTS);
			glColor3f(1.0,0.0,0.0);
			(Points[SelectedPoint]+Vector3D<>(HandleLength,0.0,0.0)).SendOGLVertex();
			glColor3f(0.0,1.0,0.0);
			(Points[SelectedPoint]+Vector3D<>(0.0,HandleLength,0.0)).SendOGLVertex();
			glColor3f(0.0,0.0,1.0);
			(Points[SelectedPoint]+Vector3D<>(0.0,0.0,HandleLength)).SendOGLVertex();
		glEnd();
	}

	GLUI2D::Begin2D();
		glBindTexture(GL_TEXTURE_2D,0);
		glColor3f(.5,0.0,0.0);
   		GLUI2D::Rect(0,SCREEN_HEIGHT-20,20,20);
   		glColor3f(0.0,0.0,1.0);
   		GLUI2D::Text(0,SCREEN_HEIGHT-18,"C\0F\0P\0M\0"+2*Mode);
	GLUI2D::End2D();
	glEnable(GL_LIGHTING);
	//wxMessageBox("/Draw");
}

void Model::SelNext()
{
	int i;
	switch(Mode)
	{
		case SEL_CUBE:
   			SelectedCube++;
   			if(SelectedCube == Cubes.size()) SelectedCube = 0;
   			SelectedPoint = Cubes[SelectedCube].Verts[Faces[SelectedFace][0]];
    		break;
  		case SEL_FACE:
  			SelectedFace = (SelectedFace+1) % 6;
     		SelectedPoint = Cubes[SelectedCube].Verts[Faces[SelectedFace][0]];
  			break;
 		case SEL_POINT:
			for(i=0;i<4;i++)
				if(Cubes[SelectedCube].Verts[Faces[SelectedFace][i]] ==
    					SelectedPoint)
     				break;
			i = (i+1)%4;
   			SelectedPoint = Cubes[SelectedCube].Verts[Faces[SelectedFace][i]];
   			break;
	}
}

void Model::SelPrev()
{
	int i;
	switch(Mode)
	{
		case SEL_CUBE:
   			SelectedCube = (SelectedCube==0)?Cubes.size()-1:SelectedCube-1;
   			SelectedPoint = Cubes[SelectedCube].Verts[Faces[SelectedFace][0]];
    		break;
  		case SEL_FACE:
  			SelectedFace = (SelectedFace==0)?5:SelectedFace-1;
     		SelectedPoint = Cubes[SelectedCube].Verts[Faces[SelectedFace][0]];
  			break;
 		case SEL_POINT:
			for(i=0;i<4;i++)
				if(Cubes[SelectedCube].Verts[Faces[SelectedFace][i]] ==
    					SelectedPoint)
     				break;
			i = i-1;
			if(i == -1)i=3;
   			SelectedPoint = Cubes[SelectedCube].Verts[Faces[SelectedFace][i]];
   			break;
	}
}

void Model::AddCube()
{
	//if there's already a cube, return
	if(Cubes[SelectedCube].Adjacent[SelectedFace] != -1) return;
	
	Cube nc;			//the new cube we will be adding
	Cube & c = Cubes[SelectedCube];
	int i;

	for(i=0;i<4;i++)
		nc.Verts[i^1] = c.Verts[Faces[SelectedFace][i]];
	//the ^1 flips the least significant bit, and causes the
	//vertices in the new cube to be aligned properly
	
	//calculate the vector perpendicular to the selected face
	Vector3D <> v0 = Points[c.Verts[Faces[SelectedFace][0]]];
	Vector3D <> v1 = Points[c.Verts[Faces[SelectedFace][1]]];
	Vector3D <> v2 = Points[c.Verts[Faces[SelectedFace][2]]];
	//The right-hand rule, plus the windings of our vertices,
	//ensure that this cross product points in the right dir
	Vector3D <> perp = (v2-v1).cross(v0-v1);
	perp.Normalize();
	//Log.Write("Perp:<%f,%f,%f>",perp.V[0],perp.V[1],perp.V[2]);
	
	for(i=0;i<4;i++)
	{
		Points.push_back(Points[c.Verts[Faces[SelectedFace][i]]] + perp);
		nc.Verts[(i^1)+4] = Points.size() - 1;
	}
	
	nc.Adjacent[0] = SelectedCube;
	
	Cubes.push_back(nc);

	c.Adjacent[SelectedFace] = Cubes.size() - 1;
	SelectedCube = Cubes.size() - 1;
	SelectedPoint = nc.Verts[Faces[SelectedFace][0]];
}

void Model::Click(int x, int y)
{
	GLuint SelBuff[512],dist;
	GLint viewport[4];
	GLint hit;
	GLint sel = 0;

	glGetIntegerv(GL_VIEWPORT,viewport);

	glSelectBuffer(512,SelBuff);
	glRenderMode(GL_SELECT);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	if(Mode == SEL_FACE || Mode == SEL_CUBE)
		gluPickMatrix((double)x,(double)(viewport[3] - y),1.0,1.0,viewport);
	else
		gluPickMatrix((double)x,(double)(viewport[3] - y),20.0,20.0,viewport);
	gluPerspective(45.0f, (GLfloat) (viewport[2]-viewport[0])/(GLfloat) (viewport[3]-viewport[1]), 0.1f, 100.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	view.View();
	DrawPicking();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);

	hit = glRenderMode(GL_RENDER);
	if(hit != 0)
	{
		sel = SelBuff[3];
		dist = SelBuff[1];
		//Log.Write("%d:%u",sel,dist);
		for(int i=0;i<hit;i++)
			if((SelBuff[4*i+1] < dist && (SelBuff[4*i+3] != 0) )|| sel == 0)
			{
				sel = SelBuff[4*i+3];
				dist = SelBuff[4*i+1];
				//Log.Write("%d:%u",sel,dist);
			}
	}
	
	int point = 0;
	if(sel != 0)
	{
		Log.Write("Selected: %d",sel);
		if(Mode == SEL_POINT && sel >= DRG_X)
		{
			DrgMode = (DragMode)sel;
			//Log.Write("Drag mode is now: %s","X\0Y\0Z\0"+2*(DrgMode-1));
		}
		else
		{
			if(Mode == SEL_CUBE)
			{
				SelectedCube = sel-1;
			}
			else if(Mode == SEL_FACE)
			{
				SelectedFace = sel-1;
			}
			else if(Mode == SEL_POINT)
			{
				point = sel-1;
			}
			SelectedPoint = Cubes[SelectedCube].Verts[Faces[SelectedFace][point]];
		}
	}
	//Log.Write("--");
}

void Model::MouseUp(int x, int y)
{
	DrgMode = DRG_NONE;
}

void Model::MouseMove(int xrel, int yrel)
{
	if(DrgMode != DRG_NONE)
	{
		double dist = MAXABS(xrel,-yrel) * 8./640.;
		//Log.Write("Dragging %f",dist);
		Points[SelectedPoint] += Vector3D<>((DrgMode == DRG_X)*dist,
											(DrgMode == DRG_Y)*dist,
											(DrgMode == DRG_Z)*dist);
	}
}

void Model::Save(const char * file)
{
	ofstream fout(file);
	int i;
	fout << "#POINTS " << Points.size() << endl;
	std::vector<Vector3D<> >::const_iterator it = Points.begin();
	for(;it!=Points.end();it++)
	{
		fout << it->V[0] << " " << it->V[1]  << " " << it->V[2] << endl;
	}

	fout << "#CUBES " << Cubes.size() << endl;
	
	std::vector<Cube>::const_iterator j = Cubes.begin();
	for(;j!=Cubes.end();j++)
	{
		for(i=0;i<8;i++)
			fout << j->Verts[i] << " ";
		for(i=0;i<6;i++)
			fout << j->Adjacent[i] << " ";
		fout << "\n";
	}
	fout.close();
}

void Model::Load(const char * file)
{
	Reset();
	
	Points.erase(Points.begin(),Points.end());
	Cubes.erase(Cubes.begin(),Cubes.end());

	ifstream fin(file);
	char buff[256];
	int N,i,j;
	float x,y,z;
	Cube c;
		
	if(!fin)
	{
		wxVLogError("Unable to open file %s",file);
		return;
	}

	fin.get(buff,8);
	if(strcmp(buff,"#POINTS"))
	{
		wxLogError("Invalid file format - expected \"#POINTS\"");
		return;
	}
	
	fin >> N;
	//wxString a;
	//a.Printf("%d verts",N);
	//wxMessageBox(a);
	for(i=0;i<N;i++)
	{
		fin >> x >> y >> z;
		Points.push_back(Vector3D<>(x,y,z));
	}

	fin.getline(buff,255);
	fin.get(buff,7);

	if(strcmp(buff,"#CUBES"))
	{
		wxLogError("Invalid file format - expected \"#CUBES\" - got \"%s\"",buff);
		return;
	}

	fin >> N;
	//a.Printf("%d cubes",N);
	//wxMessageBox(a);
	for(i=0;i<N;i++)
	{
		for(j=0;j<8;j++)
			fin >> c.Verts[j];
		for(j=0;j<6;j++)
			fin >> c.Adjacent[j];
		Cubes.push_back(c);
	}

	fin.close();
}