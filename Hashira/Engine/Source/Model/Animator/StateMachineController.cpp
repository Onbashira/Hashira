#include <DirectXMath.h>
#include <memory>
#include <algorithm>
#include "StateMachineController.h"
#include "TransitionConditions.h"
#include "../Model/MMDModel/MMDModelResourceData.h"
#include "../Util/Math.h"
#include "../Util/Utility.h"


K3D::StateMachineController::StateMachineController()
{
}


K3D::StateMachineController::~StateMachineController()
{
}


void K3D::StateMachineController::CCDIKSolver(K3D::MMDIKData & ikData, Vector3 & loction, unsigned int iterationNum, unsigned int numBones, unsigned int numMaxBones)
{
	Vector3 ikEffecterPos = _skelton.lock()->bonesMatrix[ikData.boneIndex].axisW; //アニメーションの後に行う予定なので決まっているはず（ターゲットボーンが目指す目的地？）
	Vector3 ikTargetBonePos = _skelton.lock()->bonesMatrix[ikData.targetBoneIndex].axisW; //IKのターゲットボーンの位置

	std::vector<Vector3> tempLocationMat;
	tempLocationMat.reserve(ikData.ikLinks.size());
	for (auto& ikLink : ikData.ikLinks) {
		tempLocationMat.push_back(_skelton.lock()->bonesMatrix[ikLink.boneIndex].axisW);
	}

	Vector3 ikEffecterRootVec = ikEffecterPos - tempLocationMat[ikData.ikLinks.size() - 1];
	Vector3 ikTargetBoneRootVec = ikTargetBonePos - tempLocationMat[ikData.ikLinks.size() - 1];
	float   ikMaxLength = ikEffecterRootVec.Length();
	//ループ前回転計算
	{

	}

	int chainCount = static_cast<int>(ikData.ikLinks.size());
	for (int count = 0; count < ikData.loopCount; ++count) {
		//もしもターゲット位置がIKボーン位置と同じならばこの処理を終了する
		for (int i = 0; i < ikData.ikLinks.size(); i++) {

			if (ikEffecterPos == ikTargetBonePos) {
				break;
			}

			//現在のボーン位置からIKエフェクタ、およびターゲットボーンへのベクトルを作成
			Vector3 originVec = ikEffecterPos - tempLocationMat[i];
			Vector3 transVec = ikTargetBonePos - tempLocationMat[i];

			if (abs(originVec.Length()) <= 0.0001f || abs(transVec.Length()) <= 0.0001f) {
				return;
			}
			//回転軸の計算
			Vector3 axis = Vector3::Cross(Vector3::Normalize(originVec), Vector3::Normalize(transVec));

			if (_skelton.lock()->boneNameAccessor.at(ikData.ikLinks.at(i).boneIndex).find("ひざ") != std::string::npos ||
				_skelton.lock()->boneNameAccessor.at(ikData.ikLinks.at(i).boneIndex).find("knee") != std::string::npos)
			{
				//回転軸はXに
				axis = Vector3::left;
			}
			else {
				if (axis.Length() == 0) {
					return;
				}
			}

			//二角のなす角を計算
			auto betweenRot = Vector3::ComputeCrossingAngle(Vector3::Normalize(originVec), Vector3::Normalize(transVec));
			if (abs(betweenRot) == 0.0f) {
				return;
			}

			//角度厳密性チェック
			{
				//下限上限クランプ(もしこのIKが各軸ごとの角度制限を用いるなら)
				if (ikData.ikLinks[i].hasLimit) {
					

				}
				//
				//betweenRot = ::Clamp(betweenRot, -ikData.limitRadian, ikData.limitRadian);
				//ループ一回の制限適用
				betweenRot = ::Clamp(betweenRot, 0.0f, ikData.limitRadian);


			}

			//任意軸回転を行う四元数を作成
			Quaternion rot = Quaternion::CreateFromAxisAngles(Vector3::Normalize(axis), betweenRot);
			//ボーンから原点へのベクトル
			Vector3 offset = _skelton.lock()->bonesMatrix[ikData.ikLinks.at(i).boneIndex].axisW;
			//合成行列　-T*R*T
			Matrix afinMat = Matrix::CreateTranslationMatrix(-offset) * Matrix::CreateFromQuaternion(rot) * Matrix::CreateTranslationMatrix(offset);

			_skelton.lock()->bonesMatrix[ikData.ikLinks.at(i).boneIndex] *= afinMat;

			//
			offset = tempLocationMat[i];
			//オフセット値が違うことに留意
			afinMat = Matrix::CreateTranslationMatrix(-offset) * Matrix::CreateFromQuaternion(rot) * Matrix::CreateTranslationMatrix(offset);

			for (int j = i - 1; j >= 0; --j) {
				tempLocationMat[j] = Vector3::Transform(tempLocationMat[j], afinMat);
			}
		}
	}
}

