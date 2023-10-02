#include "CreateTriangle.h"
#include "CJEngine.h"
#include <assert.h>

void CreateTriangle::Initialize(DirectXCommon* dxCommon, CitrusJunosEngine* engine) {
	dxCommon_ = dxCommon;
	CJEngine_ =  engine;
	SettingVertex();
	SettingColor();
	TransformMatrix();
	SettingDictionalLight();
}

void CreateTriangle::Draw(const TriangleData& data, const Transform& transform, const Matrix4x4 viewMatrix, uint32_t index, const DirectionalLight& light) {
	Matrix4x4 worldMatrix = MakeAffineMatrix(transform.scale, transform.rotate, transform.translate);
	Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(dxCommon_->GetWin()->kClientWidth) / float(dxCommon_->GetWin()->kClientHeight), 0.1f, 100.0f);

	Matrix4x4 wvpMatrix = Multiply(worldMatrix, Multiply(viewMatrix, projectionMatrix));

	Transform uvTransform = { { 1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f}, {0.0f,0.0f,0.0f} };
	Matrix4x4 uvtransformMtrix = MakeScaleMatrix(uvTransform.scale);
	uvtransformMtrix = Multiply(uvtransformMtrix, MakeRotateZMatrix(uvTransform.rotate.num[2]));
	uvtransformMtrix = Multiply(uvtransformMtrix, MakeTranslateMatrix(uvTransform.translate));

	//左下
	vertexData_[0].position = data.position[0];
	vertexData_[0].texcoord = { 0.0f,1.0f };

	//上
	vertexData_[1].position = data.position[1];
	vertexData_[1].texcoord = { 0.5f,0.0f };

	//右下
	vertexData_[2].position = data.position[2];
	vertexData_[2].texcoord = { 1.0f,1.0f };

	*materialData_ = { data.material,false };
	materialData_->uvTransform = uvtransformMtrix;
	*wvpData_ = { wvpMatrix,worldMatrix };
	*directionalLight_ = light;

	//VBVを設定
	dxCommon_->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView_);

	//形状を設定。PS0に設定しているものとはまた別。同じものを設定する
	dxCommon_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//マテリアルCBufferの場所を設定
	dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(3, directionalLightResource_->GetGPUVirtualAddress());
	dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());
	dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(1, wvpResource_->GetGPUVirtualAddress());

	//SRVのDescriptorTableの先頭を設定。2はrootParameter[2]のこと
	dxCommon_->GetCommandList()->SetGraphicsRootDescriptorTable(2, CJEngine_->textureSrvHandleGPU_[index]);

	//描画
	dxCommon_->GetCommandList()->DrawInstanced(3, 1, 0, 0);
}

void CreateTriangle::Finalize() {

}

void CreateTriangle::SettingVertex() {
	vertexResource_ = dxCommon_->CreateBufferResource(dxCommon_->GetDevice(), sizeof(VertexData) * 3);
	//リソースの先頭のアドレスから使う
	vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress();
	//使用するリソースのサイズは頂点3つ分のサイズ
	vertexBufferView_.SizeInBytes = sizeof(VertexData) * 3;
	//1頂点当たりのサイズ
	vertexBufferView_.StrideInBytes = sizeof(VertexData);
	//書き込むためのアドレスを取得
	vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));
}

void CreateTriangle::TransformMatrix() {
	wvpResource_ = dxCommon_->CreateBufferResource(dxCommon_->GetDevice(), sizeof(TransformationMatrix));
	wvpResource_->Map(0, NULL, reinterpret_cast<void**>(&wvpData_));
	wvpData_->WVP = MakeIdentity4x4();
}

void CreateTriangle::SettingColor() {
	//マテリアル用のリソースを作る　今回はcolor1つ分
	materialResource_ = dxCommon_->CreateBufferResource(dxCommon_->GetDevice(), sizeof(Material));
	//書き込むためのアドレスを取得
	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
}

void CreateTriangle::SettingDictionalLight() {
	directionalLightResource_ = DirectXCommon::CreateBufferResource(dxCommon_->GetDevice(), sizeof(DirectionalLight));
	directionalLightResource_->Map(0, NULL, reinterpret_cast<void**>(&directionalLight_));
}