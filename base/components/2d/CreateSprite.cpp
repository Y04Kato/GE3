#include "CreateSprite.h"

void CreateSprite::Initialize(DirectXCommon* dxCommon, CitrusJunosEngine* engine){
	dxCommon_ = dxCommon;
	CJEngine_ = engine;
	SettingVartex();
	SettingColor();
	TransformMatrix();
	SettingDictionalLight();
}

void CreateSprite::Draw(const Vector4& a, const Vector4& b, const Transform& transform, const Transform& uvTransform, const Vector4& material, uint32_t index, const DirectionalLight& light){
	
	Matrix4x4 uvtransformMtrix = MakeScaleMatrix(uvTransform.scale);
	uvtransformMtrix = Multiply(uvtransformMtrix, MakeRotateZMatrix(uvTransform.rotate.num[2]));
	uvtransformMtrix = Multiply(uvtransformMtrix, MakeTranslateMatrix(uvTransform.translate));
	
	//座標の設定
	vertexData_[0].position = { a.num[0],b.num[1],0.0f,1.0f};
	vertexData_[1].position = { a.num[0],a.num[1],0.0f,1.0f };
	vertexData_[2].position = { b.num[0],b.num[1],0.0f,1.0f };
	vertexData_[3].position = { a.num[0],a.num[1],0.0f,1.0f };
	vertexData_[4].position = { b.num[0],a.num[1],0.0f,1.0f };
	vertexData_[5].position = { b.num[0],b.num[1],0.0f,1.0f };

	//Texcoordの設定
	vertexData_[0].texcoord = { 0.0f,1.0f };
	vertexData_[1].texcoord = { 0.0f,0.0f };
	vertexData_[2].texcoord = { 1.0f,1.0f };
	vertexData_[3].texcoord = { 0.0f,0.0f };
	vertexData_[4].texcoord = { 1.0f,0.0f };
	vertexData_[5].texcoord = { 1.0f,1.0f };

	for (int i = 0; i < 6; i++) {
		vertexData_[i].normal = { 0.0f,0.0f,-1.0f };
	}

	*materialData_ = { material,false };
	materialData_->uvTransform = uvtransformMtrix;
	*directionalLight_ = light;
	
	//Sprite用のworldViewProjectionMatrixを作る
	Matrix4x4 worldMatrix = MakeAffineMatrix(transform.scale, transform.rotate, transform.translate);
	Matrix4x4 viewMatrix = MakeIdentity4x4();
	Matrix4x4 projectionmatrix = MakeOrthographicMatrix(0.0f, 0.0f, (float)dxCommon_->GetWin()->kClientWidth, (float)dxCommon_->GetWin()->kClientHeight, 0.0f, 100.0f);
	Matrix4x4 worldViewProjectionMatrix = Multiply(worldMatrix, Multiply(viewMatrix, projectionmatrix));
	*transformationMatrixdata_ = { worldViewProjectionMatrix,worldMatrix };
	
	//描画
	dxCommon_->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView_);
	dxCommon_->GetCommandList()->IASetIndexBuffer(&indexBufferViewSprite_);
	
	dxCommon_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());
	dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(1, transformationMatrixResource_->GetGPUVirtualAddress());
	dxCommon_->GetCommandList()->SetGraphicsRootDescriptorTable(2, CJEngine_->textureSrvHandleGPU_[index]);
	dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(3, directionalLightResource_->GetGPUVirtualAddress());

	dxCommon_->GetCommandList()->DrawIndexedInstanced(6, 1, 0, 0, 0);
}

void CreateSprite::Finalize(){

}

void CreateSprite::SettingVartex(){
	//Sprite用のリソースを作る
	vertexResourceSprite_ = dxCommon_->CreateBufferResource(dxCommon_->GetDevice(), sizeof(VertexData) * 6);

	//頂点バッファービューを作成し、先頭のアドレスから使う
	vertexBufferView_.BufferLocation = vertexResourceSprite_->GetGPUVirtualAddress();

	//使用するリソースサイズは頂点6つ分
	vertexBufferView_.SizeInBytes = sizeof(VertexData) * 6;

	//1頂点当たりのサイズ
	vertexBufferView_.StrideInBytes = sizeof(VertexData);

	vertexResourceSprite_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));

	//
	indexResourceSprite_ = dxCommon_->CreateBufferResource(dxCommon_->GetDevice(), sizeof(uint32_t) * 6);

	indexBufferViewSprite_.BufferLocation = indexResourceSprite_->GetGPUVirtualAddress();

	indexBufferViewSprite_.SizeInBytes = sizeof(uint32_t) * 6;

	indexBufferViewSprite_.Format = DXGI_FORMAT_R32_UINT;

	indexResourceSprite_->Map(0, nullptr, reinterpret_cast<void**>(&indexDataSprite_));

	indexDataSprite_[0] = 0;
	indexDataSprite_[1] = 1;
	indexDataSprite_[2] = 2;
	indexDataSprite_[3] = 3;
	indexDataSprite_[4] = 4;
	indexDataSprite_[5] = 5;
}

void CreateSprite::TransformMatrix(){
	//Sprite用のTransformationMatrix用のリソースを作る Matrix4x41つ分を用意する
	transformationMatrixResource_ = dxCommon_->CreateBufferResource(dxCommon_->GetDevice(), sizeof(TransformationMatrix));
	transformationMatrixdata_ = nullptr;//書き込む

	//書き込むアドレスを取得
	transformationMatrixResource_->Map(0, nullptr, reinterpret_cast<void**>(&transformationMatrixdata_));
	//単位行列を書き込んでおく
	transformationMatrixdata_->WVP = MakeIdentity4x4();
}

void CreateSprite::SettingColor() {
	materialResource_ = dxCommon_->CreateBufferResource(dxCommon_->GetDevice(), sizeof(Material));

	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
}

void CreateSprite::SettingDictionalLight() {
	directionalLightResource_ = DirectXCommon::CreateBufferResource(dxCommon_->GetDevice(), sizeof(DirectionalLight));
	directionalLightResource_->Map(0, NULL, reinterpret_cast<void**>(&directionalLight_));
}