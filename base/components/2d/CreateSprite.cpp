#include "CreateSprite.h"

void CreateSprite::Initialize(Vector2 size, Vector2 anchor, bool isFlipX, bool isFlipY) {
	dxCommon_ = DirectXCommon::GetInstance();
	CJEngine_ = CitrusJunosEngine::GetInstance();
	textureManager_ = TextureManager::GetInstance();
	SettingVertex(size, anchor,isFlipX,isFlipY);
	SettingColor();
	SettingTransform();
}

void CreateSprite::Draw(const Transform& transform, const Transform& uvTransform, const Vector4& material, uint32_t index) {
	Matrix4x4 uvtransformMtrix = MakeScaleMatrix(uvTransform.scale);
	uvtransformMtrix = Multiply(uvtransformMtrix, MakeRotateZMatrix(uvTransform.rotate.num[2]));
	uvtransformMtrix = Multiply(uvtransformMtrix, MakeTranslateMatrix(uvTransform.translate));

	*materialData_ = { material,false };
	materialData_->uvTransform = uvtransformMtrix;

	//Sprite用のworldViewProjectionMatrixを作る
	Matrix4x4 worldMatrix = MakeAffineMatrix(transform.scale, transform.rotate, transform.translate);
	Matrix4x4 viewMatrix = MakeIdentity4x4();
	Matrix4x4 projectionmatrix = MakeOrthographicMatrix(0.0f, 0.0f, (float)dxCommon_->GetWin()->kClientWidth, (float)dxCommon_->GetWin()->kClientHeight, 0.0f, 100.0f);
	Matrix4x4 worldViewProjectionMatrix = Multiply(worldMatrix, Multiply(viewMatrix, projectionmatrix));
	*wvpData_ = worldViewProjectionMatrix;

	//描画
	dxCommon_->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView_);
	dxCommon_->GetCommandList()->IASetIndexBuffer(&indexBufferViewSprite_);

	dxCommon_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());
	dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(1, wvpResource_->GetGPUVirtualAddress());
	dxCommon_->GetCommandList()->SetGraphicsRootDescriptorTable(2, textureManager_->GetGPUHandle(index));

	dxCommon_->GetCommandList()->DrawIndexedInstanced(6, 1, 0, 0, 0);
}

void CreateSprite::Finalize() {

}

void CreateSprite::SettingVertex(Vector2 size, Vector2 anchor, bool isFlipX, bool isFlipY) {
	//Sprite用のリソースを作る
	vertexResourceSprite_ = dxCommon_->CreateBufferResource(dxCommon_->GetDevice(), sizeof(VertexData) * 6);

	//頂点バッファービューを作成し、先頭のアドレスから使う
	vertexBufferView_.BufferLocation = vertexResourceSprite_->GetGPUVirtualAddress();

	//使用するリソースサイズは頂点6つ分
	vertexBufferView_.SizeInBytes = sizeof(VertexData) * 6;

	//1頂点当たりのサイズ
	vertexBufferView_.StrideInBytes = sizeof(VertexData);

	vertexResourceSprite_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));

	indexResourceSprite_ = dxCommon_->CreateBufferResource(dxCommon_->GetDevice(), sizeof(uint32_t) * 6);

	indexBufferViewSprite_.BufferLocation = indexResourceSprite_->GetGPUVirtualAddress();

	indexBufferViewSprite_.SizeInBytes = sizeof(uint32_t) * 6;

	indexBufferViewSprite_.Format = DXGI_FORMAT_R32_UINT;

	indexResourceSprite_->Map(0, nullptr, reinterpret_cast<void**>(&indexDataSprite_));

	float left = (0.0f - anchor.num[0]) * size.num[0];
	float right = (1.0f - anchor.num[0]) * size.num[0];
	float top = (0.0f - anchor.num[1]) * size.num[1];
	float bottom = (1.0f - anchor.num[1]) * size.num[1];

	if (isFlipX) {
		left = -left;
		right = -right;
	}
	if (isFlipY) {
		top = -top;
		bottom = -bottom;
	}

	//座標の設定
	vertexData_[0].position = { left,bottom,0.0f,1.0f };
	vertexData_[1].position = { left,top,0.0f,1.0f };
	vertexData_[2].position = { right,bottom,0.0f,1.0f };
	vertexData_[3].position = { right,top,0.0f,1.0f };

	//Texcoordの設定
	vertexData_[0].texcoord = { 0.0f,1.0f };
	vertexData_[1].texcoord = { 0.0f,0.0f };
	vertexData_[2].texcoord = { 1.0f,1.0f };
	vertexData_[3].texcoord = { 1.0f,0.0f };

	indexDataSprite_[0] = 0;
	indexDataSprite_[1] = 1;
	indexDataSprite_[2] = 2;
	indexDataSprite_[3] = 1;
	indexDataSprite_[4] = 3;
	indexDataSprite_[5] = 2;

	for (int i = 0; i < 6; i++) {
		vertexData_[i].normal = { 0.0f,0.0f,-1.0f };
	}
}

void CreateSprite::SettingColor() {
	materialResource_ = dxCommon_->CreateBufferResource(dxCommon_->GetDevice(), sizeof(Material));

	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
	materialData_->uvTransform = MakeIdentity4x4();
}

void CreateSprite::SettingTransform() {
	wvpResource_ = DirectXCommon::CreateBufferResource(dxCommon_->GetDevice().Get(), sizeof(Matrix4x4));
	wvpResource_->Map(0, NULL, reinterpret_cast<void**>(&wvpData_));
	*wvpData_ = MakeIdentity4x4();
}