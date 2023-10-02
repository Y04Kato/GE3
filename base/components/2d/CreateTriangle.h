#pragma once
#include "DirectXCommon.h"
#include "components/math/Vector.h"
#include "components/math/MatrixCalculation.h"
#include<wrl.h>

class CitrusJunosEngine;

class CreateTriangle {
public:
	void Initialize(DirectXCommon* dxCommon, CitrusJunosEngine* engine);

	void Draw(const TriangleData& data,const Transform& transform, const Matrix4x4 viewMatrix, uint32_t index, const DirectionalLight& light);

	void Finalize();

private:
	void SettingVertex();

	void SettingColor();

	void TransformMatrix();

	void SettingDictionalLight();

private:
	CitrusJunosEngine* CJEngine_;

	DirectXCommon* dxCommon_;

	VertexData* vertexData_;

	Material* materialData_;

	Microsoft::WRL::ComPtr <ID3D12Resource> vertexResource_;

	Microsoft::WRL::ComPtr <ID3D12Resource> materialResource_;

	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_;

	//WVP用のリソース
	Microsoft::WRL::ComPtr <ID3D12Resource> wvpResource_;
	TransformationMatrix* wvpData_;

	DirectionalLight* directionalLight_;
	Microsoft::WRL::ComPtr <ID3D12Resource> directionalLightResource_;
};
