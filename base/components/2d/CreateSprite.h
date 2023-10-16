#pragma once
#include "DirectXCommon.h"
#include "CJEngine.h"
#include "components/3d/WorldTransform.h"
#include "components/manager/TextureManager.h"
#include<wrl.h>

class CreateSprite{
public:
	void Initialize(const Vector4& a, const Vector4& b);

	void Draw(const Transform& transform, const Transform& uvTransform, const Vector4& material, uint32_t index);
	void Finalize();

private:
	DirectXCommon* dxCommon_;
	TextureManager* textureManager_;

	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};
	Microsoft::WRL::ComPtr <ID3D12Resource> vertexResourceSprite_;
	VertexData* vertexData_;
	
	Microsoft::WRL::ComPtr <ID3D12Resource> materialResource_;
	Material* materialData_;
	
	CitrusJunosEngine* CJEngine_;

	Microsoft::WRL::ComPtr <ID3D12Resource> indexResourceSprite_;
	D3D12_INDEX_BUFFER_VIEW indexBufferViewSprite_{};
	uint32_t* indexDataSprite_;

	Microsoft::WRL::ComPtr<ID3D12Resource> wvpResource_;
	Matrix4x4* wvpData_;
private:
	void SettingVertex(const Vector4& a, const Vector4& b);
	void SettingColor();
	void SettingTransform();
};