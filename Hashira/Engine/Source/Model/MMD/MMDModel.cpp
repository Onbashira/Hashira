#include "MMDModel.h"

K3D::MMDModel::MMDModel()
{
}


K3D::MMDModel::~MMDModel()
{
	this->GetMeshHeap().GetMaterialBufffer().Discard();
}


Matrix K3D::MMDModel::CulcBDEF1(WeightDeform & deform)
{
	return Matrix(_resourceData.lock()->_boneTree->bonesMatrix[deform.bdef1.boneIndex]);
}

Matrix K3D::MMDModel::CulcBDEF2(WeightDeform & deform)
{

	return Matrix(_resourceData.lock()->_boneTree->bonesMatrix[deform.bdef2.boneIndex01] * deform.bdef2.boneWeight01 +
		_resourceData.lock()->_boneTree->bonesMatrix[deform.bdef2.boneIndex02] * deform.bdef2.boneWeight02);
}

Matrix K3D::MMDModel::CulcBDEF4(WeightDeform & deform)
{
	return Matrix(_resourceData.lock()->_boneTree->bonesMatrix[deform.bdef4.boneIndex01] * deform.bdef4.boneWeight01 +
		_resourceData.lock()->_boneTree->bonesMatrix[deform.bdef4.boneIndex02] * deform.bdef4.boneWeight02 +
		_resourceData.lock()->_boneTree->bonesMatrix[deform.bdef4.boneIndex03] * deform.bdef4.boneWeight03 +
		_resourceData.lock()->_boneTree->bonesMatrix[deform.bdef4.boneIndex04] * deform.bdef4.boneWeight04);
}

std::pair<Vector3, float> K3D::MMDModel::CulcSDEF(WeightDeform & deform)
{
	float w1 = 0.0;
	float w2 = 0.0f;
	auto& skeltonRef = _resourceData.lock()->_boneTree;
	auto a = CulcSDEFWaight(deform.sdef.r0 + deform.sdef.c, deform.sdef.r1 + deform.sdef.c);

	//ウェイト値の代入
	w1 = a.first;
	w2 = a.second;
	//R0値R1値
	Vector3 r0 = Vector3(deform.sdef.r0 + deform.sdef.c);
	Vector3 r1 = Vector3(deform.sdef.r1 + deform.sdef.c);

	auto mat01 = skeltonRef->bonesMatrix[deform.sdef.boneIndex01] * deform.sdef.boneWeight01;
	auto mat02 = skeltonRef->bonesMatrix[deform.sdef.boneIndex02] * deform.sdef.boneWeight02;

	Matrix rotMat = mat01 * w1 + mat02 * w2;
	
	//C値の算出
	auto prC = Vector3::Transform(deform.sdef.c, rotMat);

	//加算処理
	{
		Matrix mat = mat01 + mat02;
		Vector3 v0 = Vector3::Transform(r0, mat02 + mat * deform.sdef.boneWeight01) - Vector3::Transform(r0, mat);
		Vector3 v1 = Vector3::Transform(r1, mat01 + mat * deform.sdef.boneWeight01) - Vector3::Transform(r1, mat);

		prC += v0 * w1 + v1 * w2;
	}
	
	std::pair<Vector3, float> ret;
	ret.first = prC;
	ret.second = w2;

	return ret;
}

Matrix K3D::MMDModel::CulcQDEF(WeightDeform & deform)
{

	return Matrix();
}

std::pair<float, float> K3D::MMDModel::CulcSDEFWaight(Vector3 r0, Vector3 r1)
{
	float l0 = r0.Length();
	float l1 = r1.Length();
	auto ret = std::pair<float, float>(0.0f, 0.0f);
	if (abs(l0 - l1) < 0.00001f) {
		ret.first = 0.5f;
	}
	else {
		ret.first = ::Saturate(l0 / (l0 + l1));
	}
	ret.second = 1.0f - ret.first;
	return ret;
}

void K3D::MMDModel::TransformUpdate()
{
	//移動量回転量スケール値のコミット
}

