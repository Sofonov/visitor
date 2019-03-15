#include "Header.h"
#include<string>

using namespace std;
using namespace cadex;

int main(int argc, char *argv[])
{
	if (argc < 2 || argc > 3) {
		cerr << "Usage: " << argv[0] << " <input_file> [<mesher>], where:" << endl;
		cerr << "    <input_file> is a name of the STEP file to be read" << endl;
		cerr << "    <mesher> is 'm' for Mefisto, 'n' for Netgen computational" << endl;
		cerr << "        mesher. Default is n" << endl;
		return 1;
	}
	
	//activate the license (aKey must be defined in cadex_license.cxx)
	if (!CADExLicense_Activate(aKey)) {
		cerr << "Failed to activate CAD Exchanger license." << endl;
		return 1;
	}

	ModelAlgo_BRepMesherParameters aParam;
	if (argc > 2 && argv[2][0] == 'm') { //Mefisto
		MeshAlgo_MefistoFactory aMF;
		MeshAlgo_MefistoFactory::Parameters aP(
			-1. /*absolute global max size*/,
			-1. /*relative global max size*/,
			1. / 10. /*relative local max size*/);
		aMF.SetParameters(aP);
		aParam.ComputationalMeshAlgo() = aMF;
	}
	else { //Netgen
		MeshAlgo_NetgenFactory aMF;
		MeshAlgo_NetgenFactory::Parameters aP;
		aP.SetGranularity(MeshAlgo_NetgenFactory::Parameters::Fine);
		aMF.SetParameters(aP);
		aParam.ComputationalMeshAlgo() = aMF;
	}
	
	
	
	 ModelData_Model aModel;
	const char* aSource = argv[1];
	
	if (!Import(aSource, aModel)) {
		cerr << "Failed to read the file " << aSource << endl;
		system("pause");
		return 1;
	}
	
	MyVisitor Visitor;
	ModelData_SceneGraphElementUniqueVisitor aVisitor(Visitor);
	
	foo(aModel);
    
	aModel.Accept(aVisitor);
	
	int num = 0;
	
	for (auto it = Visitor.Partlist.begin(); it != Visitor.Partlist.end(); it++)
	{
		const ModelData_BRepRepresentation& aBRep = (it)->BRepRepresentation();
		ModelData_PolyRepresentation a_Poly(aBRep, aParam);

		ModelData_Model aModel;
		aModel.AddRoot(*it);
		
		num++;

		string name ="Part_"+std::to_string(num) + ".stp";
				
		if (!Export(&name[0], aModel)) {
			cerr << "Failed to read the file " << aSource << endl;
			system("pause");
			return 1;
		}
	}
	
	system("pause");
	return 0;
}