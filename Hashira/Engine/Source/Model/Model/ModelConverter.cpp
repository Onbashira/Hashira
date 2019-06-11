#include "ModelConverter.h"

#include "Engine/Source/CommandList/CommandList.h"
#include <memory>
#include <vector>
#include <map>
#include <memory>


constexpr unsigned int ToonMapTextureNum = 11;
constexpr float MODEL_INIT_ROTATION = 0.0f;
//固定値
const std::string ToonMapPath[ToonMapTextureNum] =
{
	"Data/toon/toon0.bmp",
	"Data/toon/toon01.bmp",
	"Data/toon/toon02.bmp",
	"Data/toon/toon03.bmp",
	"Data/toon/toon04.bmp",
	"Data/toon/toon05.bmp",
	"Data/toon/toon06.bmp",
	"Data/toon/toon07.bmp",
	"Data/toon/toon08.bmp",
	"Data/toon/toon09.bmp",
	"Data/toon/toon10.bmp",

};
const std::string NullTexturePath = "NULL_TEXTURE";

void CreatePMDBoneTree(K3D::MMDBoneTree* skelton, std::vector<K3D::PMDBone>& bonesInfo, K3D::MMDBoneNode * node);

void CreatePMXBoneTree(K3D::MMDBoneTree* skelton, std::vector<K3D::PMXBone>& bonesInfo, K3D::MMDBoneNode * node, std::vector<K3D::MMDIKData>& ikBoneIndex);

K3D::ModelConverter::ModelConverter()
{
}


K3D::ModelConverter::~ModelConverter()
{
}


