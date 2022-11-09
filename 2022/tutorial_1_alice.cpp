#define _MAIN_

#ifdef _MAIN_

#include "main.h"

#include <headers/zApp/include/zObjects.h>
#include <headers/zApp/include/zFnSets.h>
#include <headers/zApp/include/zViewer.h>


using namespace zSpace;
using namespace std;

////////////////////////////////////////////////////////////////////////// General

bool compute = false;
bool computeMap = false;
bool exportToCSV = false;
bool display = true;

double background = 1.0;
double voxelSize = 0.35;
int numVoxels = -1;
////// --- zSpace Objects --------------------------------------------------
/*!<model*/
zModel model;

/*!<Objects*/

zUtilsCore core;

zObjMesh oMesh;
zObjGraph oGraph;

zObjPointCloud oCloud;
zPoint minBB, maxBB;


////// --- GUI OBJECTS ----------------------------------------------------

void createPointCloud(zFnPointCloud& fnCloud, double unit_X, double unit_Y, double unit_Z, int n_X, int n_Y, int n_Z, zPoint& _minBB, zPoint& _maxBB, zPoint startPt = zPoint(0, 0, 0))
{
	vector<zVector>positions;

	zVector unitVec = zVector(unit_X, unit_Y, unit_Z);

	for (int i = 0; i < n_X; i++)
	{
		for (int j = 0; j < n_Y; j++)
		{
			for (int k = 0; k < n_Z; k++)
			{

				zVector pos;
				pos.x = startPt.x + i * unitVec.x;
				pos.y = startPt.y + j * unitVec.y;
				pos.z = startPt.z + k * unitVec.z;

				positions.push_back(pos);
			}
		}
	}
	fnCloud.create(positions);
	fnCloud.getBounds(_minBB, _maxBB);
}


void setup()
{
	////////////////////////////////////////////////////////////////////////// Enable smooth display
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_POINT_SMOOTH);

	////////////////////////////////////////////////////////////////////////// ZSPACE
	// initialise model
	model = zModel(100000);
	model.addObject(oCloud);
	oCloud.setDisplayElements(false);




		
	//////////////////////////////////////////////////////////  DISPLAY SETUP
	// append to model for displaying the object
	//model.addObject(oMesh);

	// set display element booleans
	//oMesh.setDisplayElements(true, true, true);

	////////////////////////////////////////////////////////////////////////// Sliders

	S = *new SliderGroup();
	
	S.addSlider(&background, "background");
	S.sliders[0].attachToVariable(&background, 0, 1);
	S.addSlider(&voxelSize, "size");
	S.sliders[1].attachToVariable(&voxelSize, 0, 1);

	////////////////////////////////////////////////////////////////////////// Buttons

	B = *new ButtonGroup(Alice::vec(50, 450, 0));

	B.addButton(&compute, "compute");
	B.buttons[0].attachToVariable(&compute);
	B.addButton(&display, "display");
	B.buttons[1].attachToVariable(&display);
	B.addButton(&exportToCSV, "exportToCSV");
	B.buttons[2].attachToVariable(&exportToCSV);

}

void update(int value)
{
	if (compute)
	{
		zFnPointCloud fnCloud(oCloud);
		float unitX, unitY, unitZ;
		int numX, numY, numZ;

		unitX = unitY = unitZ = 1;
		numX = numY = numZ = 20;

		createPointCloud(fnCloud, unitX, unitY, unitZ, numX, numY, numZ, minBB, maxBB);
		cout << maxBB << "," << minBB << endl;

		numVoxels = fnCloud.numVertices();
		zDomainColor colDomain(zColor(0, 0, 0, 0), zColor(1, 0, 0, 1));

		float maxDist = (maxBB - minBB).length();
		zPoint target = minBB;
		for (zItPointCloudVertex v(oCloud); !v.end(); v++)
		{
			zVector vec = v.getPosition() - target;
			float dist = vec.length();
			zColor col = core.blendColor(dist, zDomainFloat(0,maxDist), colDomain, zRGB);
			v.setColor(col);
		}

		compute = !compute;	
	}

	if (exportToCSV)
	{
		ofstream file;
		file.open("data/points.csv");

		float maxDist = (maxBB - minBB).length();
		for (zItPointCloudVertex v(oCloud); !v.end(); v++)
		{
			file << v.getId() << "," << v.getPosition().x << "," << v.getPosition().y << "," << v.getPosition().z << ",";
			file << v.getColor().r << "," << v.getColor().g << "," << v.getColor().b << "\n";
		}
		file.close();

		exportToCSV = !exportToCSV;
	}
}

void draw()
{
	backGround(background);
	//drawGrid(50);

	S.draw();
	B.draw();

	if (display)
	{
		// zspace model draw
		model.draw();


		for (zItPointCloudVertex v(oCloud); !v.end(); v++)
		{
			model.displayUtils.drawPoint(v.getPosition(), v.getColor(), 10* voxelSize);
		}
		
	}

	


	//////////////////////////////////////////////////////////

	setup2d();
	glColor3f(0, 0, 0);

	drawString("AADRL WS2 2022", vec(50, 200, 0));

	drawString("Total num of voxels #:" + to_string(numVoxels), vec(50, 250, 0));
	drawString("p - compute voxels", vec(50, 275, 0));
	drawString("o - export to csv", vec(50, 300, 0));

	restore3d();
}

void keyPress(unsigned char k, int xm, int ym)
{
	if (k == 'p') compute = true;;

	if (k == 'w') compute = true;;

	if (k == 'o') exportToCSV = true;

}

void mousePress(int b, int s, int x, int y)
{
	
}

void mouseMotion(int x, int y)
{	

}



#endif // _MAIN_
