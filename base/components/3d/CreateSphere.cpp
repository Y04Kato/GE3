#include "CreateSphere.h"
#include "CJEngine.h"
#include<cmath>

void CreateSphere::Initialize(DirectXCommon* dxCommon, CitrusJunosEngine* engine) {
	dxCommon_ = dxCommon;
	CJEngine_ = engine;
	kSubDivision_ = 16;
	vertexCount_ = kSubDivision_ * kSubDivision_ * 6;
	SettingVertex();
	SettingColor();
	TransformMatrix();
	SettingDictionalLight();
}

void CreateSphere::Draw(const Vector4& material, const Transform& transform, uint32_t index, const Matrix4x4 viewMatrix, const DirectionalLight& light) {
	Matrix4x4 worldMatrix = MakeAffineMatrix(transform.scale, transform.rotate, transform.translate);
	Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(dxCommon_->GetWin()->kClientWidth) / float(dxCommon_->GetWin()->kClientHeight), 0.1f, 100.0f);

	Matrix4x4 wvpMatrix = Multiply(worldMatrix, Multiply(viewMatrix, projectionMatrix));

	Transform uvTransform = { { 1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f}, {0.0f,0.0f,0.0f} };
	Matrix4x4 uvtransformMtrix = MakeScaleMatrix(uvTransform.scale);
	uvtransformMtrix = Multiply(uvtransformMtrix, MakeRotateZMatrix(uvTransform.rotate.num[2]));
	uvtransformMtrix = Multiply(uvtransformMtrix, MakeTranslateMatrix(uvTransform.translate));

	//経度分割一つ分の角度
	const float kLonEvery = pi * 2.0f / float(kSubDivision_);
	const float kLatEvery = pi / float(kSubDivision_);

	//緯度の方向に分割
	for (uint32_t latIndex = 0; latIndex < kSubDivision_; ++latIndex) {
		float lat = -pi / 2.0f + kLatEvery * latIndex;
		//経度の方向に分割しながら線を描く
		for (uint32_t lonIndex = 0; lonIndex < kSubDivision_; ++lonIndex) {
			uint32_t start = (latIndex * kSubDivision_ + lonIndex) * 6;
			float lon = lonIndex * kLonEvery;
			//頂点にデータを入力する　基準点a
			vertexData_[start].position = { cos(lat) * cos(lon),sin(lat),cos(lat) * sin(lon),1.0f };
			vertexData_[start].texcoord = { float(lonIndex) / float(kSubDivision_),1.0f - float(latIndex) / kSubDivision_ };

			vertexData_[start + 1].position = { cos(lat + kLatEvery) * cos(lon),sin(lat + kLatEvery),cos(lat + kLatEvery) * sin(lon),1.0f };
			vertexData_[start + 1].texcoord = { vertexData_[start].texcoord.num[0],vertexData_[start].texcoord.num[1] - 1.0f / float(kSubDivision_) };

			vertexData_[start + 2].position = { cos(lat) * cos(lon + kLonEvery),sin(lat),cos(lat) * sin(lon + kLonEvery),1.0f };
			vertexData_[start + 2].texcoord = { vertexData_[start].texcoord.num[0] + 1.0f / float(kSubDivision_),vertexData_[start].texcoord.num[1] };

			vertexData_[start + 3].position = { cos(lat) * cos(lon + kLonEvery),sin(lat),cos(lat) * sin(lon + kLonEvery),1.0f };
			vertexData_[start + 3].texcoord = { vertexData_[start].texcoord.num[0] + 1.0f / float(kSubDivision_),vertexData_[start].texcoord.num[1] };

			vertexData_[start + 4].position = { cos(lat + kLatEvery) * cos(lon),sin(lat + kLatEvery),cos(lat + kLatEvery) * sin(lon),1.0f };
			vertexData_[start + 4].texcoord = { vertexData_[start].texcoord.num[0],vertexData_[start].texcoord.num[1] - 1.0f / float(kSubDivision_) };

			vertexData_[start + 5].position = { cos(lat + kLatEvery) * cos(lon + kLonEvery),sin(lat + kLatEvery), cos(lat + kLatEvery) * sin(lon + kLonEvery),1.0f };
			vertexData_[start + 5].texcoord = { vertexData_[start].texcoord.num[0] + 1.0f / float(kSubDivision_),vertexData_[start].texcoord.num[1] - 1.0f / float(kSubDivision_) };

			vertexData_[start].normal.num[0] = vertexData_[start].position.num[0];
			vertexData_[start].normal.num[1] = vertexData_[start].position.num[1];
			vertexData_[start].normal.num[2] = vertexData_[start].position.num[2];

			vertexData_[start + 1].normal.num[0] = vertexData_[start + 1].position.num[0];
			vertexData_[start + 1].normal.num[1] = vertexData_[start + 1].position.num[1];
			vertexData_[start + 1].normal.num[2] = vertexData_[start + 1].position.num[2];

			vertexData_[start + 2].normal.num[0] = vertexData_[start + 2].position.num[0];
			vertexData_[start + 2].normal.num[1] = vertexData_[start + 2].position.num[1];
			vertexData_[start + 2].normal.num[2] = vertexData_[start + 2].position.num[2];

			vertexData_[start + 3].normal.num[0] = vertexData_[start + 3].position.num[0];
			vertexData_[start + 3].normal.num[1] = vertexData_[start + 3].position.num[1];
			vertexData_[start + 3].normal.num[2] = vertexData_[start + 3].position.num[2];

			vertexData_[start + 4].normal.num[0] = vertexData_[start + 4].position.num[0];
			vertexData_[start + 4].normal.num[1] = vertexData_[start + 4].position.num[1];
			vertexData_[start + 4].normal.num[2] = vertexData_[start + 4].position.num[2];

			vertexData_[start + 5].normal.num[0] = vertexData_[start + 5].position.num[0];
			vertexData_[start + 5].normal.num[1] = vertexData_[start + 5].position.num[1];
			vertexData_[start + 5].normal.num[2] = vertexData_[start + 5].position.num[2];
		}
	}

	*materialData_ = { material,true };
	materialData_->uvTransform = uvtransformMtrix;
	*wvpData_ = { wvpMatrix,worldMatrix };
	*directionalLight_ = light;

	//VBVを設定
	dxCommon_->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView_);
	dxCommon_->GetCommandList()->IASetIndexBuffer(&indexBufferViewSphere_);

	//形状を設定。PS0に設定しているものとはまた別。同じものを設定する
	dxCommon_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//マテリアルCBufferの場所を設定
	dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());
	dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(1, wvpResource_->GetGPUVirtualAddress());
	dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(3, directionalLightResource_->GetGPUVirtualAddress());

	//SRVのDescriptorTableの先頭を設定。2はrootParameter[2]のこと
	dxCommon_->GetCommandList()->SetGraphicsRootDescriptorTable(2, CJEngine_->textureSrvHandleGPU_[index]);

	//描画
	dxCommon_->GetCommandList()->DrawIndexedInstanced(vertexCount_, 1, 0, 0, 0);

}