std::shared_ptr<K3D::MMDModelResourceData> K3D::ModelConverter::ConvertPMDModel(std::shared_ptr<PMDModel> model)
{
	std::shared_ptr<K3D::MMDModelResourceData> convertModel = std::make_shared<MMDModelResourceData>();
	convertModel->_modelPath = model->modelPath;

	Matrix rotMat = Matrix::CreateRotationY(DegToRad(MODEL_INIT_ROTATION));

	//頂点コンバート
	{
		convertModel->_vertexes.resize(model->vertices.size());
		convertModel->_deformationData.resize(model->vertices.size());

		for (size_t i = 0; i < convertModel->_vertexes.size(); i++) {
			convertModel->_vertexes[i].pos = Vector3::Transform(model->vertices[i].pos, rotMat);
			convertModel->_vertexes[i].normal = Vector3::Transform(model->vertices[i].normal, rotMat);
			convertModel->_vertexes[i].texCoord = std::move(Vector2(model->vertices[i].texCoord.x, model->vertices[i].texCoord.y));
			convertModel->_vertexes[i].deformType = MMDWeightDeformType::BDEF2;
			convertModel->_vertexes[i].deformation.bdef2.boneIndex01 = model->vertices[i].boneIndex[0];
			convertModel->_vertexes[i].deformation.bdef2.boneIndex02 = model->vertices[i].boneIndex[1];
			convertModel->_vertexes[i].deformation.bdef2.boneWeight01 = model->vertices[i].boneWeight;
			convertModel->_vertexes[i].deformation.bdef2.boneWeight02 = 1.0f - model->vertices[i].boneWeight;

		}

		////GPU渡し用の変形データの構築
		//{
		//	convertModel->_deformation.Create(convertModel->_vertexes.size(), convertModel->_vertexes.size() * sizeof(MMDWeightDeform), &convertModel->_deformationData[0]);
		//}
	}
	//	インデックスリストコンバート
	{
		convertModel->_indexList.list.resize(model->verticesIndex.size());
		for (size_t i = 0; i < convertModel->_indexList.list.size(); i++) {
			convertModel->_indexList.list[i] = model->verticesIndex[i];
		}
	}
	//マテリアルコンバート
	{
		//疑似的なテクスチャテーブル
		std::map<std::string, int> textureTable;
		convertModel->_materials.resize(model->materials.size());
		for (size_t i = 0; i < convertModel->_materials.size(); i++) {
			convertModel->_materials[i].ambient = Vector3(0.4f, 0.4f, 0.4f);
			convertModel->_materials[i].diffuse = Vector4(Vector3(model->materials[i].diffuse), 1.0);
			convertModel->_materials[i].specular = Vector4(Vector3(model->materials[i].specular), model->materials[i].power);
			convertModel->_materials[i].emissive = model->materials[i].emissive;
			//通常テクスチャ
			if (textureTable.find(model->materials[i].textureName) == textureTable.end()) {
				//テクスチャがなかったならば
				convertModel->_materials[i].textureTableIndex = static_cast<unsigned int>(textureTable.size());
				textureTable[model->materials[i].textureName] = static_cast<unsigned int>(textureTable.size());

			}
			else {
				//もしあるならば
				convertModel->_materials[i].textureTableIndex = textureTable[model->materials[i].textureName];
			}

			//スフィアテクスチャロード
			//(今回は数合わせのために適当な文字列を入れて
			//ローダーのほうに実装した無効文字列セット時にロードされるNULLテクスチャをロードする。）
			if (textureTable.find(NullTexturePath) == textureTable.end()) {
				//テクスチャがなかったならば
				convertModel->_materials[i].sphereIndex = static_cast<unsigned int>(textureTable.size());
				textureTable[NullTexturePath] = static_cast<unsigned int>(textureTable.size());

			}
			else {
				//もしあるならば
				convertModel->_materials[i].sphereIndex = textureTable[NullTexturePath];
			}
			//Sphereマップはない…というか対応しない…こともない
			convertModel->_materials[i].sphereIndex = -1;
			convertModel->_materials[i].sphereBlendType = static_cast<int>(MMDSphereBlendType::NONE);
			//トゥーンテクスチャ
			if (textureTable.find(ToonMapPath[model->materials[i].toonIndex]) == textureTable.end()) {
				//テクスチャがなかったならば 
				convertModel->_materials[i].toonIndex = static_cast<unsigned int>(textureTable.size());
				textureTable[ToonMapPath[model->materials[i].toonIndex]] = static_cast<unsigned int>(textureTable.size());

			}
			else {
				//もしあるならば
				convertModel->_materials[i].toonIndex = textureTable[ToonMapPath[model->materials[i].toonIndex]];
			}
			convertModel->_materials[i].surfaceCount = model->materials[i].faceVertex;

		}
		//テクスチャの取得
		{
			std::vector<std::string> paths;
			paths.reserve(model->materials.size());

			for (std::map<std::string, int>::iterator itr = textureTable.begin(); itr != textureTable.end(); itr++) {

				paths.push_back(Util::GetRelativeTexturePath(model->modelPath, itr->first));

			}
			convertModel->_texturePaths = std::move(paths);
			TextureLoader::GetInstance().LoadModelTexture(D3D12System::GetInstance().GetCommandList("CommandList"), &D3D12System::GetInstance().GetMasterCommandQueue(), model->modelPath, convertModel->_texturePaths);

		}
	}

	//スケルトンのツリーを展開
	{
		//ルートボーンの定義（transformのPosと一致
		{
			convertModel->_boneTree = std::make_shared<MMDBoneTree>();
			convertModel->_boneTree->boneNum = static_cast<unsigned int>(model->bonesInfo.size());
			convertModel->_boneTree->rootBone.pos = model->bonesInfo.at(0).boneHeadPosition;
			convertModel->_boneTree->rootBone.parentIndex = model->bonesInfo.at(0).parentIndex;
			convertModel->_boneTree->rootBone.rotation = Quaternion::CreateIdentity();
			convertModel->_boneTree->rootBone.index = 0; // すべての親（ルートオブジェクト）

			convertModel->_boneTree->boneAccessor[model->bonesInfo.at(0).boneName] = &convertModel->_boneTree->rootBone;
			convertModel->_boneTree->boneNameAccessor[0] = model->bonesInfo.at(0).boneName;
			convertModel->_boneTree->bonesMatrix.resize(model->bones.size());
			std::fill(convertModel->_boneTree->bonesMatrix.begin(), convertModel->_boneTree->bonesMatrix.end(), Matrix::CreateIdentity());


		}
		CreatePMDBoneTree(convertModel->_boneTree.get(), model->bonesInfo, &convertModel->_boneTree->rootBone);
	}
	//スケルトンのIKをコンバート
	{
		convertModel->_boneTree->modelIKdata.resize(model->ikData.size());
		for (size_t i = 0; convertModel->_boneTree->modelIKdata.size(); i++) {
			convertModel->_boneTree->modelIKdata[i].targetBoneIndex = model->ikData[i].targetboneIndex;
			convertModel->_boneTree->modelIKdata[i].boneIndex = model->ikData[i].boneIndex;
			convertModel->_boneTree->modelIKdata[i].loopCount = model->ikData[i].recursiveCount;
			convertModel->_boneTree->modelIKdata[i].limitRadian = model->ikData[i].controlWeight;

			convertModel->_boneTree->modelIKdata[i].ikLinks.resize(model->ikData[i].childBone.size());
			//IKにリンクしているボーン情報の抜き取り
			for (size_t j = 0; j < convertModel->_boneTree->modelIKdata[i].ikLinks.size(); i++) {
				convertModel->_boneTree->modelIKdata[i].ikLinks[j].boneIndex = model->ikData[i].childBone[j];
				//PMDにはこの項目はないので困っている。一応なしでいっか
				//convertModel->_modelIKdata[i].ikLinks[j].hasLimit = ？;
				convertModel->_boneTree->modelIKdata[i].ikLinks[j].hasLimit = false;
				//条件で下限値上限値が有効かどうかを判断しなければいけないのだが、PMDにはこのデータがないのだ…困った
				//条件が真ならば下限値上限値が存在しているとして、設定を行う
				if (convertModel->_boneTree->modelIKdata[i].ikLinks[j].hasLimit) {
					convertModel->_boneTree->modelIKdata[i].ikLinks[j].limits.maximumAngle = Vector3::zero;
					convertModel->_boneTree->modelIKdata[i].ikLinks[j].limits.minimamAngle = Vector3::zero;
				}
				else {
					convertModel->_boneTree->modelIKdata[i].ikLinks[j].limits.maximumAngle = Vector3::zero;
					convertModel->_boneTree->modelIKdata[i].ikLinks[j].limits.minimamAngle = Vector3::zero;
				}
			}

		}
	}
	return convertModel;
};

