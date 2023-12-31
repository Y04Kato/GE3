#pragma once
#include "DirectXCommon.h"
#include "components/math/Vector.h"
#include "CJEngine.h"
#include<wrl.h>

class CreateSprite{
public:
	void Initialize(DirectXCommon* dxCommon, CitrusJunosEngine* engine);

	void Draw(const Vector4& a, const Vector4& b, const Transform& transform, const Transform& uvTransform, const Vector4& material, uint32_t index, const DirectionalLight& light);
	void Finalize();

private:
	DirectXCommon* dxCommon_;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};
	Microsoft::WRL::ComPtr <ID3D12Resource> vertexResourceSprite_;
	VertexData* vertexData_;
	Microsoft::WRL::ComPtr <ID3D12Resource> transformationMatrixResource_;
	TransformationMatrix* transformationMatrixdata_;
	Microsoft::WRL::ComPtr <ID3D12Resource> materialResource_;
	Material* materialData_;
	CitrusJunosEngine* CJEngine_;
	DirectionalLight* directionalLight_;
	Microsoft::WRL::ComPtr <ID3D12Resource> directionalLightResource_;

	Microsoft::WRL::ComPtr <ID3D12Resource> indexResourceSprite_;
	D3D12_INDEX_BUFFER_VIEW indexBufferViewSprite_{};
	uint32_t* indexDataSprite_;

private:
	void SettingVartex();
	void TransformMatrix();
	void SettingColor();
	void SettingDictionalLight();
};