void CreateSphere::Finalize() {

}

void CreateSphere::SettingVertex() {
	vertexResource_ = dxCommon_->CreateBufferResource(dxCommon_->GetDevice(), sizeof(VertexData) * vertexCount_);
	//リソースの先頭のアドレスから使う
	vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress();

	vertexBufferView_.SizeInBytes = sizeof(VertexData) * vertexCount_;

	vertexBufferView_.StrideInBytes = sizeof(VertexData);

	vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));

	indexResourceSphere_ = dxCommon_->CreateBufferResource(dxCommon_->GetDevice(), sizeof(uint32_t) * vertexCount_);

	indexBufferViewSphere_.BufferLocation = indexResourceSphere_->GetGPUVirtualAddress();

	indexBufferViewSphere_.SizeInBytes = sizeof(uint32_t) * vertexCount_;

	indexBufferViewSphere_.Format = DXGI_FORMAT_R32_UINT;

	indexResourceSphere_->Map(0, nullptr, reinterpret_cast<void**>(&indexDataSphere_));

	for (uint32_t i = 0; i < vertexCount_; i++) {
		indexDataSphere_[i] = i;
	}
}

void CreateSphere::TransformMatrix() {
	wvpResource_ = dxCommon_->CreateBufferResource(dxCommon_->GetDevice(), sizeof(TransformationMatrix));
	wvpResource_->Map(0, NULL, reinterpret_cast<void**>(&wvpData_));
	wvpData_->WVP = MakeIdentity4x4();
}

void CreateSphere::SettingColor() {
	materialResource_ = dxCommon_->CreateBufferResource(dxCommon_->GetDevice(), sizeof(Material));

	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
}

void CreateSphere::SettingDictionalLight() {
	directionalLightResource_ = DirectXCommon::CreateBufferResource(dxCommon_->GetDevice(), sizeof(DirectionalLight));
	directionalLightResource_->Map(0, NULL, reinterpret_cast<void**>(&directionalLight_));
}