std::shared_ptr<K3D::MMDModelResourceData> K3D::ModelConverter::ConvertPMXModel(std::shared_ptr<PMXModel> model)
{
	std::shared_ptr<MMDModelResourceData> convertModel = std::make_shared<MMDModelResourceData>();
	convertModel->_modelPath = model->modelPath;

	//頂点コンバート
	{
		Matrix rotMat = Matrix::CreateRotationY(DegToRad(MODEL_INIT_ROTATION));

		convertModel->_vertexes.resize(model->vertcies.size());
		convertModel->_deformationData.resize(model->vertcies.size());

		for (size_t i = 0; i < convertModel->_vertexes.size(); i++) {
			convertModel->_vertexes[i].pos = Vector3::Transform(model->vertcies[i].position, rotMat);
			convertModel->_vertexes[i].normal = Vector3::Transform(model->vertcies[i].normal, rotMat);;
			convertModel->_vertexes[i].texCoord = model->vertcies[i].uv;
			convertModel->_vertexes[i].deformType = static_cast<MMDWeightDeformType>(model->vertcies[i].weightDeformType);
			convertModel->_vertexes[i].deformation.sdef = model->vertcies[i].weightDeform.sdef;

			//convertModel->_deformationData[i].deformType = static_cast<int>(model->vertcies[i].weightDeformType);
			//convertModel->_deformationData[i].boneIndex01 = model->vertcies[i].weightDeform.qdef.boneIndex01;
			//convertModel->_deformationData[i].boneIndex02 = model->vertcies[i].weightDeform.qdef.boneIndex02;
			//convertModel->_deformationData[i].boneIndex03 = model->vertcies[i].weightDeform.qdef.boneIndex03;
			//convertModel->_deformationData[i].boneIndex04 = model->vertcies[i].weightDeform.qdef.boneIndex04;
			//convertModel->_deformationData[i].boneWeight01 = model->vertcies[i].weightDeform.qdef.boneWeight01;
			//convertModel->_deformationData[i].boneWeight02 = model->vertcies[i].weightDeform.qdef.boneWeight02;
			//convertModel->_deformationData[i].boneWeight03 = model->vertcies[i].weightDeform.qdef.boneWeight03;
			//convertModel->_deformationData[i].boneWeight04 = model->vertcies[i].weightDeform.qdef.boneWeight04;
			//convertModel->_deformationData[i].c = model->vertcies[i].weightDeform.sdef.c;
			//convertModel->_deformationData[i].r0 = model->vertcies[i].weightDeform.sdef.r0;
			//convertModel->_deformationData[i].r1 = model->vertcies[i].weightDeform.sdef.r1;

		}

		////GPU渡し用の変形データの構築
		//{
		//	convertModel->_deformation.Create(convertModel->_vertexes.size(), convertModel->_vertexes.size() * sizeof(MMDWeightDeform), &convertModel->_deformationData[0]);
		//}
	}
	//	インデックスリストコンバート
	{
		convertModel->_indexList.list.resize(model->surfaces.surfaceIndex.size());
		for (size_t i = 0; i < convertModel->_indexList.list.size(); i++) {
			convertModel->_indexList.list[i] = model->surfaces.surfaceIndex[i];
		}
	}
	//マテリアルコンバート
	{
		//テクスチャパス（テクスチャテーブル）
		//基本テクスチャおよびユーザートゥーンテクスチャ、ユーザースフィアマップの名前を取得
		std::vector<std::string> textureTable;
		textureTable.resize(model->paths.path.size());
		for (size_t i = 0; i < model->paths.path.size(); i++) {
			textureTable[i] = Util::GetRelativeTexturePath(model->modelPath, Util::WStringToString(model->paths.path[i]));
		}
		//念のためのヌルテクスチャへのパスを押しこむ
		textureTable.push_back(NullTexturePath);
		//ヌルテクスチャへのインデックスを持っておく
		int nullTextureIndex = static_cast<int>(textureTable.size() - 1);
		convertModel->_materials.resize(model->materials.size());
		for (size_t i = 0; i < convertModel->_materials.size(); i++) {
			convertModel->_materials[i].ambient = model->materials[i].ambientColor;
			convertModel->_materials[i].diffuse = model->materials[i].diffuseColor;
			convertModel->_materials[i].specular = Vector4(Vector3(model->materials[i].specularColor), model->materials[i].specularStrength);
			convertModel->_materials[i].emissive = Vector3::zero;
			//トゥーンテクスチャ
			//個別トゥーンフラグが真ならば
			if (model->materials[i].toonReference) {
				convertModel->_materials[i].toonIndex = model->materials[i].toonValue.individualToon;
			}
			else {
				convertModel->_materials[i].toonIndex = model->materials[i].toonValue.individualToon;
				//この時点で共有トゥーンマップを使うことは確定しいない。不正値チェックを行う。
				//もし不正値が代入されていたならばNullTextureを代入
				if (convertModel->_materials[i].toonIndex != -1) {
					if (std::find(textureTable.begin(), textureTable.end(), ToonMapPath[convertModel->_materials[i].toonIndex]) == textureTable.end()) {
						textureTable.push_back(ToonMapPath[convertModel->_materials[i].toonIndex]);
					}
				}
				else {
					textureTable.push_back(NullTexturePath);
					convertModel->_materials[i].toonIndex = nullTextureIndex;
				}
			}
			//PMXの場合、通常テクスチャインデックスはすでに作られている。トゥーンマップインデックスも同様である。
			//それぞれテクスチャ系は不正値（－１）が入れられている場合の処理が必要。
			//もしなかったならば、Nullテクスチャをロードするようにする。

			//通常テクスチャ
			//不正なテクスチャインデックスじゃなかったら
			if (model->materials[i].textureIndex != -1) {
				convertModel->_materials[i].textureTableIndex = model->materials[i].textureIndex;
			}
			else {
				//インデックスの調整とNULLテクスチャをテクスチャパスへぶち込む
				convertModel->_materials[i].textureTableIndex = nullTextureIndex;
			}
			//SphereマップはPMXだと対応
			//スフィアテクスチャロード
			if (model->materials[i].environmentTextureIndex != -1) {
				convertModel->_materials[i].sphereIndex = model->materials[i].environmentTextureIndex;
				convertModel->_materials[i].sphereBlendType = static_cast<int>(model->materials[i].environmentBlendFlags);

			}
			else {
				convertModel->_materials[i].sphereIndex = nullTextureIndex;
				convertModel->_materials[i].sphereBlendType = static_cast<int>(MMDSphereBlendType::NONE);

			}

			//サーフェスカウントのセット
			convertModel->_materials[i].surfaceCount = model->materials[i].surfaceCount;
		}

		//テクスチャパスの取得およびリソースバッファの確保
		{
			convertModel->_texturePaths = std::move(textureTable);
			TextureLoader::GetInstance().LoadModelTexture(D3D12System::GetInstance().GetCommandList("CommandList"), &D3D12System::GetInstance().GetMasterCommandQueue(), model->modelPath, convertModel->_texturePaths);

		}
	}

	//スケルトンのツリーを展開 およびIKデータを生成
	{
		std::vector<MMDIKData> ikDataMap;
		//ルートボーンの定義（transformのPosと一致
		{
			convertModel->_boneTree = std::make_shared<MMDBoneTree>();
			convertModel->_boneTree->boneNum = static_cast<unsigned int>(model->bones.size());

			convertModel->_boneTree->rootBone.pos = model->bones.at(0).bonePosition;
			convertModel->_boneTree->rootBone.parentIndex = model->bones.at(0).parentBoneIndex;
			convertModel->_boneTree->rootBone.rotation = Quaternion::CreateIdentity();
			convertModel->_boneTree->rootBone.index = 0; // すべての親（ルートオブジェクト）
			convertModel->_boneTree->boneAccessor[Util::WStringToString(model->bones.at(0).boneNameLocal)] = &convertModel->_boneTree->rootBone;
			convertModel->_boneTree->boneNameAccessor[0] = Util::WStringToString(model->bones.at(0).boneNameLocal);

			convertModel->_boneTree->bonesMatrix.resize(model->bones.size());
			std::fill(convertModel->_boneTree->bonesMatrix.begin(), convertModel->_boneTree->bonesMatrix.end(), Matrix::CreateIdentity());
		}
		CreatePMXBoneTree(convertModel->_boneTree.get(), model->bones, &(convertModel->_boneTree->rootBone), ikDataMap);

		//スケルトンのIKをコンバート
		{
			convertModel->_boneTree->modelIKdata.reserve(ikDataMap.size());
			for (std::vector<MMDIKData>::iterator itr = ikDataMap.begin(); itr != ikDataMap.end(); itr++) {
				convertModel->_boneTree->modelIKdata.push_back(*itr);
			}
		}
	}

	///未対応
	////モーフ
	////剛体
	////ジョイント
	///

	return convertModel;
}

