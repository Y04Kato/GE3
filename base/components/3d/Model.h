#pragma once
#include "DirectXCommon.h"
#include "CJEngine.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "TextureManager.h"
#include <string>
#include <fstream>
#include <sstream>
#include<wrl.h>

class Model {
public:
	void Initialize(const std::string& directoryPath, const std::string& filename);

	void Draw(const WorldTransform& worldTransform, const ViewProjection& viewProjection, const Vector4& material, const DirectionalLight& light);
	
	void Finalize();

	ModelData modelData_;
	
	ModelData LoadObjFile(const std::string& directoryPath, const std::string& filename);
	MaterialData LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename);

	static Model* CreateModelFromObj(const std::string& directoryPath, const std::string& filename);

private:
	DirectXCommon* dxCommon_;
	CitrusJunosEngine* CJEngine_;
	TextureManager* textureManager_;

	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};
	Microsoft::WRL::ComPtr <ID3D12Resource> vertexResource_;
	VertexData* vertexData_;

	Microsoft::WRL::ComPtr <ID3D12Resource> wvpResource_;
	Microsoft::WRL::ComPtr <ID3D12Resource> materialResource_;
	Material* material_;

	uint32_t texture_;

	DirectionalLight* directionalLight_;
	Microsoft::WRL::ComPtr <ID3D12Resource> directionalLightResource_;

private:
	void CreateVartexData();
	void SetColor();
	void CreateDictionalLight();

};