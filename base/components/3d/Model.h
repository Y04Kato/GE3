#pragma once
#include "DirectXCommon.h"
#include "CJEngine.h"
#include "components/math/Vector.h"
#include <string>
#include <fstream>
#include <sstream>
#include<wrl.h>

class Model {
public:
	void Initialize(DirectXCommon* dxCommon, CitrusJunosEngine* engine, const std::string& directoryPath, const std::string& filename, uint32_t index);

	void Draw(const Vector4& material, const Transform& transform, uint32_t index, const Matrix4x4 viewMatrix, const DirectionalLight& light);
	
	void Finalize();

	ModelData modelData_;
	
	ModelData LoadObjFile(const std::string& directoryPath, const std::string& filename);
	MaterialData LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename);

private:
	DirectXCommon* dxCommon_;
	CitrusJunosEngine* CJEngine_;

	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};
	Microsoft::WRL::ComPtr <ID3D12Resource> vertexResource_;
	VertexData* vertexData_;

	Microsoft::WRL::ComPtr <ID3D12Resource> wvpResource_;
	TransformationMatrix* wvpData_;
	Microsoft::WRL::ComPtr <ID3D12Resource> materialResource_;
	Material* material_;

	DirectionalLight* directionalLight_;
	Microsoft::WRL::ComPtr <ID3D12Resource> directionalLightResource_;

private:
	void CreateVartexData();
	void SetColor();
	void TransformMatrix();
	void CreateDictionalLight();

};