//変換したオリジナルモデルデータから実モデルデータを作成
std::shared_ptr<K3D::MMDModel> K3D::ModelConverter::ExtructMMDModel(std::weak_ptr<PipelineStateObject> pso, std::weak_ptr<RootSignature> rootSignature, std::weak_ptr<CommandList> list, std::shared_ptr<MMDModelResourceData> modelResource)
{
	std::shared_ptr<K3D::MMDModel> extructedModel(std::make_shared<K3D::MMDModel>());
	std::vector<MMDRendererMaterial> materials;
	extructedModel->_bundleList.SetName("BundleList");
	//マテリアル抽出
	materials.resize(modelResource->_materials.size());
	for (UINT i = 0; i < materials.size(); i++) {
		materials[i].diffuse = modelResource->_materials[i].diffuse;
		materials[i].ambient = modelResource->_materials[i].ambient;
		materials[i].specular = modelResource->_materials[i].specular;
		materials[i].emissive = modelResource->_materials[i].emissive;
		materials[i].sphereBlendType = modelResource->_materials[i].sphereBlendType;
	}
	//頂点抽出
	extructedModel->_vertexes.reserve(modelResource->_vertexes.size());
	for (UINT i = 0; i < extructedModel->_vertexes.size(); i++) {
		extructedModel->_vertexes[i].pos = modelResource->_vertexes[i].pos;
		extructedModel->_vertexes[i].normal = modelResource->_vertexes[i].normal;
		extructedModel->_vertexes[i].texCoord = modelResource->_vertexes[i].texCoord;
	}
	extructedModel->_resourceData = modelResource;
	{
		//オリジナルデータからインデックスバッファ、頂点バッファを作成、さらにリソースに対して名前を付ける。（デバッグ時に確認しやすくするため）；
		extructedModel->GetMeshBuffer().GetIBO().Create(modelResource->_indexList.list.size() * sizeof(UINT), sizeof(UINT), &modelResource->_indexList.list[0]);
		extructedModel->GetMeshBuffer().GetIBO().SetName("MMDModelIndexData");

		//extructedModel->GetDefaultVBO()->Create(extructedModel->GetDefaultVertexData().size() * sizeof(Vertex3D), sizeof(Vertex3D), &extructedModel->_vertexes[0]);
		extructedModel->GetMeshBuffer().InitializeVBO(extructedModel->_vertexes.size() * sizeof(Vertex3D), sizeof(Vertex3D), &extructedModel->_vertexes[0]);
		extructedModel->GetMeshBuffer().GetVBO().SetName("MMDModelVertexData");
	}
	//マテリアルバッファを作成。256Byteアラインでマテリアル数だけ領域を確保
	extructedModel->GetMeshHeap().GetMaterialBufffer().Create(materials.size() * Util::Alignment256Bytes(sizeof(MMDRendererMaterial)));
	extructedModel->GetMeshHeap().GetMaterialBufffer().SetName("ModelMaterialBuffer");

	//ヒープの作成・各情報のスタートポイントの設定
	{
		unsigned int heapSize = static_cast<unsigned int>(1 + modelResource->_materials.size() + modelResource->_texturePaths.size()); //カメラ情報を除いたモデルに必要な情報の数を加算。（変形情報、マテリアル数、テクスチャ数）
		extructedModel->GetMeshHeap().GetHeap().Create(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, heapSize);
		extructedModel->GetMeshHeap().GetHeap().SetName("ModelHeap");

		//extructedModel->_transformStartPoint = 0;
		extructedModel->GetMeshHeap().SetTransformDescStartIndex(0);
		extructedModel->GetMeshHeap().SetMaterialDescStartIndex(extructedModel->GetMeshHeap().GetTransformDescStartIndex() + 1);
		extructedModel->GetMeshHeap().SetTextureDescStartIndex(extructedModel->GetMeshHeap().GetMaterialDescStartIndex() + static_cast<unsigned int>(modelResource->_materials.size()));

		//extructedModel->_textureStartPoint = extructedModel->_materialStartPoint + static_cast<unsigned int>(modelResource->_materials.size());

	}
	//変形情報の構築およびそれに対しての見方を定義
	{

		extructedModel->InitalizeTransformBuffer(Util::Alignment256Bytes(sizeof(Transform)));
		extructedModel->_transformBuffer.SetName("MMDModelTransformBuffer");

		D3D12_CONSTANT_BUFFER_VIEW_DESC view{};

		view.BufferLocation = extructedModel->_transformBuffer.GetResource()->GetGPUVirtualAddress();
		view.SizeInBytes = static_cast<unsigned int>(Util::Alignment256Bytes(sizeof(Transform)));

		extructedModel->_transformBuffer.CreateView(view, extructedModel->GetMeshHeap().GetHeap().GetCPUHandle(extructedModel->GetMeshHeap().GetTransformDescStartIndex()));

		//位置情報の初期設定
		{
			extructedModel->SetRotation(Quaternion::CreateIdentity());
			extructedModel->SetPos(Vector3::zero);
			extructedModel->SetScale(Vector3::one);
			extructedModel->TransformUpdate();
			extructedModel->Update();
		}
	}
	//マテリアルの書き込みとデスクリプタ作成
	{
		auto gpuPtr = extructedModel->GetMeshHeap().GetMaterialBufffer().GetResource()->GetGPUVirtualAddress();
		D3D12_CONSTANT_BUFFER_VIEW_DESC view{};
		UINT writeOffset = 0;
		for (UINT i = 0; i < materials.size(); i++) {

			//確保したリソースの領域に書き込み
			extructedModel->GetMeshHeap().GetMaterialBufffer().Update(&materials[i], sizeof(MMDRendererMaterial), writeOffset);

			//デスクリプタが見る、GPUにミラーリングされた領域をスライドする
			view.BufferLocation = gpuPtr;
			view.SizeInBytes = static_cast<unsigned int>(Util::Alignment256Bytes(sizeof(MMDRendererMaterial)));

			extructedModel->GetMeshHeap().GetMaterialBufffer().CreateView(view, extructedModel->GetMeshHeap().GetHeap().GetCPUHandle(extructedModel->GetMeshHeap().GetMaterialDescStartIndex() + i));

			gpuPtr += static_cast<unsigned int>(Util::Alignment256Bytes(sizeof(MMDRendererMaterial)));
			writeOffset += static_cast<unsigned int>(Util::Alignment256Bytes(sizeof(MMDRendererMaterial)));
		}
	}
	//テクスチャリソースのビューの作成
	{
		TextureLoader::GetInstance().LoadModelTexture(D3D12System::GetInstance().GetCommandList("CommandList"), &D3D12System::GetInstance().GetMasterCommandQueue(), extructedModel->GetMeshHeap().GetHeap(), extructedModel->GetMeshHeap().GetTextureDescStartIndex(), modelResource->_modelPath, modelResource->_texturePaths);
	}

	//ローダーにセットされたコマンドリストをモデルにセット
	extructedModel->_commandList = list;

	//モデルにパイプラインとルートシグネチャをセット
	extructedModel->SetPipelineState(pso);
	extructedModel->SetRootSignature(rootSignature);
	extructedModel->RegisterToBundle();
	return extructedModel;
}

