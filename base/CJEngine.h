#pragma once
#include "DirectXCommon.h"
#include "components/math/Vector.h"
#include "components/2d/CreateTriangle.h"
#include "components/math/MatrixCalculation.h"

#include "DirectXTex/d3dx12.h"
#include <dxcapi.h>
#pragma comment(lib,"dxcompiler.lib")
#include<vector>
#include<wrl.h>

enum BlendMode {
	//ブレンドなし
	kBlendModeNone,
	//通常のαブレンド
	kBlendModeNormal,
	//加算ブレンド
	kBlendModeAdd,
	//減産ブレンド
	kBlendModeSubtract,
	//乗算ブレンド
	kBlendModeMultiply,
	//スクリーンブレンド
	kBlendModeScreen,
	//計算用、利用しない
	kCountOfBlendMode,
};

class CitrusJunosEngine {
public:
	static CitrusJunosEngine* GetInstance();

	void Initialize(const wchar_t* title, int32_t width, int32_t height);

	void BeginFrame();

	void EndFrame();

	void Finalize();

	void Update();

	DirectXCommon* GetDirectXCommon() { return dxCommon_; }

	CitrusJunosEngine(const CitrusJunosEngine& obj) = delete;
	CitrusJunosEngine& operator=(const CitrusJunosEngine& obj) = delete;

private:
	CitrusJunosEngine() = default;
	~CitrusJunosEngine() = default;

	static	DirectXCommon* dxCommon_;

	IDxcUtils* dxcUtils_;
	IDxcCompiler3* dxcCompiler_;

	IDxcIncludeHandler* includeHandler_;

	Microsoft::WRL::ComPtr <ID3DBlob> signatureBlob_;
	Microsoft::WRL::ComPtr <ID3DBlob> errorBlob_;
	Microsoft::WRL::ComPtr <ID3D12RootSignature> rootSignature_;

	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc_{};

	D3D12_BLEND_DESC blendDesc_[kCountOfBlendMode];

	Microsoft::WRL::ComPtr <IDxcBlob> vertexShaderBlob_;

	Microsoft::WRL::ComPtr <IDxcBlob> pixelShaderBlob_;

	D3D12_RASTERIZER_DESC rasterizerDesc_{};

	Microsoft::WRL::ComPtr <ID3D12PipelineState> graphicsPipelineState_;

	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_;

	D3D12_VIEWPORT viewport_{};
	D3D12_RECT scissorRect_{};

	D3D12_INPUT_ELEMENT_DESC inputElementDescs_[3];

	//頂点リソースにデータを書き込む
	Vector4* vertexData_;

	D3D12_DEPTH_STENCIL_DESC depthStencilDesc_{};


	IDxcBlob* CompileShader(
		//CompileShaderするShaderファイルへのパス
		const std::wstring& filePath,
		//Compielerに使用するProfile
		const wchar_t* profile,
		//初期化で生成したものを3つ
		IDxcUtils* dxcUtils,
		IDxcCompiler3* dxcCompiler,
		IDxcIncludeHandler* includeHandler
	);

	void InitializeDxcCompiler();
	void CreateRootSignature();
	void CreateInputlayOut();
	void BlendState();
	void RasterizerState();
	void InitializePSO();
	void ViewPort();
	void ScissorRect();
	void SettingDepth();
};