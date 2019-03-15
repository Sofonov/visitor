#pragma once
#include <cadex/Base_ProgressScope.hxx>
#include <cadex/Base_ProgressStatus.hxx>
#include <cadex/LicenseManager_Activate.h>
#include <cadex/MeshAlgo_MefistoFactory.hxx>
#include <cadex/MeshAlgo_NetgenFactory.hxx>
#include <cadex/ModelData_Body.hxx>
#include <cadex/ModelData_BodyList.hxx>
#include <cadex/ModelAlgo_BRepMesher.hxx>
#include <cadex/ModelAlgo_BRepMesherParameters.hxx>
#include <cadex/ModelData_BRepRepresentation.hxx>
#include <cadex/ModelData_IndexedTriangleSet.hxx>
#include <cadex/ModelData_Model.hxx>
#include <cadex/ModelData_Part.hxx>
#include <cadex/ModelData_Assembly.hxx>
#include <cadex/ModelData_Instance.hxx>
#include <cadex/ModelData_PolyRepresentation.hxx>
#include <cadex/ModelData_PolyShapeList.hxx>
#include <cadex/ModelData_SceneGraphElementUniqueVisitor.hxx>
#include <cadex/ModelData_Transformation.hxx>
#include <cadex/ModelData_PropertyTable.hxx>
#include <cadex/ModelData_Vector.hxx>
#include <cadex/STEP_Reader.hxx>
#include <cadex/STEP_Writer.hxx>
#include <iostream>
#include<set>
#include <string>
#include "../cadex_license.cxx"
using namespace std;
using namespace cadex;



static bool Import(const char* theSource, ModelData_Model& theModel)
{
	STEP_Reader aReader;
	return aReader.ReadFile(theSource) && aReader.Transfer(theModel);
}



static bool Export(const char* theDest, const ModelData_Model& theModel)
{
	STEP_Writer aWriter;
	if (!aWriter.Transfer(theModel) || !aWriter.WriteFile(theDest)) {
		cerr << "Failed to write the file " << theDest << endl;
		return false;
	}
	return true;
}

template <typename T>
void foo(T & aModel, int deep = 1)
{
	ModelData_Model::ElementIterator anIterator(aModel);
	while (anIterator.HasNext())
	{

		ModelData_SceneGraphElement& aSGE = anIterator.Next();
		if (aSGE.TypeId() != ModelData_Part::GetTypeId())
		{
			for (int i = 0; i < deep; i++)
				cout << "-";
			wcout << aSGE.Name().ToWString();

			if (aSGE.TypeId() == ModelData_Assembly::GetTypeId())
				cout << "(Assembly)";
			else if (aSGE.TypeId() == ModelData_Instance::GetTypeId())
				cout << "(Instance)";
			else
				cout << "(Unknown)";
			cout << " " << &aSGE << " ";
			for (ModelData_Model::ElementIterator anIterator2(aSGE); anIterator2.HasNext(); anIterator2.Next())
				cout << '*';
			cout << endl;
			foo(aSGE, deep + 1);
		}
		else
		{
			for (int i = 0; i < deep; i++)
				cout << "-";
			wcout << aSGE.Name().ToWString() << "(Part)" << endl;

			return;
		}

	}

}



class MyVisitor : public  ModelData_Model::VoidElementVisitor
{
public:
	
	std::list<ModelData_Part> Partlist;
	
	virtual void operator()(const ModelData_Part&  Part) override 
	{
		Partlist.push_back (Part);
	}

	

};


	







