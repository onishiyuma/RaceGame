#include "k2EngineLowPreCompile.h"
#include "MeshParts.h"
#include "Skeleton.h"
#include "Material.h"
#include "IndexBuffer.h"

namespace
{
	using MeshResourceArray = std::vector<std::shared_ptr<nsK2EngineLow::MeshResource>>;

	// NOTE:
	// Bank自身がshared_ptrを保持しているため、Modelをすべて破棄しても
	// MeshResourceは解放されず、基本的にアプリケーション終了まで保持される。

	// TODO:
	// 不要になったMeshResourceをBankから解放できる仕組みを追加する。

	// メッシュリソースのバンク。tkmファイルのポインタをキーにして、メッシュリソースをまとめる。
	std::unordered_map<const nsK2EngineLow::TkmFile*, MeshResourceArray> g_meshResourceBank;
}

namespace nsK2EngineLow {
	MeshParts::~MeshParts()
	{
		for (auto& mesh : m_meshs) {
			//マテリアルを削除。
			for (auto& mat : mesh->m_materials) {
				delete mat;
			}
			//メッシュを削除。
			delete mesh;
		}
	}
	void MeshParts::InitFromTkmFile(
		const TkmFile& tkmFile,
		const char* fxFilePath,
		const char* vsEntryPointFunc,
		const char* vsSkinEntryPointFunc,
		const char* psEntryPointFunc,
		void* expandData,
		int expandDataSize,
		const std::array<IShaderResource*, MAX_MODEL_EXPAND_SRV>& expandShaderResourceView,
		const std::array<DXGI_FORMAT, MAX_RENDERING_TARGET>& colorBufferFormat,
		AlphaBlendMode alphaBlendMode,
		bool isDepthWrite,
		bool isDepthTest,
		D3D12_CULL_MODE cullMode
	)
	{

		//tkmファイルのポインタを保持する。
		m_tkmFile = &tkmFile;//追加（高橋）（IB/VB共有）

		m_meshs.resize(tkmFile.GetNumMesh());
		int meshNo = 0;
		int materianNo = 0;
		tkmFile.QueryMeshParts([&](const TkmFile::SMesh& mesh) {
			//tkmファイルのメッシュ情報からメッシュを作成する。
			CreateMeshFromTkmMesh(
				mesh,
				meshNo,
				materianNo,
				fxFilePath,
				vsEntryPointFunc,
				vsSkinEntryPointFunc,
				psEntryPointFunc,
				colorBufferFormat,
				alphaBlendMode,
				isDepthWrite,
				isDepthTest,
				cullMode
			);
			meshNo++;
			});
		//共通定数バッファの作成。
		m_commonConstantBuffer.Init(sizeof(SConstantBuffer), nullptr);
		//ユーザー拡張用の定数バッファを作成。
		if (expandData) {
			m_expandConstantBuffer.Init(expandDataSize, nullptr);
			m_expandData = expandData;
		}
		for (int i = 0; i < MAX_MODEL_EXPAND_SRV; i++) {
			m_expandShaderResourceView[i] = expandShaderResourceView[i];
		}
		//ディスクリプタヒープを作成。
		CreateDescriptorHeaps();
	}
	void MeshParts::ReInitMaterials(const MaterialReInitData& reInitData)
	{
		for (int i = 0; i < MAX_MODEL_EXPAND_SRV; i++) {
			m_expandShaderResourceView[i] = reInitData.m_expandShaderResoruceView[i];
		}
		//ディスクリプタヒープを作成。
		CreateDescriptorHeaps();
	}
	void MeshParts::CreateDescriptorHeaps()
	{
		// 必要なディスクリプタヒープの総数を計算する。
		int srvNo = 0;
		int cbNo = 0;
		for (auto& mesh : m_meshs) {
			for (int matNo = 0; matNo < mesh->m_materials.size(); matNo++) {
				srvNo += NUM_SRV_ONE_MATERIAL;
				cbNo += NUM_CBV_ONE_MATERIAL;
			}
		}
		// シェーダーリソースビューと定数バッファの登録できるサイズをリサイズする。
		m_descriptorHeap.ResizeShaderResource(srvNo);
		m_descriptorHeap.ResizeConstantBuffer(cbNo);
		// UAVいらない。
		m_descriptorHeap.ResizeUnorderAccessResource(0);
		//ディスクリプタヒープを構築していく。
		srvNo = 0;
		cbNo = 0;
		for (auto& mesh : m_meshs) {
			for (int matNo = 0; matNo < mesh->m_materials.size(); matNo++) {

				//ディスクリプタヒープにディスクリプタを登録していく。
				m_descriptorHeap.RegistShaderResource(srvNo, mesh->m_materials[matNo]->GetAlbedoMap());		//アルベドマップ。
				m_descriptorHeap.RegistShaderResource(srvNo + 1, mesh->m_materials[matNo]->GetNormalMap());		//法線マップ。
				m_descriptorHeap.RegistShaderResource(srvNo + 2, mesh->m_materials[matNo]->GetSpecularMap());		//スペキュラマップ。
				m_descriptorHeap.RegistShaderResource(srvNo + 3, m_boneMatricesStructureBuffer);							//ボーンのストラクチャードバッファ。
				for (int i = 0; i < MAX_MODEL_EXPAND_SRV; i++) {
					if (m_expandShaderResourceView[i]) {
						m_descriptorHeap.RegistShaderResource(srvNo + EXPAND_SRV_REG__START_NO + i, *m_expandShaderResourceView[i]);
					}
				}
				srvNo += NUM_SRV_ONE_MATERIAL;
				m_descriptorHeap.RegistConstantBuffer(cbNo, m_commonConstantBuffer);
				if (m_expandConstantBuffer.IsValid()) {
					m_descriptorHeap.RegistConstantBuffer(cbNo + 1, m_expandConstantBuffer);
				}
				cbNo += NUM_CBV_ONE_MATERIAL;
			}
		}
		m_descriptorHeap.Commit();
	}
	void MeshParts::CreateMeshFromTkmMesh(
		const TkmFile::SMesh& tkmMesh,
		int meshNo,
		int& materialNum,
		const char* fxFilePath,
		const char* vsEntryPointFunc,
		const char* vsSkinEntryPointFunc,
		const char* psEntryPointFunc,
		const std::array<DXGI_FORMAT, MAX_RENDERING_TARGET>& colorBufferFormat,
		AlphaBlendMode alphaBlendMode,
		bool isDepthWrite,
		bool isDepthTest,
		D3D12_CULL_MODE cullMode
	) {

		//変更、追加箇所 （高橋）（IB/VB共有）ここから//////////////////////////////////////////////////

		//NOTE:
		//無駄なGPUメモリを使わないように
		//同じtkmなら頂点バッファ―とインデックスバッファーを共有する。
		//なので新しいtkmのメッシュが来れば登録して、既存のtkmのメッシュが来ればそれを参照するだけにする。
		//ただ、影モデルを作るときに同じモデルを使うけどマテリアルは変えるので
		//マテリアルはtkmごとに共有しないで、メッシュごとに作る。

		auto mesh = new SMesh;

		// このTKM用のResource配列を取得
		auto& resources = g_meshResourceBank[m_tkmFile];

		if (resources.size() <= meshNo) {
			resources.resize(meshNo + 1);
		}

		if (resources[meshNo] == nullptr) {
			// まだ登録されていないメッシュならリソースを作成する


			// 共有リソースを作成する。
			auto resource = std::make_shared<MeshResource>();



			/////////////////////////
			// 頂点バッファを作成。
			//////////////////////////

			int numVertex = static_cast<int>(tkmMesh.vertexBuffer.size());
			// 1頂点のバイト数を取得
			int vertexStride = sizeof(TkmFile::SVertex);

			resource->m_vertexBuffer.Init(
				vertexStride * numVertex,
				vertexStride
			);

			resource->m_vertexBuffer.Copy(
				(void*)&tkmMesh.vertexBuffer[0]
			);

			auto SetSkinFlag = [&](int index) {
				// その頂点がスキニングするかを判定する。スキニングする場合は1、しない場合は0を設定する。
				if (tkmMesh.vertexBuffer[index].skinWeights.x > 0.0f) {

					resource->m_skinFlags.push_back(1);
				}
				else {

					resource->m_skinFlags.push_back(0);
				}
				};



			///////////////////////////////
			// インデックスバッファを作成。
			///////////////////////////////

			if (!tkmMesh.indexBuffer16Array.empty()) {
				// インデックスのサイズが2byteなら


				// インデックスバッファの配列のサイズを確保する。
				resource->m_indexBufferArray.reserve(
					tkmMesh.indexBuffer16Array.size()
				);

				for (auto& tkIb : tkmMesh.indexBuffer16Array) {

					auto ib = new IndexBuffer;

					ib->Init(
						static_cast<int>(tkIb.indices.size()) * 2,
						2
					);

					ib->Copy((uint16_t*)&tkIb.indices.at(0));

					// スキンがあるかどうかを設定する。
					SetSkinFlag(tkIb.indices[0]);

					//リソースにインデックスバッファを登録する。
					resource->m_indexBufferArray.push_back(ib);
				}
			}
			else {
				// インデックスのサイズが4byteなら


				// インデックスバッファの配列のサイズを確保する。
				resource->m_indexBufferArray.reserve(
					tkmMesh.indexBuffer32Array.size()
				);

				for (auto& tkIb : tkmMesh.indexBuffer32Array) {

					auto ib = new IndexBuffer;

					ib->Init(
						static_cast<int>(tkIb.indices.size()) * 4,
						4
					);

					ib->Copy(
						(uint32_t*)&tkIb.indices.at(0)
					);

					SetSkinFlag(tkIb.indices[0]);

					resource->m_indexBufferArray.push_back(ib);
				}
			}

			// Bankへ保存
			resources[meshNo] = resource;
		}

		// このModelは既存のResourceを参照するだけ
		mesh->m_resource = resources[meshNo];


		//ここまで//////////////////////////////////////////////////////////////

		//3. マテリアルを作成。
		mesh->m_materials.reserve(tkmMesh.materials.size());
		for (auto& tkmMat : tkmMesh.materials) {
			auto mat = new Material;
			mat->InitFromTkmMaterila(
				tkmMat,
				fxFilePath,
				vsEntryPointFunc,
				vsSkinEntryPointFunc,
				psEntryPointFunc,
				colorBufferFormat,
				NUM_SRV_ONE_MATERIAL,
				NUM_CBV_ONE_MATERIAL,
				NUM_CBV_ONE_MATERIAL * materialNum,
				NUM_SRV_ONE_MATERIAL * materialNum,
				alphaBlendMode,
				isDepthWrite,
				isDepthTest,
				cullMode
			);
			//作成したマテリアル数をカウントする。
			materialNum++;
			mesh->m_materials.push_back(mat);
		}

		m_meshs[meshNo] = mesh;

	}

