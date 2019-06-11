#pragma once
#include <memory>
#include "PMXModel.h"

namespace K3D {

	class PMXModel;

	class PMXLoader
	{
	private:
		void LoadInfo(FILE*fp, K3D::PMXModel* model);
		void LoadVertex(FILE*fp, K3D::PMXModel* model);
		void LoadSurface(FILE*fp, K3D::PMXModel* model);
		void LoadTexture(FILE*fp, K3D::PMXModel* model);
		void LoadMaterials(FILE*fp, K3D::PMXModel* model);
		void LoadBone(FILE*fp, K3D::PMXModel* model);
		void LoadMorph(FILE*fp, K3D::PMXModel* model);;
		void LoadDisplay(FILE*fp, K3D::PMXModel* model);
		void LoadRigidBody(FILE*fp, K3D::PMXModel* model);
		void LoadJoint(FILE*fp, K3D::PMXModel* model);
	public:;
		   std::shared_ptr<K3D::PMXModel>  Load(std::string modelPath);
		   PMXLoader();
		   ~PMXLoader();
	};
}