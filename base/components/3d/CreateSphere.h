#pragma once
#include "DirectXCommon.h"
#include "components/3d/WorldTransform.h"
#include "components/3d/ViewProjection.h"
#include "components/manager/TextureManager.h"
#include<wrl.h>

class CitrusJunosEngine;

class CreateSphere{
public:
	void Initialize();

	void Draw(const WorldTransform& worldTransform, const ViewProjection& viewProjection, const Vector4& material, uint32_t index, const DirectionalLight& light);
	
	void Finalize();

private:
	void SettingVertex();
	void SettingColor();
	void TransformMatrix();
	void SettingDictionalLight();

private:
	DirectXCommon* dxCommon_;
	CitrusJunosEngine* CJEngine_;
	TextureManager* textureManager_;

	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};
	Microsoft::WRL::ComPtr <ID3D12Resource> vertexResource_;
	VertexData* vertexData_;

	Microsoft::WRL::ComPtr <ID3D12Resource> wvpResource_;
	TransformationMatrix* wvpData_;

	Microsoft::WRL::ComPtr <ID3D12Resource> materialResource_;
	Material* materialData_;

	const float pi = 3.1415f;
	uint32_t kSubDivision_;
	uint32_t vertexCount_;

	DirectionalLight* directionalLight_;
	Microsoft::WRL::ComPtr <ID3D12Resource> directionalLightResource_;

	Microsoft::WRL::ComPtr <ID3D12Resource> indexResourceSphere_;
	D3D12_INDEX_BUFFER_VIEW indexBufferViewSphere_{};
	uint32_t* indexDataSphere_;
};