void K3D::StateMachineController::SetModelSkelton(std::weak_ptr<K3D::MMDBoneTree> skelton)
{
	this->_skelton = skelton;
}

void K3D::StateMachineController::ResetSkelton()
{
	std::fill(_skelton.lock()->bonesMatrix.begin(), _skelton.lock()->bonesMatrix.end(), Matrix::CreateIdentity());
}

//ボーン自体の変形（ブレンドは考慮しない）
void K3D::StateMachineController::PlayAnimation(unsigned int& time)
{

	for (auto& frames : _currentNode.lock()->_animation.lock()->keyFrame) {
		//インデックスの設定
		if (!_skelton.expired()) {
			int index = _skelton.lock()->boneAccessor.at(frames.first)->index;

			//現在あるべきボーンのキーフレームの検索 （負荷削減できる）
			auto rbegin = frames.second.rbegin();
			auto rend = frames.second.rend();
			auto revIt = std::find_if(frames.second.rbegin(), frames.second.rend(), [&](K3D::MotionKeyFrameData& md) { return md.frameNo <= _currentNode.lock()->_animation.lock()->frame; });
			//補完係数を算出
			float t;
			//0divを考慮

			if (revIt != frames.second.rbegin()) {
				t = static_cast<float>((_currentNode.lock()->_animation.lock()->frame - revIt->frameNo)) / static_cast<float>((revIt.base()->frameNo - revIt->frameNo));
			}
			else {
				t = 0.0f;
			}
			//回転補間
			Matrix rotationMat;
			Vector3 location;
			if (!(revIt != frames.second.rbegin())) {
				rotationMat = Matrix::CreateFromQuaternion(revIt->quaternion);
				location = revIt->location;
			}
			else {
				rotationMat = Matrix::CreateFromQuaternion(Quaternion::Slerp(revIt->quaternion, revIt.base()->quaternion, t));
				location = Vector3::Lerp(revIt->location, revIt.base()->location, t);

			}
			//原点へのベクトル
			Vector3 offsetToOrigin = _skelton.lock()->boneAccessor[frames.first]->pos;
			Matrix bone = {};

			bone = Matrix::CreateTranslationMatrix(-offsetToOrigin);
			//任意軸回転（クォターニオン）を回転行列へ直してつかう
			bone *= rotationMat;
			bone *= Matrix::CreateTranslationMatrix(offsetToOrigin);
			_skelton.lock()->bonesMatrix.at(index) = bone;
		}
	}
	//変更適用
	ApplyRecursionMatirixBone(&_skelton.lock()->rootBone);
	++time;
	if (++_currentNode.lock()->_animation.lock()->frame >= _currentNode.lock()->_animation.lock()->vmdMotion.back().frameNo) {
		_currentNode.lock()->_animation.lock()->frame = 0;
	}

}

void K3D::StateMachineController::AttachAnimation(std::string animationName, std::weak_ptr<K3D::MotionData> motionData)
{
	if (_stateMap.find(animationName) != _stateMap.end()) {
		//すでにある
		return;
	}

	std::shared_ptr<StateNode>animationNode =  std::make_shared<StateNode>();
	animationNode->_animation = motionData;
	animationNode->_stateName = animationName;
	_stateMap[animationName] = animationNode;
	if (_stateMap.size() == 1) {
		_currentNode = _stateMap[animationName];
	}
}

void K3D::StateMachineController::ConnectingAnimation(std::string from, std::string to, K3D::TransitionConditions conditions)
{

	assert(_stateMap.find(from) != _stateMap.end());
	assert(_stateMap.find(to) != _stateMap.end());
	_stateMap[from]->SetTransitionConditions(std::pair<std::string , std::shared_ptr<K3D::StateNode>>(to, _stateMap.find(to)->second), conditions);

}

void K3D::StateMachineController::ApplyRecursionMatirixBone(K3D::MMDBoneNode* node)
{
	for (auto& nodeChildren : node->childrenBone) {
		node->rotation *= nodeChildren.rotation;
	}
	for (auto& nodeChildren : node->childrenBone) {
		ApplyRecursionMatirixBone(&nodeChildren);
	}
}