void CreatePMXBoneTree(K3D::MMDBoneTree* skelton, std::vector<K3D::PMXBone>& bonesInfo, K3D::MMDBoneNode * node, std::vector<K3D::MMDIKData>& ikDataMap)
{
	Matrix rotMat = Matrix::CreateRotationY(DegToRad(MODEL_INIT_ROTATION));

	for (auto info = bonesInfo.begin(); info != bonesInfo.end(); info++) {
		if (node->index == info->parentBoneIndex) {
			K3D::MMDBoneNode newNode;
			//ボーンの位置を180度回転
			newNode.pos = Vector3::Transform(info->bonePosition, rotMat);
			newNode.parentIndex = node->index;
			newNode.rotation = Quaternion::CreateIdentity();
			newNode.index = static_cast<UINT>(std::distance(bonesInfo.begin(), info));

			newNode.boneOption.inheritBone = info->inheritBone;
			newNode.boneOption.externalParentBoneIndex = info->externalParent;
			newNode.boneOption.fixidAxis = info->fixedAxis;
			newNode.boneOption.localAxis = info->boneAxis;

			newNode.boneOptionFlags = info->boneFlags;

			node->childrenBone.push_back(newNode);
			skelton->boneNameAccessor[newNode.index] = K3D::Util::WStringToString(info->boneNameLocal);

			//ボーンがIK成分を持っているなら
			if ((static_cast<int>(info->boneFlags) & 0x0020) == 0x0020) {
				K3D::MMDIKData ikData;
				ikData.boneIndex = newNode.index;
				ikData.limitRadian = info->ikData.limitRadian;
				ikData.loopCount = info->ikData.loopCount;
				ikData.targetBoneIndex = info->ikData.targetIndex;
				ikData.ikLinks.resize(info->ikData.ikLinks.size());
				for (size_t i = 0; i < ikData.ikLinks.size(); i++) {
					ikData.ikLinks[i].boneIndex = info->ikData.ikLinks[i].boneIndex;
					ikData.ikLinks[i].hasLimit = info->ikData.ikLinks[i].hasLimit;
					ikData.ikLinks[i].limits = info->ikData.ikLinks[i].limits;
				}
				ikDataMap.push_back(ikData);
			}
		}
	}

	for (unsigned int i = 0; i < node->childrenBone.size(); i++)
	{
		skelton->boneAccessor[skelton->boneNameAccessor[node->childrenBone[i].index]] = &node->childrenBone[i];

	}

	for (unsigned int i = 0; i < node->childrenBone.size(); i++)
	{
		//再帰
		CreatePMXBoneTree(skelton, bonesInfo, &node->childrenBone[i], ikDataMap);
	}
}