void K3D::MMDModel::AnimationUpdate()
{

	//GPUにコードを移植すること！

	////ボーン情報から頂点ブレンディング
	//for (unsigned int i = 0; i < _vertexes.size(); ++i) {
	//	Matrix rotMat;
	//	auto vertex = this->_vertexes[i];
	//	auto& deform = _resourceData.lock()->_vertexes[i].deformation;
	//	switch (_resourceData.lock()->_vertexes[i].deformType)
	//	{
	//	case MMDWeightDeformType::BDEF1:
	//		deform.bdef1;
	//		rotMat = CulcBDEF1(deform);
	//		vertex.pos = Vector3::Transform(vertex.pos, rotMat);
	//		vertex.normal = Vector3::TransformNormal(vertex.normal, rotMat);
	//		break;
	//	case MMDWeightDeformType::BDEF2:
	//		deform.bdef2;
	//		rotMat = CulcBDEF2(deform);
	//		vertex.pos = Vector3::Transform(vertex.pos, rotMat);
	//		vertex.normal = Vector3::TransformNormal(vertex.normal, rotMat);
	//		break;
	//	case MMDWeightDeformType::BDEF4:
	//		deform.bdef4;

	//		rotMat = rotMat = CulcBDEF4(deform);
	//		vertex.pos = Vector3::Transform(vertex.pos, rotMat);
	//		vertex.normal = Vector3::TransformNormal(vertex.normal, rotMat);
	//		break;
	//	case MMDWeightDeformType::SDEF:
	//		deform.sdef;
	//		{
	//			auto& skeltonRef = _resourceData.lock()->_boneTree;
	//			auto mcPair = CulcSDEF(deform);
	//			Vector4 q0 = skeltonRef->boneAccessor[skeltonRef->boneNameAccessor[deform.sdef.boneIndex01]]->rotation;
	//			Vector4 q1 = skeltonRef->boneAccessor[skeltonRef->boneNameAccessor[deform.sdef.boneIndex02]]->rotation;
	//			Quaternion q = Quaternion::Slerp(q0,q1, mcPair.second);
	//			
	//			vertex.pos = mcPair.first + Vector3::Rotate(vertex.pos - deform.sdef.c, q);
	//			vertex.normal = Vector3::Rotate(vertex.normal, q).Normalize();
	//		
	//		}
	//		break;
	//	case MMDWeightDeformType::QDEF:
	//		deform.qdef;

	//		break;
	//	default:
	//		break;
	//	}
	//}
}

void K3D::MMDModel::SkeltonReset()
{
	_animator.ResetSkelton();
}

void K3D::MMDModel::SetAnimationData(std::string motionPath)
{

}

void K3D::MMDModel::RegisterToBundle()
{

	_bundleList.GetCommandList()->IASetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	BindingShaderObjectToBundle();
	BindVertexBufferToBundle();
	BindingShaderObjectToBundle();

	this->GetMeshHeap().BindingDescriptorHeaps(_bundleList);

	_bundleList.GetCommandList()->SetGraphicsRootDescriptorTable(SHADER_ROOT_PARAMATER_INDEX_TRANSFORM, GetMeshHeap().GetHeap().GetGPUHandle(this->GetMeshHeap().GetTransformDescStartIndex()));
	unsigned int surfaceOffset = 0;
	for (UINT index = 0; index < _resourceData.lock()->_materials.size(); index++) {

		_bundleList.GetCommandList()->SetGraphicsRootDescriptorTable(SHADER_ROOT_PARAMATER_INDEX_MATERIAL, GetMeshHeap().GetHeap().GetGPUHandle(this->GetMeshHeap().GetMaterialDescStartIndex() + index));
		_bundleList.GetCommandList()->SetGraphicsRootDescriptorTable(SHADER_ROOT_PARAMATER_INDEX_TEXTURE, GetMeshHeap().GetHeap().GetGPUHandle(this->GetMeshHeap().GetTextureDescStartIndex() + _resourceData.lock()->_materials[index].textureTableIndex));
		_bundleList.GetCommandList()->SetGraphicsRootDescriptorTable(SHADER_ROOT_PARAMATER_INDEX_TOON_MAP, GetMeshHeap().GetHeap().GetGPUHandle(this->GetMeshHeap().GetTextureDescStartIndex() + _resourceData.lock()->_materials[index].toonIndex));
		_bundleList.GetCommandList()->SetGraphicsRootDescriptorTable(SHADER_ROOT_PARAMATER_INDEX_SPHERE_TEXTURE, GetMeshHeap().GetHeap().GetGPUHandle(this->GetMeshHeap().GetTextureDescStartIndex() + _resourceData.lock()->_materials[index].sphereIndex));

		_bundleList.GetCommandList()->DrawIndexedInstanced(_resourceData.lock()->_materials[index].surfaceCount, 1, surfaceOffset, 0, 0);
		surfaceOffset += _resourceData.lock()->_materials[index].surfaceCount;
	};
	_bundleList.CloseCommandList();
}

void K3D::MMDModel::Update()
{
	GameObject::UpdateTransformBuffer();
}

void K3D::MMDModel::Draw()
{

	this->_commandList.lock()->GetCommandList()->IASetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	this->BindingShaderObject();

	this->_commandList.lock()->GetCommandList()->SetGraphicsRootConstantBufferView(SHADER_ROOT_PARAMATER_INDEX_CAMERA, K3D::GetCamera().GetCameraBuffer().GetResource()->GetGPUVirtualAddress());
	//ID3D12DescriptorHeap* heaps[] = { this->GetMeshHeap().GetHeap().GetPtr() };
	//this->_commandList.lock()->GetCommandList()->SetDescriptorHeaps(1, heaps);
	GetMeshHeap().BindingDescriptorHeaps(_commandList);
	this->_commandList.lock()->GetCommandList()->ExecuteBundle(_bundleList.GetCommandList().Get());
}

K3D::Animator & K3D::MMDModel::Animator()
{
	return _animator;
}