	void MeshParts::BindSkeleton(Skeleton& skeleton)
	{
		m_skeleton = &skeleton;
		//構造化バッファを作成する。
		m_boneMatricesStructureBuffer.Init(
			sizeof(Matrix),
			m_skeleton->GetNumBones(),
			m_skeleton->GetBoneMatricesTopAddress()
		);
	}
	void MeshParts::Draw(
		RenderContext& rc,
		const Matrix& mWorld,
		const Matrix& mView,
		const Matrix& mProj,
		int numInstance
	)
	{
		//メッシュごとにドロー
		//プリミティブのトポロジーはトライアングルリストのみ。
		rc.SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//定数バッファを更新する。
		SConstantBuffer cb;
		cb.mWorld = mWorld;
		cb.mView = mView;
		cb.mProj = mProj;
		m_commonConstantBuffer.CopyToVRAM(cb);

		if (m_expandData) {
			m_expandConstantBuffer.CopyToVRAM(m_expandData);
		}
		if (m_boneMatricesStructureBuffer.IsInited()) {
			//ボーン行列を更新する。
			m_boneMatricesStructureBuffer.Update(m_skeleton->GetBoneMatricesTopAddress());
		}
		int descriptorHeapNo = 0;
		for (auto& mesh : m_meshs) {
			//1. 頂点バッファを設定。
			rc.SetVertexBuffer(mesh->m_resource->m_vertexBuffer);
			//マテリアルごとにドロー。
			for (int matNo = 0; matNo < mesh->m_materials.size(); matNo++) {
				//このマテリアルが貼られているメッシュの描画開始。
				mesh->m_materials[matNo]->BeginRender(rc, mesh->m_resource->m_skinFlags[matNo]);
				//2. ディスクリプタヒープを設定。
				rc.SetDescriptorHeap(m_descriptorHeap);
				//3. インデックスバッファを設定。
				auto& ib = mesh->m_resource->m_indexBufferArray[matNo];
				rc.SetIndexBuffer(*ib);

				//4. ドローコールを実行。
				rc.DrawIndexedInstance(ib->GetCount(), numInstance);
				descriptorHeapNo++;
			}
		}
	}
}