void CreatePMDBoneTree(K3D::MMDBoneTree* skelton, std::vector<K3D::PMDBone>& bonesInfo, K3D::MMDBoneNode * node)
{

	Matrix rotMat = Matrix::CreateRotationY(DegToRad(MODEL_INIT_ROTATION));

	for (auto info = bonesInfo.begin(); info != bonesInfo.end(); info++) {
		if (node->index == info->parentIndex) {
			K3D::MMDBoneNode newNode;
			//ボーンの位置を180度回転
			newNode.pos = Vector3::Transform(info->boneHeadPosition, rotMat);
			newNode.parentIndex = node->index;
			newNode.rotation = Quaternion::CreateIdentity();
			newNode.index = static_cast<UINT>(std::distance(bonesInfo.begin(), info));

			newNode.boneOption = {};

			newNode.boneOptionFlags = 0x0002;
			node->childrenBone.push_back(std::move(newNode));
			skelton->boneAccessor[info->boneName] = &node->childrenBone.back();
		}
	}

	for (unsigned int i = 0; i < node->childrenBone.size(); i++)
	{
		skelton->boneAccessor[skelton->boneNameAccessor[node->childrenBone[i].index]] = &node->childrenBone[i];

	}

	for (auto& child : node->childrenBone)
	{
		//再帰rekure
		CreatePMDBoneTree(skelton, bonesInfo, &child);
	}
}
